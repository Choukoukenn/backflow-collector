#pragma once

#include "Base.h"

// プレイヤーの当たり判定の半幅（サイズ調整用）
constexpr float PLAYER_HALF = 0.5f;

// プレイヤークラス
class Player : public Base
{
public:

	// モデルの回転および姿勢制御用の行列
	MATRIX	m_mat = MGetIdent();

	// 初期化処理
	void Init(int model, Float3 pos) override;

	// 更新処理
	void Update(int way, float movX, float movY) override;

	// 描画処理
	void Render() override;

	// 終了処理
	void Exit() override;

	// 前フレームの座標を取得
	const Float3& GetPrevPosition() const { return m_prevPos; }

	// HPのゲッター
	int GetHP() const override { return m_hp; }

private:
	Float3 m_prevPos = Float3(0, 0, 0);	// 前フレームの座標保持用
	float m_moveSpeed = 1.5f;   		// 現在の移動速度

	float m_kbVelX = 0.0f; // ノックバック速度（X軸）
	float m_kbVelZ = 0.0f; // ノックバック速度（Z軸）

	int m_hp = 10; // プレイヤーのHP

	int m_startFrame = 0; // 開始フレームのカウント用（無敵・安全対策用）
};