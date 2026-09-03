#pragma once

// ゲームの状態（シーン）を管理する列挙型
enum class GameState
{
	Title,     // タイトル
	Playing,   // プレイ中
	Clear,     // ゲームクリア
	GameOver   // ゲームオーバー
};

// 全てのシーンの基底クラス
class SceneBase
{
public:

	// デストラクタ
	virtual ~SceneBase() = default;

	// 初期化処理
	virtual void Init() = 0;

	// 終了処理
	virtual void Exit() = 0;

	// 描画処理
	virtual void Render() = 0;

	// 更新処理（次のゲーム状態を返す）
	virtual GameState Update() = 0;
};