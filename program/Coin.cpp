#include "Coin.h"
#include "Main.h"
#include "Game.h"

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void Coin::Init(int model, Float3 pos) {
	m_pos = pos;      // 座標の設定
	m_pos.y = 0.0f;   // 水面に固定

	m_radius = 1.0f;  // 衝突判定用の半径
	m_isActive = true; // 生存フラグをオン
}

//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void Coin::Update(int way, float movX, float movY) {

	// 非アクティブな場合は処理をスキップ
	if (!m_isActive) return;

	float waterSpeed = 0.08f; // 水流の移動速度

	// 流水の向き（way）に応じてコインを移動
	switch (way)
	{
	case 0: // UP
		m_pos.z += waterSpeed;
		break;

	case 1: // DOWN
		m_pos.z -= waterSpeed;
		break;

	case 2: // LEFT
		m_pos.x -= waterSpeed;
		break;

	case 3: // RIGHT
		m_pos.x += waterSpeed;
		break;

	default:
		break;
	}

	// ステージの有効範囲外（画面外）に出た場合は非アクティブ化
	if (m_pos.z < -21.0f || m_pos.z > 39.0f || m_pos.x < -53.0f || m_pos.x > 53.0f) {
		m_isActive = false;
	}

	// 波の起伏によるY座標（高さ）の動的変動計算
	float timeSec = GetNowCount() * 0.001f;
	float waveHeight = 0.2f;
	float waveSpeed = 2.5f;

	m_pos.y = sinf((m_pos.x) * 0.1f + timeSec * waveSpeed + movX * 10.0f) * waveHeight
		+ cosf((m_pos.z) * 0.1f + timeSec * waveSpeed * 1.2f + movY * 10.0f) * waveHeight;
}

//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void Coin::Render() {

	if (!m_isActive) return;

	// 3D球体としてコインを描画
	DrawSphere3D(
		VGet(m_pos.x, m_pos.y, m_pos.z),
		m_radius, 8,
		GetColor(255, 215, 0),    // ゴールド
		GetColor(255, 255, 255),  // 白（ワイヤーフレーム色）
		TRUE                      // 塗りつぶしを有効化
	);
}

//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void Coin::Exit() {

}