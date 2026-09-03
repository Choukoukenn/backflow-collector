#include "SceneTitle.h"
#include "../program/Main.h"
#include "DxLib.h"
#include "../program/Game.h" 
#include <cstring>

// 更新処理
GameState SceneTitle::Update()
{
	// ENTERキーが押されたらゲームプレイ画面へ遷移
	if (PushHitKey(KEY_INPUT_RETURN))
	{
		return GameState::Playing;
	}

	// キーが押されていない場合は現在の状態を維持
	return GameState::Title;
}

// 初期化処理
void SceneTitle::Init()
{

}

// 終了処理
void SceneTitle::Exit()
{

}

// 描画処理
void SceneTitle::Render()
{
	int screenWidth = SCREEN_W;
	int screenHeight = SCREEN_H;
	float centerX = screenWidth / 2.0f;
	float centerY = screenHeight / 2.0f;

	// タイトルUIの描画を実行
	RenderTitleUI(screenWidth, screenHeight, centerX, centerY);
}

// タイトルUIの描画処理（内部関数）
void SceneTitle::RenderTitleUI(int screenWidth, int screenHeight, float centerX, float centerY)
{
	// 背景を黒で塗りつぶす
	DrawBox(0, 0, screenWidth, screenHeight, GetColor(0, 0, 0), TRUE);

	// 表示するテキストの設定
	const char* titleText = "--- リバーラッシュ！逆流コレクター ---";
	const char* startText = "[ ENTER ] キーを押してスタート";
	const char* controlText = "【操作方法】 矢印キー [ ↑ / ↓ / ← / → ] 移動調整";
	const char* goalText = "【ルール】 流れてくるアイテムをたくさん集めて高得点を目指そう！";

	// 各種テキストを中央揃えで描画
	int titleX = (int)(centerX - ((float)GetDrawStringWidth(titleText, (int)strlen(titleText)) / 2.0f));
	DrawString(titleX, (int)(centerY - 60), titleText, GetColor(0, 255, 255));

	int startX = (int)(centerX - ((float)GetDrawStringWidth(startText, (int)strlen(startText)) / 2.0f));
	DrawString(startX, (int)centerY, startText, GetColor(255, 255, 0));

	int controlX = (int)(centerX - ((float)GetDrawStringWidth(controlText, (int)strlen(controlText)) / 2.0f));
	DrawString(controlX, (int)(centerY + 60), controlText, GetColor(180, 180, 180));

	int goalX = (int)(centerX - ((float)GetDrawStringWidth(goalText, (int)strlen(goalText)) / 2.0f));
	DrawString(goalX, (int)(centerY + 90), goalText, GetColor(180, 180, 180));
}