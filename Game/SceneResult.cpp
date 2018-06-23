#include "SceneResult.h"
#include "SceneManager.h"
#include "InputManager.h"

// グローバル変数の定義 ====================================================

int g_result_count;

// 関数の定義 ==============================================================

// リザルトシーンの初期化処理
void InitializeResult(void)
{
	g_result_count = 0;
}

// リザルトシーンの更新処理
void UpdateResult(void)
{
	//if (g_result_count++ >= 180)
	if (IsKeyDown(PAD_INPUT_3))
		RequestScene(SCENE_TITLE);
}

// リザルトシーンの描画処理
void RenderResult(void)
{
	DrawFormatString(10, 10, COLOR_WHITE, "リザルトシーン");
	//DrawFormatString(10, 25, COLOR_WHITE, "カウント: %3d / 180", g_result_count);
	DrawFormatString(10, 25, COLOR_WHITE, "PRESS C TO BACK TO TITLE");
}

// リザルトシーンの終了処理
void FinalizeResult(void)
{

}