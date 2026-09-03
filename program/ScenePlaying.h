#pragma once
#include "SceneBase.h"
#include "../program/Camera.h"
#include "../program/Base.h"
#include <vector>

// ゲームプレイ中のメインシーン
class ScenePlaying : public SceneBase
{
private:

	float m_gameTimer = 45.0f; // 制限時間
	int   m_score = 0;         // 現在のスコア

	// 流水の向きを定義する列挙型
	enum { UP, DOWN, LEFT, RIGHT };
	int   m_waterWay = DOWN;   // 現在の流水の向き

	int   m_waterTexture = -1;  // 水面のテクスチャハンドル
	float m_waterMovX = 0.0f;    // 水面の移動速度（X軸）
	float m_waterMovY = 0.0f;    // 水面の移動速度（Y軸）
	float m_waterOffsetX = 0.0f; // 水面のUVスクロールオフセット（X軸）
	float m_waterOffsetY = 0.0f; // 水面のUVスクロールオフセット（Y軸）
	float m_spawnTimer = 0.0f;   // アイテムの生成タイマー
	int   m_spikesImg = -1;     // 障害物（トゲ）の画像ハンドル1
	int   m_spikesImg2 = -1;    // 障害物（トゲ）の画像ハンドル2
	float m_waterChangeTimer = 0.0f; // 流水の向きが変わるまでのタイマー
	bool  m_isGameOver = false;  // ゲームオーバーフラグ

	int   m_playerModel = -1;    // プレイヤーの3Dモデルハンドル
	Base* m_player = nullptr;    // プレイヤーオブジェクトのポインタ
	std::vector<Base*> m_coins;  // ステージ上のコインオブジェクトの配列
	Camera m_camera;             // 3Dカメラ

	// コイン（アイテム）の生成処理
	void SpawnCoin();

	// 当たり判定のチェック処理
	void CheckCollision();

	// 流水の更新処理（速度やタイマーの管理）
	void UpdateWater();

	// ゲーム制限時間の更新処理
	void UpdateGameTimer();

	// 水面の描画処理
	void DrawWater();

	// 3Dステージの描画処理
	void RenderStage3D();

	// プレイ画面の2D UI描画処理
	void RenderPlayingUI();

public:
	// コンストラクタ
	ScenePlaying() = default;

	// デストラクタ
	virtual ~ScenePlaying() override;

	// 初期化処理
	void Init() override;

	// 終了処理
	void Exit() override;

	// 更新処理
	GameState Update() override;

	// 描画処理
	void Render() override;

	// スコアのゲッター（外部取得用）
	int GetScore() const { return m_score; }
};