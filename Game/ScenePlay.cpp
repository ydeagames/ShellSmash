#include "ScenePlay.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "GameObject.h"
#include "GameUtils.h"

// 定数の定義 ==============================================================

#define NUM_SHELLS 10

// グローバル変数の定義 ====================================================

int g_play_count;
GameObject g_field;
GameObject g_shells[NUM_SHELLS];
GameObject g_paddle;

// 関数の定義 ==============================================================

// プレイシーンの初期化処理
void InitializePlay(void)
{
	g_play_count = 0;

	g_field = GameObject_Field_Create();

	for (int i = 0; i < NUM_SHELLS; i++)
	{
		g_shells[i] = GameObject_Shell_Create();
		g_shells[i].pos = Vec2_Create(GetRandRangeF(GameObject_GetX(&g_field, LEFT), GameObject_GetX(&g_field, RIGHT)), GetRandRangeF(GameObject_GetY(&g_field, TOP), GameObject_GetY(&g_field, BOTTOM)));
	}

	g_paddle = GameObject_Paddle_Create();
	g_paddle.sprite.color = COLOR_GREEN;
}

#define _USE_MATH_DEFINES
#include <math.h>

// プレイシーンの更新処理
void UpdatePlay(void)
{
	//if (g_play_count++ >= 180)
	if (IsKeyPressed(PAD_INPUT_2))
		RequestScene(SCENE_RESULT);

	{
		int mouse_x, mouse_y;
		GetMousePoint(&mouse_x, &mouse_y);

		g_paddle.vel.x = mouse_x - g_paddle.pos.x;
		g_paddle.vel.y = mouse_y - g_paddle.pos.y;
	}

	{
		g_paddle.pos.x += g_paddle.vel.x;
		g_paddle.pos.y += g_paddle.vel.y;

		for (int i = 0; i < NUM_SHELLS; i++)
		{
			g_shells[i].pos.x += g_shells[i].vel.x;
			g_shells[i].pos.y += g_shells[i].vel.y;
			g_shells[i].vel.x *= .98f;
			g_shells[i].vel.y *= .98f;
		}
	}

	for (int i = 0; i < NUM_SHELLS; i++)
	{
		if (GameObject_IsHit(&g_paddle, &g_shells[i]))
		{
			Vec2 last_vel = g_shells[i].vel;
			while (GameObject_IsHit(&g_paddle, &g_shells[i]))
			{
				Vec2 relative_vel = Vec2_Create(g_paddle.vel.x - last_vel.x, g_paddle.vel.y - last_vel.y);
				g_shells[i].vel = relative_vel;
				if (Vec2_IsZero(&relative_vel))
					break;
				g_shells[i].pos.x += relative_vel.x;
				g_shells[i].pos.y += relative_vel.y;
			}
		}
	}

	for (int iy = 0; iy < NUM_SHELLS; iy++)
	{
		for (int ix = 0; ix < iy; ix++)
		{
			GameObject* shell_a = &g_shells[ix];
			GameObject* shell_b = &g_shells[iy];
			if (GameObject_IsHit(shell_a, shell_b))
			{
				// 衝突前のオブジェクトAの速度ベクトル
				Vec2 vel_before_a = shell_a->vel;
				// 衝突前のオブジェクトBの速度ベクトル
				Vec2 vel_before_b = shell_b->vel;

				// 進むべき方向ベクトル
				Vec2 forward = Vec2_Create(shell_b->pos.x - shell_a->pos.x, shell_b->pos.y - shell_a->pos.y);
				Vec2_Render(&forward, &shell_b->pos, COLOR_YELLOW);

				Vec2 vel_vertical_a;
				Vec2 vel_horizontal_a;
				Vec2_Decompose(&vel_before_a, &forward, &vel_vertical_a, &vel_horizontal_a);
				Vec2_Render(&vel_vertical_a, &shell_a->pos, COLOR_GREEN);
				Vec2_Render(&vel_horizontal_a, &shell_a->pos, COLOR_LIME);
				Vec2 vel_vertical_b;
				Vec2 vel_horizontal_b;
				Vec2_Decompose(&vel_before_b, &forward, &vel_vertical_b, &vel_horizontal_b);
				Vec2_Render(&vel_vertical_b, &shell_b->pos, COLOR_GREEN);
				Vec2_Render(&vel_horizontal_b, &shell_b->pos, COLOR_LIME);

				// 衝突後のオブジェクトAのベクトル合成
				Vec2_Add(&shell_a->vel, &vel_vertical_b, &vel_horizontal_a);
				Vec2_Render(&shell_a->vel, &shell_a->pos, COLOR_BLUE);
				// 衝突後のオブジェクトBのベクトル合成
				Vec2_Add(&shell_b->vel, &vel_vertical_a, &vel_horizontal_b);
				Vec2_Render(&shell_b->vel, &shell_b->pos, COLOR_FUCHSIA);
			}
		}
	}

	for (int i = 0; i < NUM_SHELLS; i++)
	{
		if (GameObject_Field_CollisionHorizontal(&g_field, &g_shells[i], CONNECTION_BARRIER, EDGESIDE_INNER))
		{
			g_shells[i].vel.x *= -.98f;
		}
		if (GameObject_Field_CollisionVertical(&g_field, &g_shells[i], CONNECTION_BARRIER, EDGESIDE_INNER))
		{
			g_shells[i].vel.y *= -.98f;
		}
	}
}

// プレイシーンの描画処理
void RenderPlay(void)
{
	DrawFormatString(10, 10, COLOR_WHITE, "プレイシーン");
	//DrawFormatString(10, 25, COLOR_WHITE, "カウント: %3d / 180", g_play_count);
	DrawFormatString(10, 25, COLOR_WHITE, "PRESS X TO END");

	for (int i = 0; i < NUM_SHELLS; i++)
	{
		GameObject_Render(&g_shells[i]);
	}

	GameObject_Render(&g_paddle);

	/*
	{
		Vec2 base = Vec2_Create(320, 240);
		Vec2 vec = Vec2_Create(100, 100);
		Vec2 angle = Vec2_Create(120, -80);
		Vec2 vec_a;
		Vec2 vec_b;

		Vec2_Decompose(&vec, &angle, &vec_a, &vec_b);

		Vec2_Render(&vec, &base, COLOR_WHITE);
		Vec2_Render(&angle, &base, COLOR_GRAY);
		Vec2_Render(&vec_a, &base, COLOR_RED);
		Vec2_Render(&vec_b, &base, COLOR_BLUE);
	}
	/**/
}

// プレイシーンの終了処理
void FinalizePlay(void)
{

}