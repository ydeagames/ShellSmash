#include "ScenePlay.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "GameObject.h"

// グローバル変数の定義 ====================================================

int g_play_count;
GameObject g_field;
GameObject g_shell;
GameObject g_paddle;

// 関数の定義 ==============================================================

// プレイシーンの初期化処理
void InitializePlay(void)
{
	g_play_count = 0;

	g_field = GameObject_Field_Create();
	g_shell = GameObject_Shell_Create();
	g_shell.pos = g_field.pos;
	g_shell.size.y = 64;
	g_paddle = GameObject_Paddle_Create();
	g_paddle.shape = SHAPE_OVAL;
	g_paddle.size.x = 64;
	g_paddle.size.y = 64;
	g_paddle.sprite.color = COLOR_GREEN;
}

// プレイシーンの更新処理
void UpdatePlay(void)
{
	//if (g_play_count++ >= 180)
	if (IsKeyPressed(PAD_INPUT_2))
		RequestScene(SCENE_RESULT);

	{
		int mouse_x, mouse_y;
		GetMousePoint(&mouse_x, &mouse_y);

		g_paddle.pos.x = (float)mouse_x;
		g_paddle.pos.y = (float)mouse_y;
	}
}

// プレイシーンの描画処理
void RenderPlay(void)
{
	DrawFormatString(10, 10, COLOR_WHITE, "プレイシーン");
	//DrawFormatString(10, 25, COLOR_WHITE, "カウント: %3d / 180", g_play_count);
	DrawFormatString(10, 25, COLOR_WHITE, "PRESS X TO END");
	DrawFormatString(10, 40, COLOR_WHITE, "HIT: %s", GameObject_IsHit(&g_paddle, &g_shell) ? "TRUE" : "FALSE");

	GameObject_Render(&g_shell);
	GameObject_Render(&g_paddle);
}

// プレイシーンの終了処理
void FinalizePlay(void)
{

}