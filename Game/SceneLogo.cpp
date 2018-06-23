#include "SceneLogo.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "GameMain.h"
#include "GameUtils.h"

// 定数の定義 ==============================================================

// ロゴムービー
#define MOVIE_LOGO "Resources\\Movies\\ydeagames.avi"

// グローバル変数の定義 ====================================================

int g_logo_count;
HGRP g_movie_logo;

// 関数の定義 ==============================================================

// ロゴシーンの初期化処理
void InitializeLogo(void)
{
	g_logo_count = 0;

	// 動画を読み込む
	g_movie_logo = LoadGraph(MOVIE_LOGO);

	// ロゴムービー再生
	PlayMovieToGraph(g_movie_logo);
}

// ロゴシーンの更新処理
void UpdateLogo(void)
{
	g_logo_count++;

	if (IsKeyPressed(PAD_INPUT_2) || g_logo_count >= 60 * 3.5f)
		RequestScene(SCENE_TITLE);
}

// ロゴシーンの描画処理
void RenderLogo(void)
{
	// ロゴムービー描画
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,
			(int)GetMinF(
				GetEasingValue(ESG_INCUBIC, GetPercentageRange((float)g_logo_count, 60 * 0, 60 * 0.5f), 255),		// フェードイン
				GetEasingValue(ESG_OUTCUBIC, 1 - GetPercentageRange((float)g_logo_count, 60 * 3, 60 * 3.5f), 255)	// フェードアウト
			)
		);
		DrawExtendGraph(SCREEN_LEFT, SCREEN_TOP, SCREEN_WIDTH, SCREEN_HEIGHT, g_movie_logo, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	//DrawFormatString(10, 10, COLOR_WHITE, "ロゴシーン");
	//DrawFormatString(10, 25, COLOR_WHITE, "カウント: %3d / 180", g_logo_count);
}

// ロゴシーンの終了処理
void FinalizeLogo(void)
{
	// ロゴムービーアンロード
	DeleteGraph(g_movie_logo);
}