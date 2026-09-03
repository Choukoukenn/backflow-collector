#pragma once
#include "SceneBase.h"

// タイトル画面のシーン
class SceneTitle : public SceneBase
{
public:
	// コンストラクタ
	SceneTitle() = default;

	// デストラクタ
	virtual ~SceneTitle() override = default;

	// 初期化処理
	void Init() override;

	// 終了処理
	void Exit() override;

	// 更新処理
	GameState Update() override;

	// 描画処理
	void Render() override;

private:
	// タイトルUIの描画処理
	void RenderTitleUI(int screenWidth, int screenHeight, float centerX, float centerY);
};