#include "ScenePlaying.h"
#include "../program/Game.h"
#include "DxLib.h"
#include "../program/Player.h"
#include "../program/Coin.h"
#include "../program/Random.h"
#include <algorithm>

// デストラクタ
ScenePlaying::~ScenePlaying() {
	Exit();
}

// 初期化処理
void ScenePlaying::Init() {
	m_isGameOver = false;

	// リソースの読み込み
	m_playerModel = MV1LoadModel("data/player.mv1");
	m_waterTexture = LoadGraph("data/water.png");
	m_spikesImg = LoadGraph("data/spikes.png");
	m_spikesImg2 = LoadGraph("data/spikes2.png");

	// カメラの初期化
	m_camera.Init();

	// プレイヤーの生成と初期化
	m_player = new Player();
	m_player->Init(m_playerModel, Float3(0.0f, 0.0f, 0.0f));

	// ゲームパラメータのリセット
	m_gameTimer = 45.0f;
	m_score = 0;
	m_waterWay = 1; // DOWN
	m_waterChangeTimer = 0.0f;
	m_spawnTimer = 0.0f;
	m_coins.clear();
}

// 更新処理
GameState ScenePlaying::Update() {
	// デバッグ用または強制的な流水方向の切り替えキー入力
	//if (CheckHitKey(KEY_INPUT_1)) { m_waterWay = UP; }
	//else if (CheckHitKey(KEY_INPUT_2)) { m_waterWay = DOWN; }
	//else if (CheckHitKey(KEY_INPUT_3)) { m_waterWay = LEFT; }
	//else if (CheckHitKey(KEY_INPUT_4)) { m_waterWay = RIGHT; }

	// 各種タイマーや流水状態の更新
	UpdateGameTimer();
	UpdateWater();

	// プレイヤーの更新
	if (m_player != nullptr) { m_player->Update(m_waterWay, m_waterMovX, m_waterMovY); }

	// コインの定期生成
	m_spawnTimer += 1.0f / 60.0f;
	if (m_spawnTimer >= 1.5f)
	{
		SpawnCoin();
		m_spawnTimer = 0.0f;
	}

	// 全てのコインの更新
	for (auto& coin : m_coins) { if (coin != nullptr) coin->Update(m_waterWay, m_waterMovX, m_waterMovY); }

	// 当たり判定のチェック
	CheckCollision();

	// カメラの注視点更新（プレイヤーに追従）
	if (m_player != nullptr) { m_camera.Update(m_player->GetPos(), m_player->m_rot); }

	// 非アクティブになったコインの削除（メモリ解放含む）
	m_coins.erase(
		std::remove_if(m_coins.begin(), m_coins.end(),
			[](Base* c) {
				if (c == nullptr) return true;
				if (!c->IsActive()) { c->Exit(); delete c; return true; }
				return false;
			}),
		m_coins.end()
	);

	// ゲームオーバー判定
	if (m_player != nullptr && m_player->GetHP() <= 0) { m_isGameOver = true; }
	if (m_isGameOver)
	{
		return GameState::GameOver;
	}

	// タイムアップによるクリア判定
	if (m_gameTimer <= 0.0f)
	{
		return GameState::Clear;
	}

	return GameState::Playing;
}

// 描画処理
void ScenePlaying::Render() {
	RenderStage3D();  // 3D空間の描画
	RenderPlayingUI(); // 2D UIの描画
}

// 終了（解放）処理
void ScenePlaying::Exit() {
	// プレイヤーの解放
	if (m_player != nullptr) { m_player->Exit(); delete m_player; m_player = nullptr; }

	// コイン配列の解放
	for (auto* coin : m_coins) { if (coin != nullptr) { coin->Exit(); delete coin; } }
	m_coins.clear();

	// グラフィック・モデルリソースの削除
	MV1DeleteModel(m_playerModel);
	DeleteGraph(m_waterTexture);
	DeleteGraph(m_spikesImg);
	DeleteGraph(m_spikesImg2);
}

