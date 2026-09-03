#pragma once
#include "SceneBase.h"

// リザルト（結果）画面のシーン
class SceneResult : public SceneBase
{
public:

	// コンストラクタ（状態と最終スコアを受け取る）
	SceneResult(GameState state, int finalScore);

	// デストラクタ
	virtual ~SceneResult() override = default;

	// 初期化処理
	void Init() override;

	// 終了処理
	void Exit() override;

	// 更新処理
	GameState Update() override;

	// 描画処理
	void Render() override;

private:
	GameState m_myState;  // 現在のゲーム状態の保持用
	int m_finalScore;     // 最終スコアの保持用

	// リザルトUIの描画処理
	void RenderResultUI(int screenWidth, int screenHeight, float centerX, float centerY);
};