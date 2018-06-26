#include "ScenePlay.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "GameObject.h"
#include "GameUtils.h"
#define _USE_MATH_DEFINES
#include <math.h>

// 定数の定義 ==============================================================

#define COEFFICIENCE .995
#define BOUNCE .95

#define NUM_SHELL_PIN 4
// C言語で習った階乗計算
#define NUM_SHELL_PINS ((NUM_SHELL_PIN+1)*NUM_SHELL_PIN/2)
#define NUM_SHELL_PIN_BETWEEN 40
#define NUM_SHELL_TRY 1
#define NUM_SHELLS (NUM_SHELL_PINS+NUM_SHELL_TRY)

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

	// C言語の図形問題05を応用
	{
		Vec2 right = Vec2_Create(NUM_SHELL_PIN_BETWEEN, 0);
		Vec2 bottom = Vec2_Create(NUM_SHELL_PIN_BETWEEN*cosf(ToRadians(60)), NUM_SHELL_PIN_BETWEEN*sinf(ToRadians(60)));
		float cx = GameObject_GetX(&g_field, CENTER_X);
		float ty = GameObject_GetY(&g_field, TOP);
		float by = GameObject_GetY(&g_field, BOTTOM);
		float sw = right.x*(NUM_SHELL_PIN / 2);
		float sh = bottom.y*(NUM_SHELL_PIN / 2);
		float px = cx + bottom.x - sw;
		float py = ty + bottom.y;

		{
			int i = 0;
			for (int iy = 0; iy < NUM_SHELL_PIN; iy++)
			{
				for (int ix = 0; ix < NUM_SHELL_PIN; ix++)
				{
					// ixが右からiyの位置より右ならば
					if (ix < NUM_SHELL_PIN - iy)
						g_shells[i++] = GameObject_Shell_Create(Vec2_Create(px + (right.x*ix) + (bottom.x*iy), py + (right.y*ix) + (bottom.y*iy)));
				}
			}
		}

		for (int i = NUM_SHELL_PINS; i < NUM_SHELLS; i++)
		{
			g_shells[i] = GameObject_Shell_Create(Vec2_Create(cx, by - sh * 2));
			g_shells[i].sprite.color = COLOR_GREEN;
		}
	}

	g_paddle = GameObject_Paddle_Create();
	g_paddle.sprite.color = COLOR_GREEN;
}

// プレイシーンの更新処理
void UpdatePlay(void)
{
	GameObject inner_field = g_field;
	inner_field.size.x = 200;
	inner_field.size.y = 200;
	inner_field.pos.y += 100;

	//if (g_play_count++ >= 180)
	if (IsKeyPressed(PAD_INPUT_2))
		RequestScene(SCENE_RESULT);

	// 操作
	{
		int mouse_x, mouse_y;
		GetMousePoint(&mouse_x, &mouse_y);

		{
			GameObject mouse = g_paddle;
			mouse.pos = Vec2_Create((float)mouse_x, (float)mouse_y);

			GameObject_Field_CollisionHorizontal(&inner_field, &mouse, CONNECTION_BARRIER, EDGESIDE_INNER);
			GameObject_Field_CollisionVertical(&inner_field, &mouse, CONNECTION_BARRIER, EDGESIDE_INNER);

			g_paddle.vel.x = mouse.pos.x - g_paddle.pos.x;
			g_paddle.vel.y = mouse.pos.y - g_paddle.pos.y;
		}
	}

	// オブジェクト移動
	{
		g_paddle.pos.x += g_paddle.vel.x;
		g_paddle.pos.y += g_paddle.vel.y;

		for (int i = 0; i < NUM_SHELLS; i++)
		{
			g_shells[i].pos.x += g_shells[i].vel.x;
			g_shells[i].pos.y += g_shells[i].vel.y;
			g_shells[i].vel.x *= COEFFICIENCE;
			g_shells[i].vel.y *= COEFFICIENCE;
		}
	}

	// 壁
	{
		// 壁とコウラ
		for (int i = 0; i < NUM_SHELLS; i++)
		{
			if (GameObject_Field_CollisionHorizontal(&g_field, &g_shells[i], CONNECTION_BARRIER, EDGESIDE_INNER))
			{
				g_shells[i].vel.x *= -BOUNCE;
			}
			if (GameObject_Field_CollisionVertical(&g_field, &g_shells[i], CONNECTION_BARRIER, EDGESIDE_INNER))
			{
				g_shells[i].vel.y *= -BOUNCE;
			}
		}

		// 壁とパドル
		{
			GameObject_Field_CollisionHorizontal(&inner_field, &g_paddle, CONNECTION_BARRIER, EDGESIDE_INNER);
			GameObject_Field_CollisionVertical(&inner_field, &g_paddle, CONNECTION_BARRIER, EDGESIDE_INNER);
		}
	}

	for (int i = NUM_SHELL_PINS; i < NUM_SHELLS; i++)
	{
		// パドルとコウラ
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

			// コウラ同士
			if (GameObject_IsHit(shell_a, shell_b))
			{
				// めり込み防止
				{
					float r1 = GetMinF(shell_a->size.x, shell_a->size.y) / 2;
					float r2 = GetMinF(shell_b->size.x, shell_b->size.y) / 2;
					float angle = Vec2_Angle(&Vec2_Create(shell_a->pos.x - shell_b->pos.x, shell_a->pos.y - shell_b->pos.y));
					shell_a->pos = Vec2_Create((r1 + r2)*cosf(angle) + shell_b->pos.x, (r1 + r2)*sinf(angle) + shell_b->pos.y);
				}

				// 衝突前のオブジェクトAの速度ベクトル
				Vec2 vel_before_a = shell_a->vel;
				// 衝突前のオブジェクトBの速度ベクトル
				Vec2 vel_before_b = shell_b->vel;

				// 進むべき方向ベクトル
				Vec2 forward = Vec2_Create(shell_b->pos.x - shell_a->pos.x, shell_b->pos.y - shell_a->pos.y);

				{
					// 進むべき方向ベクトルと平行
					Vec2 vel_parallel_a, vel_parallel_b;
					// 進むべき方向ベクトルと鉛直
					Vec2 vel_perpendicular_a, vel_perpendicular_b;

					// 衝突前のオブジェクトAのベクトル分解
					Vec2_Decompose(&vel_before_a, &forward, &vel_parallel_a, &vel_perpendicular_a);
					// 衝突前のオブジェクトBのベクトル分解
					Vec2_Decompose(&vel_before_b, &forward, &vel_parallel_b, &vel_perpendicular_b);

					// 衝突後のオブジェクトAのベクトル合成
					Vec2_Add(&shell_a->vel, &vel_parallel_b, &vel_perpendicular_a);
					// 衝突後のオブジェクトBのベクトル合成
					Vec2_Add(&shell_b->vel, &vel_parallel_a, &vel_perpendicular_b);
				}
			}
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