// コインの生成処理
void ScenePlaying::SpawnCoin() {
	Float3 spawnPos(0.0f, 0.0f, 0.0f);

	// ステージの境界座標定義
	const float TOP_Z = 34.0f;
	const float BOTTOM_Z = -16.0f;
	const float LEFT_X_TOP = -48.0f;
	const float RIGHT_X_TOP = 48.0f;
	const float LEFT_X_BOTTOM = -10.0f;
	const float RIGHT_X_BOTTOM = 10.0f;

	// 流水の向きに応じて生成位置（上流側）を決定
	switch (m_waterWay)
	{
	case DOWN:
		spawnPos.z = TOP_Z;
		spawnPos.x = GetRandomF(LEFT_X_TOP, RIGHT_X_TOP);
		break;

	case UP:
		spawnPos.z = BOTTOM_Z;
		spawnPos.x = GetRandomF(LEFT_X_BOTTOM, RIGHT_X_BOTTOM);
		break;

	case RIGHT:
		spawnPos.z = GetRandomF(BOTTOM_Z, TOP_Z);
		{
			float t = (spawnPos.z - BOTTOM_Z) / (TOP_Z - BOTTOM_Z);
			spawnPos.x = LEFT_X_BOTTOM + t * (LEFT_X_TOP - LEFT_X_BOTTOM);
		}
		break;

	case LEFT:
		spawnPos.z = GetRandomF(BOTTOM_Z, TOP_Z);
		{
			float t = (spawnPos.z - BOTTOM_Z) / (TOP_Z - BOTTOM_Z);
			spawnPos.x = RIGHT_X_BOTTOM + t * (RIGHT_X_TOP - RIGHT_X_BOTTOM);
		}
		break;
	}

	spawnPos.y = 0.0f;

	// コインインスタンスの生成と登録
	Base* newCoin = new Coin();
	newCoin->Init(0, Float3(spawnPos));
	m_coins.push_back(newCoin);
}

// 当たり判定処理
void ScenePlaying::CheckCollision()
{
	Float3 playerPos = Float3(0.0f, 0.0f, 0.0f);

	if (m_player != nullptr)
	{
		playerPos = m_player->GetPos();
	}

	float playerRadius = PLAYER_HALF;

	// プレイヤーと全コインの球（円）同士の当たり判定
	for (auto& coin : m_coins)
	{
		if (coin == nullptr) continue;
		if (!coin->IsActive()) continue;

		float dx = playerPos.x - coin->GetPos().x;
		float dz = playerPos.z - coin->GetPos().z;
		float distance = sqrtf(dx * dx + dz * dz);

		// 接触しているか判定
		if (distance < (playerRadius + coin->GetRadius()))
		{
			coin->Kill();   // コインを非アクティブ化
			m_score += 100; // スコア加算
		}
	}
}

// 水流（方向とタイマー）の更新処理
void ScenePlaying::UpdateWater()
{
	const float CHANGE_INTERVAL = 7.0f; // 切り替え間隔（秒）

	m_waterChangeTimer += 1.0f / 60.0f;

	// 一定時間ごとに流水の向きをランダムに変更
	if (m_waterChangeTimer >= CHANGE_INTERVAL)
	{
		m_waterChangeTimer = 0.0f;
		int old_way = m_waterWay;

		// 前回と同じ向きにならないようにループ
		do {
			m_waterWay = GetRand(3);
		} while (m_waterWay == old_way);
	}
}

// ゲーム制限時間の更新処理
void ScenePlaying::UpdateGameTimer()
{
	if (m_isGameOver) return;

	m_gameTimer -= 1.0f / 60.0f;

	if (m_gameTimer <= 0.0f)
	{
		m_gameTimer = 0.0f;
	}
}

