#include "ScenePlay.h"
#include "SceneManager.h"
#include "InputManager.h"

// グローバル変数の定義 ====================================================

int g_play_count;

// 関数の定義 ==============================================================

// プレイシーンの初期化処理
void InitializePlay(void)
{
	g_play_count = 0;
}

// プレイシーンの更新処理
void UpdatePlay(void)
{
	//if (g_play_count++ >= 180)
	if (IsKeyDown(PAD_INPUT_2))
		RequestScene(SCENE_RESULT);
}

// プレイシーンの描画処理
void RenderPlay(void)
{
	DrawFormatString(10, 10, COLOR_WHITE, "プレイシーン");
	//DrawFormatString(10, 25, COLOR_WHITE, "カウント: %3d / 180", g_play_count);
	DrawFormatString(10, 25, COLOR_WHITE, "PRESS X TO END");
}

// プレイシーンの終了処理
void FinalizePlay(void)
{

}