#pragma once

#include "Float3.h"

//---------------------------------------------------------------------------------
//	カメラクラス
//---------------------------------------------------------------------------------
class Camera
{
public:

	void Init();		//	初期化処理
	void Update(const Float3& player_pos, const Float3& player_rot);
	void Setting();		//	描画前のカメラ設定
	void Exit();		//	終了処理



private:
	//	カメラ座標
	Float3		m_pos;
	//	カメラの向き
	Float3		m_rot;

	float m_fov = 60.0f;        
	float m_shakeIntensity = 0.0f; 

};