// 水面の描画処理（頂点ノイズによる波表現とUVスクロール）
void ScenePlaying::DrawWater()
{
	// 水流の方向に応じてUVオフセット速度を設定
	switch (m_waterWay)
	{
	case UP:    m_waterOffsetX = 0.0f;  m_waterOffsetY = -0.01f; break;
	case DOWN:  m_waterOffsetX = 0.0f;  m_waterOffsetY = 0.01f;  break;
	case LEFT:  m_waterOffsetX = 0.01f; m_waterOffsetY = 0.0f;   break;
	case RIGHT: m_waterOffsetX = -0.01f; m_waterOffsetY = 0.0f;  break;
	}

	m_waterMovX += m_waterOffsetX;
	m_waterMovY += m_waterOffsetY;

	// レイヤー1のUVスクロール計算
	float scrollU1 = fmodf(0.0f + m_waterMovX, 1.0f);
	float scrollV1 = fmodf(0.0f + m_waterMovY, 1.0f);

	// レイヤー2（ブレンド用）のUVスクロール計算
	float timeSec = GetNowCount() * 0.001f;
	float scrollU2 = fmodf(m_waterMovX * 0.5f + sinf(timeSec) * 0.05f, 1.0f);
	float scrollV2 = fmodf(m_waterMovY * 0.5f + timeSec * 0.1f, 1.0f);

	// 3D水面グリッドの定義
	const int DIV_X = 10;
	const int DIV_Z = 10;
	const float totalSize = 110.0f;
	const float startX = -55.0f;
	const float startZ = -55.0f;
	const float stepX = totalSize / DIV_X;
	const float stepZ = totalSize / DIV_Z;

	const float tiling = 2.0f;
	const float waveHeight = 0.2f;
	const float waveSpeed = 2.5f;

	// 3D描画ステートの設定
	SetUseLighting(FALSE);
	SetUseBackCulling(FALSE);
	SetTextureAddressMode(DX_TEXADDRESS_WRAP);
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(FALSE); // 水面は半透明のためZバッファ書き込みをオフ

	// ポリゴングリッドの生成と描画
	for (int z = 0; z < DIV_Z; z++)
	{
		for (int x = 0; x < DIV_X; x++)
		{
			float x0 = startX + x * stepX;
			float x1 = startX + (x + 1) * stepX;
			float z0 = startZ + z * stepZ;
			float z1 = startZ + (z + 1) * stepZ;

			float u0 = (float)x / DIV_X * tiling;
			float u1 = (float)(x + 1) / DIV_X * tiling;
			float v0 = (float)z / DIV_Z * tiling;
			float v1 = (float)(z + 1) / DIV_Z * tiling;

			VERTEX3D v[6] = {};

			// 三角形2枚分の頂点座標設定
			v[0].pos = VGet(x0, 0.0f, z0);
			v[1].pos = VGet(x1, 0.0f, z0);
			v[2].pos = VGet(x0, 0.0f, z1);

			v[3].pos = VGet(x1, 0.0f, z0);
			v[4].pos = VGet(x1, 0.0f, z1);
			v[5].pos = VGet(x0, 0.0f, z1);

			// サイン波による頂点Y座標（高さ）の動的変形処理
			for (int i = 0; i < 6; i++)
			{
				v[i].pos.y = sinf(v[i].pos.x * 0.1f + timeSec * waveSpeed) * waveHeight
					+ cosf(v[i].pos.z * 0.1f + timeSec * waveSpeed * 1.2f) * waveHeight;

				v[i].dif = GetColorU8(255, 255, 255, 255);
				v[i].spc = GetColorU8(0, 0, 0, 0);
				v[i].norm = VGet(0.0f, 1.0f, 0.0f);
			}

			// パス1: ベース水面（アルファブレンド）
			v[0].u = u0 + scrollU1; v[0].v = v0 + scrollV1;
			v[1].u = u1 + scrollU1; v[1].v = v0 + scrollV1;
			v[2].u = u0 + scrollU1; v[2].v = v1 + scrollV1;
			v[3].u = u1 + scrollU1; v[3].v = v0 + scrollV1;
			v[4].u = u1 + scrollU1; v[4].v = v1 + scrollV1;
			v[5].u = u0 + scrollU1; v[5].v = v1 + scrollV1;

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 160);
			DrawPolygon3D(v, 2, m_waterTexture, TRUE);

			// パス2: 波のハイライト表現（加算ブレンド）
			v[0].u = u0 + scrollU2; v[0].v = v0 + scrollV2;
			v[1].u = u1 + scrollU2; v[1].v = v0 + scrollV2;
			v[2].u = u0 + scrollU2; v[2].v = v1 + scrollV2;
			v[3].u = u1 + scrollU2; v[3].v = v0 + scrollV2;
			v[4].u = u1 + scrollU2; v[4].v = v1 + scrollV2;
			v[5].u = u0 + scrollU2; v[5].v = v1 + scrollV2;

			for (int i = 0; i < 6; i++) {
				v[i].dif = GetColorU8(100, 150, 255, 255);
			}

			SetDrawBlendMode(DX_BLENDMODE_ADD, 90);
			DrawPolygon3D(v, 2, m_waterTexture, TRUE);
		}
	}

	// 描画ステートを元に戻す
	SetWriteZBuffer3D(TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetUseLighting(TRUE);
	SetUseBackCulling(TRUE);
}

