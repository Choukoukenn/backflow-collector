#pragma once
#include "Base.h"

//---------------------------------------------------------------------------------
//	リングクラス（障害物・収集アイテム）
//---------------------------------------------------------------------------------
class Coin : public Base
{
public:

	// リングの初期化処理（座標、回転、半径、種類の指定）
	void Init(int model, Float3 pos) override;

	// 更新処理
	void Update(int way, float movX, float movY) override;

	// 描画処理
	void Render() override;

	// 終了処理
	void Exit() override;

	// 生存フラグの確認
	bool IsActive() const override { return m_isActive; }

	// 消滅処理（フラグをオフにする）
	void Kill() override { m_isActive = false; }

	// 当たり判定用の半径を取得
	float GetRadius() const override { return m_radius; }

private:
	float    m_radius = 5.0f;   // リングの当たり判定半径
	bool     m_isActive = true; // 生存フラグ
};