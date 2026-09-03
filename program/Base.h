#pragma once

#include "Float3.h"

//---------------------------------------------------------------------------------
//	基本クラス
//---------------------------------------------------------------------------------
class Base
{
public:

	int		m_model;	// モデルの識別番号（ハンドル）

	Float3	m_rot;		// オブジェクトの回転角度（向き）

	// コンストラクタ
	Base();

	// デストラクタ
	virtual ~Base() {}

	// 各種仮想関数（初期化・更新・描画・終了処理）
	virtual void Init(int model, Float3 pos) = 0;
	virtual void Update(int way, float movX, float movY) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;

	// HPの取得（デフォルトは0、必要に応じて派生クラスでオーバーライド）
	virtual int GetHP() const { return 0; }

	// 生存フラグの確認
	virtual bool IsActive() const { return true; }

	// オブジェクトの消滅処理（死亡フラグを立てるなど）
	virtual void Kill() {}

	// 当たり判定用の半径を取得
	virtual float GetRadius() const { return 0.0f; }

	// 座標のゲッター
	Float3 GetPos() const { return m_pos; }

protected:
	Float3	m_pos;		// オブジェクトの座標位置（X, Y, Z）

};
