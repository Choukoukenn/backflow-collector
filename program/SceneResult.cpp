#include "SceneResult.h"
#include "../program/Main.h"
#include "DxLib.h"
#include "../program/Game.h"
#include <cstdio>
#include <cstring>

// コンストラクタ
SceneResult::SceneResult(GameState state, int finalScore)
	: m_myState(state), m_finalScore(finalScore)
{
}

// 初期化処理
void SceneResult::Init() {}

// 終了処理
void SceneResult::Exit() {}

// 更新処理
GameState SceneResult::Update()
{
	// ENTERキーが押されたらタイトル画面へ遷移
	if (PushHitKey(KEY_INPUT_RETURN))
	{
		return GameState::Title;
	}

	// キーが押されていない場合は現在の状態を維持
	return m_myState;
}

// 描画処理
void SceneResult::Render()
{
	int screenWidth = SCREEN_W;
	int screenHeight = SCREEN_H;
	float centerX = screenWidth / 2.0f;
	float centerY = screenHeight / 2.0f;

	// リザルトUIの描画を実行
	RenderResultUI(screenWidth, screenHeight, centerX, centerY);
}

// リザルトUIの描画処理（内部関数）
void SceneResult::RenderResultUI(int screenWidth, int screenHeight, float centerX, float centerY)
{
	// 背景を黒で塗りつぶす
	DrawBox(0, 0, screenWidth, screenHeight, GetColor(0, 0, 0), TRUE);

	// ゲームクリア時の表示
	if (m_myState == GameState::Clear)
	{
		const char* titleText = " ゲームクリア！ ";
		char scoreText[64];
		sprintf_s(scoreText, "最終スコア: %d", m_finalScore);
		const char* hintText = "[ ENTER ] キーを押してもう一度プレイ";

		// 各種テキストを中央揃えで描画
		DrawString((int)(centerX - (((float)GetDrawStringWidth(titleText, (int)strlen(titleText))) / 2.0f)), (int)(centerY - 40), titleText, GetColor(0, 255, 0));
		DrawString((int)(centerX - (((float)GetDrawStringWidth(scoreText, (int)strlen(scoreText))) / 2.0f)), (int)centerY, scoreText, GetColor(255, 255, 255));
		DrawString((int)(centerX - (((float)GetDrawStringWidth(hintText, (int)strlen(hintText))) / 2.0f)), (int)(centerY + 50), hintText, GetColor(200, 200, 200));
	}
	// ゲームオーバー時の表示
	else if (m_myState == GameState::GameOver)
	{
		const char* titleText = " GAME OVER！ ";
		const char* hintText = "[ ENTER ] キーを押してリトライ";

		// 各種テキストを中央揃えで描画
		DrawString((int)(centerX - (((float)GetDrawStringWidth(titleText, (int)strlen(titleText))) / 2.0f)), (int)(centerY - 20), titleText, GetColor(255, 0, 0));
		DrawString((int)(centerX - (((float)GetDrawStringWidth(hintText, (int)strlen(hintText))) / 2.0f)), (int)(centerY + 30), hintText, GetColor(200, 200, 200));
	}
}