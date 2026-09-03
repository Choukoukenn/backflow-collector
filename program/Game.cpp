#include "Game.h"
#include <vector>
#include "SceneTitle.h"
#include "ScenePlaying.h"
#include "SceneResult.h"

// グローバル変数：現在のゲーム状態とシーンのポインタ
GameState g_gameState = GameState::Title;
SceneBase* g_currentScene = nullptr;

// ゲーム全体の初期化処理
void GameInit()
{
	// 既存のシーンがある場合はメモリを解放
	if (g_currentScene != nullptr)
	{
		delete g_currentScene;
	}

	// 初期状態をタイトル画面に設定
	g_gameState = GameState::Title;
	g_currentScene = new SceneTitle();
	g_currentScene->Init();
}

// ゲーム全体の更新処理（メインループから毎フレーム呼ばれる）
void GameUpdate()
{
	if (g_currentScene != nullptr)
	{
		// 現在のシーンの更新処理を実行し、次のシーン状態を受け取る
		GameState nextState = g_currentScene->Update();

		// シーン遷移が発生した場合の処理
		if (nextState != g_gameState)
		{
			int finalScore = 0;

			// プレイ中からリザルトへ遷移する場合、スコアを引き継ぐ
			if (g_gameState == GameState::Playing)
			{
				ScenePlaying* playingScene = dynamic_cast<ScenePlaying*>(g_currentScene);
				if (playingScene != nullptr)
				{
					finalScore = playingScene->GetScore();
				}
			}

			// 現在のシーンの終了処理とメモリ解放
			g_currentScene->Exit();
			delete g_currentScene;
			g_currentScene = nullptr;

			// ゲーム状態を更新
			g_gameState = nextState;

			// 新しいシーンのインスタンスを生成
			switch (g_gameState)
			{
			case GameState::Title:
				g_currentScene = new SceneTitle();
				break;

			case GameState::Playing:
				g_currentScene = new ScenePlaying();
				break;

			case GameState::Clear:
			case GameState::GameOver:
				// クリアまたはゲームオーバー時はスコアを渡してリザルトを生成
				g_currentScene = new SceneResult(g_gameState, finalScore);
				break;
			}

			// 新しいシーンの初期化処理を実行
			if (g_currentScene != nullptr)
			{
				g_currentScene->Init();
			}
		}
	}
}

// 描画処理
void GameRender()
{
	// 現在のシーンの描画処理を実行
	if (g_currentScene != nullptr)
	{
		g_currentScene->Render();
	}
}

// 終了処理
void GameExit()
{
	// アプリケーション終了時に現在のシーンを解放
	if (g_currentScene != nullptr)
	{
		g_currentScene->Exit();
		delete g_currentScene;
		g_currentScene = nullptr;
	}
}