// 3Dステージの描画一括処理
void ScenePlaying::RenderStage3D()
{
	m_camera.Setting(); // カメラ行列の適用
	if (m_player != nullptr) { m_player->Render(); } // プレイヤー描画
	for (auto& coin : m_coins) { if (coin != nullptr) coin->Render(); } // コイン描画
	DrawWater(); // 水面描画
}

// プレイ中の2D UI描画処理
void ScenePlaying::RenderPlayingUI()
{
	// 画面外周の障害物（トゲトゲ）の描画
	const int SPIKE_W = 68; const int SPIKE_H = 15;
	for (int x = 0; x < SCREEN_W; x += SPIKE_W) { DrawRectExtendGraph(x, SCREEN_H - SPIKE_H, x + SPIKE_W, SCREEN_H, 0, 0, 761, 167, m_spikesImg, TRUE); }
	for (int x = 0; x < SCREEN_W; x += SPIKE_W) { DrawRectExtendGraph(x, SPIKE_H, x + SPIKE_W, 0, 0, 0, 761, 167, m_spikesImg, TRUE); }
	for (int y = 0; y < SCREEN_H; y += SPIKE_W) { DrawRectExtendGraph(0, y, SPIKE_H, y + SPIKE_W, 0, 0, 167, 761, m_spikesImg2, TRUE); }
	for (int y = 0; y < SCREEN_H; y += SPIKE_W) { DrawRectExtendGraph(SCREEN_W, y, SCREEN_W - SPIKE_H, y + SPIKE_W, 0, 0, 167, 761, m_spikesImg2, TRUE); }

	// UI表示用パラメータの設定
	unsigned int whiteColor = GetColor(255, 255, 255);
	int uiX = 30; int uiY = 30; int lineGap = 30;
	int currentHP = 0;
	if (m_player != nullptr) { currentHP = m_player->GetHP(); }

	// ピンチ時のHP色変更（赤文字）
	unsigned int hpColor = whiteColor;
	if (currentHP <= 5) { hpColor = GetColor(255, 0, 0); }

	// UIの背景半透明黒ボックスを描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(uiX - 10, uiY - 10, uiX + 120, uiY + (lineGap * 2) + 25, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 各種ステータス（HP、残り時間、スコア）の描画
	DrawFormatString(uiX, uiY, hpColor, "HP    : %d", currentHP);
	int displaySeconds = (int)m_gameTimer;
	unsigned int timeColor = whiteColor;
	if (displaySeconds <= 10) { timeColor = GetColor(255, 0, 0); } // タイムリミット寸前で赤文字化
	DrawFormatString(uiX, uiY + lineGap, timeColor, "TIME : %d", displaySeconds);
	DrawFormatString(uiX, uiY + (lineGap * 2), whiteColor, "SCORE: %05d", m_score);
}