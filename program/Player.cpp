#include "Player.h"
#include "Main.h"
#include "Game.h"

// 回転スピード
constexpr float PLAYER_ROT_SPEED = 2.0f;

// 通常移動速度
constexpr float PLAYER_MOV_SPEED = 0.3f;

// 最大移動速度
constexpr float PLAYER_MAX_SPEED = 0.6f;

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void Player::Init(int model, Float3 pos)
{
	m_model = model;

	// 初期座標の設定（Y座標を少し高い位置に配置）
	m_pos.Set(0.0f, 0.0f, -5.0f);

	m_hp = 10;

	m_rot.Clear();

	// 単位行列で変換行列を初期化
	m_mat = MGetIdent();

	m_kbVelX = 0.0f;
	m_kbVelZ = 0.0f;

	m_startFrame = 0;
}

//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void Player::Update(int way, float movX, float movY)
{
	// 前フレームの座標を保存
	m_prevPos = m_pos;

	// キー入力から移動ベクトル（方向）を計算
	Float3 moveDir(0.0f, 0.0f, 0.0f);
	bool isMoving = false;

	if (CheckHitKey(KEY_INPUT_UP)) {
		moveDir.z += 1.0f; // 上
		isMoving = true;
	}
	if (CheckHitKey(KEY_INPUT_DOWN)) {
		moveDir.z -= 1.0f; // 下
		isMoving = true;
	}
	if (CheckHitKey(KEY_INPUT_LEFT)) {
		moveDir.x -= 1.0f; // 左
		isMoving = true;
	}
	if (CheckHitKey(KEY_INPUT_RIGHT)) {
		moveDir.x += 1.0f; // 右
		isMoving = true;
	}

	// 移動と回転（向き・傾き）の計算
	Float3 mov(0.0f, 0.0f, 0.0f);

	// 目標とする傾きの角度
	float targetRotX = 0.0f;
	float targetRotZ = 0.0f;

	if (isMoving) {
		// ベクトルの長さを計算
		float length = sqrtf(moveDir.x * moveDir.x + moveDir.z * moveDir.z);

		// ゼロ除算防止チェック
		if (length > 0.0001f) {
			moveDir.x /= length;
			moveDir.z /= length;

			// プレイヤー自身の移動速度を計算
			mov.x = moveDir.x * m_moveSpeed * 0.1f;
			mov.z = moveDir.z * m_moveSpeed * 0.1f;

			// 【Y軸：船首の向き】
			float targetRotY = (float)atan2f(moveDir.x, moveDir.z) * (180.0f / DX_PI);

			float rotDiff = targetRotY - m_rot.y;
			while (rotDiff < -180.0f) rotDiff += 360.0f;
			while (rotDiff > 180.0f) rotDiff -= 360.0f;
			m_rot.y += rotDiff * 0.15f;

			// 【X軸：船首の持ち上がり（ピッチ）】
			targetRotX = -10.0f;

			// 【Z軸：旋回時の傾き（ロール）】
			targetRotZ = -rotDiff * 1.5f;
		}
	}
	else {
		// 入力がない時
		targetRotX = 0.0f;
		targetRotZ = 0.0f;
	}

	// -------------------------------------------------------------
	// 水流による影響（逆流による押し流し効果）の追加
	// -------------------------------------------------------------
	Float3 waterForce(0.0f, 0.0f, 0.0f);
	float waterSpeed = 0.08f;

	// 流水の方向（way）に応じて、プレイヤーに働く水流の力を設定
	switch (way)
	{
	case 0: // UP
		waterForce.z = waterSpeed;
		break;

	case 1: // DOWN
		waterForce.z = -waterSpeed;
		break;

	case 2: // LEFT
		waterForce.x = -waterSpeed;
		break;

	case 3: // RIGHT
		waterForce.x = waterSpeed;
		break;
	}

	// プレイヤーの移動量に水流の力を加算
	mov.x += waterForce.x;
	mov.z += waterForce.z;

	// -------------------------------------------------------------
	// 最終的な角度と座標の適用
	// -------------------------------------------------------------
	// 【傾きの補間処理】
	m_rot.x += (targetRotX - m_rot.x) * 0.1f;
	m_rot.z += (targetRotZ - m_rot.z) * 0.1f;

	// 各軸の回転角度を安全のために制限
	m_rot.x = min(max(-30.0f, m_rot.x), 30.0f);
	m_rot.z = min(max(-25.0f, m_rot.z), 25.0f);

	// 各軸の回転行列を作成
	MATRIX mat_y = MGetRotY(TO_RADIAN(m_rot.y));
	MATRIX mat_z = MGetRotZ(TO_RADIAN(m_rot.z));
	MATRIX mat_x = MGetRotX(TO_RADIAN(m_rot.x));

	// 描画用の合成回転行列を作成
	m_mat = MMult(mat_z, mat_x);
	m_mat = MMult(m_mat, mat_y);

	// 移動量とノックバック速度から仮の次フレーム座標を計算
	Float3 nextPos = m_pos + mov;

	nextPos.x += m_kbVelX;
	nextPos.z += m_kbVelZ;

	// ノックバック速度の減衰処理
	m_kbVelX *= 0.85f;
	m_kbVelZ *= 0.85f;

	if (fabsf(m_kbVelX) < 0.001f) m_kbVelX = 0.0f;
	if (fabsf(m_kbVelZ) < 0.001f) m_kbVelZ = 0.0f;

	// -------------------------------------------------------------
	// 【3D画面内完全制限】画面外に行こうとしたら座標を戻す
	// -------------------------------------------------------------
	VECTOR dxNextPos = VGet(nextPos.x, nextPos.y, nextPos.z);
	VECTOR screenPos = ConvWorldPosToScreenPos(dxNextPos);

	// 画面の端からどれくらい内側に制限するか（マージン）
	float margin = 2.0f;

	// 画面の有効範囲（ピクセル）を定義
	float minX = margin;
	float maxX = (float)SCREEN_W - margin;
	float minY = margin;
	float maxY = (float)SCREEN_H - margin;

	// 開始数フレームは画面外判定をスキップ（初期化時の安全対策）
	if (m_startFrame < 5)
	{
		m_startFrame++;
		m_pos = nextPos;
		return;
	}

	// 画面外への侵入を検知した場合のノックバック処理
	if (screenPos.x < minX || screenPos.x > maxX || screenPos.y < minY + 20.0f || screenPos.y > maxY)
	{
		m_hp--; // ダメージ処理

		// ステージ中心へ向かうベクトルを計算して弾き返す
		Float3 centerPos(0.0f, m_pos.y, 0.0f);

		float dirX = centerPos.x - m_pos.x;
		float dirZ = centerPos.z - m_pos.z;

		float len = sqrtf(dirX * dirX + dirZ * dirZ);
		if (len > 0.0f)
		{
			dirX /= len;
			dirZ /= len;
		}

		float knockbackForce = 1.0f;

		m_kbVelX = dirX * knockbackForce;
		m_kbVelZ = dirZ * knockbackForce;
	}
	else
	{
		// 画面内の場合は座標更新を確定
		m_pos = nextPos;
	}

	// 波の起伏によるY座標（高さ）の動的変動計算
	float timeSec = GetNowCount() * 0.001f;
	float waveHeight = 0.15f;
	float waveSpeed = 2.5f;

	m_pos.y = sinf((m_pos.x) * 0.1f + timeSec * waveSpeed + movX * 10.0f) * waveHeight
		+ cosf((m_pos.z) * 0.1f + timeSec * waveSpeed * 1.2f + movY * 10.0f) * waveHeight;

	// 回転行列に平行移動行列（座標）を合成して最終的な行列を確定
	MATRIX mat_pos = MGetTranslate(m_pos.VGet());
	m_mat = MMult(m_mat, mat_pos);
}

//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void Player::Render()
{
	// プレイヤーモデルの座標を設定
	MV1SetPosition(m_model, VGet(m_pos.x, m_pos.y, m_pos.z));

	// 独自の合成変換行列をモデルに適用
	MV1SetRotationMatrix(m_model, m_mat);

	// 3Dモデルを描画
	MV1DrawModel(m_model);
}

//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void Player::Exit()
{
}