#include "SceneResult.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "GameMain.h"
#include "GameUtils.h"
#include "ScenePlay.h"

// 定数の定義 ==============================================================

#define SCENE_TIME (60 * 10)

#define FONT_SCORE_SIZE 80
#define FONT_MAIN_SIZE 22

// グローバル変数の定義 ====================================================

static int g_count;

static HFNT g_font_score;
static HFNT g_font_main;

static HGRP g_texture_result;

static HSND g_se_clear;

// 関数の定義 ==============================================================

// リザルトシーンの初期化処理
void InitializeResult(void)
{
	g_count = 0;

	// フォント
	g_font_score = CreateFontToHandle("Segoe UI Black", FONT_SCORE_SIZE, DX_FONTTYPE_ANTIALIASING_4X4);
	g_font_main = CreateFontToHandle("Segoe UI Black", FONT_MAIN_SIZE, DX_FONTTYPE_ANTIALIASING_4X4);

	// テクスチャ
	g_texture_result = LoadGraph("Resources\\Textures\\Result\\result.png");

	// 効果音
	g_se_clear = LoadSoundMem("Resources\\Audio\\se_clear.ogg");

	PlaySoundMem(g_se_clear, DX_PLAYTYPE_BACK);
}

// リザルトシーンの更新処理
void UpdateResult(void)
{
	if (g_count++ >= SCENE_TIME || IsKeyDown(PAD_INPUT_3))
		RequestScene(SCENE_TITLE);
}

// リザルトシーンの描画処理
void RenderResult(void)
{
	int bright = (int)GetEasingValue(ESG_INCUBIC, GetPercentageRange((float)g_count, 60 * 0, 60 * 0.5f), 255);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, bright);
	{
		DrawRotaGraph(SCREEN_CENTER_X, SCREEN_CENTER_Y, 1, 0, g_texture_result, TRUE);

		{
			int width = GetDrawFormatStringWidthToHandle(g_font_score, "%d", g_score);
			SetDrawBright(bright, bright, bright);
			DrawFormatStringToHandle(SCREEN_CENTER_X - width / 2 + 30, SCREEN_CENTER_Y - 40, COLOR_WHITE, g_font_score, "%d", g_score);
			SetDrawBright(255, 255, 255);
		}

		DrawFormatStringToHandle(SCREEN_CENTER_X - 58, SCREEN_CENTER_Y + 83, COLOR_BLACK, g_font_main, "%d", (SCENE_TIME - g_count) / 60 + 1);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// リザルトシーンの終了処理
void FinalizeResult(void)
{
	DeleteGraph(g_texture_result);

	DeleteSoundMem(g_se_clear);
}