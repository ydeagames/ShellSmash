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
				// 衝突前のオブジェクトAの速さ
				float vel_before_a_length = Vec2_Length(&vel_before_a);
				// 衝突前のオブジェクトBの速さ
				float vel_before_b_length = Vec2_Length(&vel_before_b);
				
				float vel_percent_a = vel_before_a_length / (vel_before_a_length + vel_before_b_length);
				float vel_percent_b = vel_before_b_length / (vel_before_a_length + vel_before_b_length);

				// 進むべき方向ベクトル
				Vec2 forward = Vec2_Create(shell_b->pos.x - shell_a->pos.x, shell_b->pos.y - shell_a->pos.y);
				// 現在の速度方向ベクトル
				Vec2 current = Vec2_Create(shell_a->vel.x - shell_b->vel.x, shell_a->vel.y - shell_b->vel.y);
				// 進むべき方向の角度
				float forward_rota = atan2f(forward.y, forward.x);
				// 現在の速度方向の角度
				float current_rota = atan2f(current.y, current.x);
				// 現在の速さ
				float current_length = Vec2_Length(&current);
				// 方向の差
				float diff_rota = forward_rota - current_rota;
				// 衝突後のオブジェクトAの速さ
				float vel_after_a_length = sinf(diff_rota) * current_length;
				// 衝突後のオブジェクトBの速さ
				float vel_after_b_length = cosf(diff_rota) * current_length;
				// 衝突後のオブジェクトAの角度
				float vel_after_a_rota = forward_rota + (float)((sinf(diff_rota)<0) ? -M_PI_2 : M_PI_2);
				// 衝突後のオブジェクトBの角度
				float vel_after_b_rota = forward_rota;
				// 衝突後のオブジェクトAの速度ベクトル
				Vec2 vel_after_a = Vec2_Create(cosf(vel_after_a_rota)*vel_after_a_length, sinf(vel_after_a_rota)*vel_after_a_length);
				// 衝突後のオブジェクトBの速度ベクトル
				Vec2 vel_after_b = Vec2_Create(cosf(vel_after_b_rota)*vel_after_b_length, sinf(vel_after_b_rota)*vel_after_b_length);
				// 衝突後のオブジェクトAのベクトル合成
				shell_a->vel = Vec2_Create(vel_after_a.x * vel_percent_b - vel_after_b.x * vel_percent_a, vel_after_a.y * vel_percent_b - vel_after_b.y * vel_percent_a);
				// 衝突後のオブジェクトBのベクトル合成
				shell_b->vel = Vec2_Create(vel_after_b.x * vel_percent_b - vel_after_a.x * vel_percent_a, vel_after_b.y * vel_percent_b - vel_after_a.y * vel_percent_a);
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
}

// プレイシーンの終了処理
void FinalizePlay(void)
{

}