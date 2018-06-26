#include "ScenePlay.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "GameObject.h"
#include "GameUtils.h"
#include "GameMain.h"

#define _USE_MATH_DEFINES
#include <math.h>

// 定数の定義 ==============================================================

#define FONT_MAIN_SIZE 20

// 物理で習った係数
#define FRICTION .995f
#define BOUND_COEFFICIENT .95f
#define STOP_SPEED .1f
#define MAX_PADDLE_SPEED 40

#define SHELL_PIN_BETWEEN 40
#define NUM_SHELL_PIN 4
// C言語で習った階乗計算
#define NUM_SHELL_PINS ((NUM_SHELL_PIN+1)*NUM_SHELL_PIN/2)
#define NUM_SHELL_TRY 5
#define NUM_SHELLS (NUM_SHELL_PINS+NUM_SHELL_TRY)

#define LABEL_TIME 60
#define NUM_LABELS ((NUM_SHELL_PINS+1)*NUM_SHELL_PINS/2)

// 構造体の宣言 ============================================================

typedef struct
{
	Vec2 pos;
	int score;
	int time;
} ScoreLabel;

// グローバル変数の定義 ====================================================

int g_score;
int g_score_combo;
int g_num_shells;
int g_time_finished;
BOOL g_started;
BOOL g_done;
GameObject g_field;
GameObject g_paddle;
GameObject g_shells[NUM_SHELLS];
ScoreLabel g_labels[NUM_LABELS];

HFNT g_font_main;

HGRP g_texture_background;
HGRP g_texture_shell_red;
HGRP g_texture_shell_green;
HGRP g_texture_paddle;

HSND g_se_smash;
HSND g_se_hit_shell;
HSND g_se_hit_wall;
HSND g_se_finish;
HSND g_bgm_start;
HSND g_bgm_loop;
BOOL g_bgm_loop_flag;

// 関数の定義 ==============================================================

// プレイシーンの初期化処理
void InitializePlay(void)
{
	g_score = 0;
	g_score_combo = 0;
	g_num_shells = NUM_SHELL_PINS + 1;
	g_started = FALSE;
	g_done = FALSE;
	g_time_finished = -1;

	// フォント
	g_font_main = CreateFontToHandle("Segoe UI Black", FONT_MAIN_SIZE, DX_FONTTYPE_ANTIALIASING_4X4);

	// テクスチャ
	{
		g_texture_background = LoadGraph("Resources\\Textures\\Play\\background.jpg");
		g_texture_shell_red = LoadGraph("Resources\\Textures\\Play\\Object\\shell_red.png");
		g_texture_shell_green = LoadGraph("Resources\\Textures\\Play\\Object\\shell_green.png");
		g_texture_paddle = LoadGraph("Resources\\Textures\\Play\\Object\\paddle.png");
	}

	g_field = GameObject_Field_Create();

	// C言語の図形問題05を応用
	{
		Vec2 right = Vec2_Create(SHELL_PIN_BETWEEN, 0);
		Vec2 bottom = Vec2_Create(SHELL_PIN_BETWEEN*cosf(ToRadians(60)), SHELL_PIN_BETWEEN*sinf(ToRadians(60)));
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
					{
						g_shells[i] = GameObject_Shell_Create(Vec2_Create(px + (right.x*ix) + (bottom.x*iy), py + (right.y*ix) + (bottom.y*iy)));
						g_shells[i].sprite = GameSprite_Create(GameTexture_Create(g_texture_shell_red, Vec2_Create(), Vec2_Create(32, 32)));
						i++;
					}
				}
			}
		}

		for (int i = NUM_SHELL_PINS; i < NUM_SHELLS; i++)
		{
			g_shells[i] = GameObject_Shell_Create(Vec2_Create(cx, by - sh * 2));
			g_shells[i].sprite = GameSprite_Create(GameTexture_Create(g_texture_shell_green, Vec2_Create(), Vec2_Create(32, 32)));
			g_shells[i].sprite.color = COLOR_GREEN;
		}

		g_paddle = GameObject_Paddle_Create();
		g_paddle.pos = Vec2_Create(cx, by - sh * 2);
		g_paddle.sprite = GameSprite_Create(GameTexture_Create(g_texture_paddle, Vec2_Create(), Vec2_Create(69, 19)));
		g_paddle.sprite.color = COLOR_BLUE;
	}

	for (int i = 0; i < NUM_LABELS; i++)
	{
		g_labels[i].pos = Vec2_Create();
		g_labels[i].score = 0;
		g_labels[i].time = 0;
	}

	// 効果音
	{
		g_se_smash = LoadSoundMem("Resources\\Audio\\se_smash.ogg");
		g_se_hit_shell = LoadSoundMem("Resources\\Audio\\se_hit_shell.ogg");
		g_se_hit_wall = LoadSoundMem("Resources\\Audio\\se_hit_wall.ogg");
		g_se_finish = LoadSoundMem("Resources\\Audio\\se_finish.ogg");
		g_bgm_start = LoadSoundMem("Resources\\Audio\\bgm_start.ogg");
		g_bgm_loop = LoadSoundMem("Resources\\Audio\\bgm_loop.ogg");
		g_bgm_loop_flag = FALSE;
	}

	PlaySoundMem(g_bgm_start, DX_PLAYTYPE_BACK);
}

// プレイシーンの更新処理
void UpdatePlay(void)
{
	GameObject inner_field = g_field;
	inner_field.size.x = 200;
	inner_field.size.y = 200;
	inner_field.pos.y += 100;

	if (g_time_finished == 0)
		RequestScene(SCENE_RESULT);
	if (g_time_finished > 0)
		g_time_finished--;

	// 操作
	{
		int mouse_x, mouse_y;
		GetMousePoint(&mouse_x, &mouse_y);

		{
			GameObject mouse = g_paddle;
			mouse.pos = Vec2_Create((float)mouse_x, (float)mouse_y);

			GameObject_Field_CollisionHorizontal(&inner_field, &mouse, CONNECTION_BARRIER, EDGESIDE_INNER);
			GameObject_Field_CollisionVertical(&inner_field, &mouse, CONNECTION_BARRIER, EDGESIDE_INNER);

			g_paddle.vel.x = ClampF(mouse.pos.x - g_paddle.pos.x, -MAX_PADDLE_SPEED, MAX_PADDLE_SPEED);
			g_paddle.vel.y = ClampF(mouse.pos.y - g_paddle.pos.y, -MAX_PADDLE_SPEED, MAX_PADDLE_SPEED);
		}
	}

	// オブジェクト移動
	{
		BOOL finish = TRUE;

		g_paddle.pos.x += g_paddle.vel.x;
		g_paddle.pos.y += g_paddle.vel.y;

		for (int i = 0; i < g_num_shells; i++)
		{
			g_shells[i].pos.x += g_shells[i].vel.x;
			g_shells[i].pos.y += g_shells[i].vel.y;
			g_shells[i].vel.x *= FRICTION;
			g_shells[i].vel.y *= FRICTION;

			if (GetAbsF(g_shells[i].vel.x) < STOP_SPEED && GetAbsF(g_shells[i].vel.y) < STOP_SPEED)
			{
				g_shells[i].vel.x = 0;
				g_shells[i].vel.y = 0;
			}
			else
				finish = FALSE;
		}

		for (int i = 0; i < NUM_LABELS; i++)
			g_labels[i].time = GetMax(0, g_labels[i].time - 1);

		// 終了判定
		if (g_done && finish)
		{
			g_num_shells++;
			if (g_num_shells > NUM_SHELLS)
			{
				g_num_shells = NUM_SHELLS;
				if (g_time_finished < 0)
				{
					PlaySoundMem(g_se_finish, DX_PLAYTYPE_BACK);
					g_time_finished = 60 * 3;
				}
			}
			else
			{
				g_score_combo = 0;
				g_started = FALSE;
				g_done = FALSE;
			}
		}
	}

	// 壁
	{
		// 壁とコウラ
		for (int i = 0; i < g_num_shells; i++)
		{
			if (GameObject_Field_CollisionHorizontal(&g_field, &g_shells[i], CONNECTION_BARRIER, EDGESIDE_INNER))
			{
				g_shells[i].vel.x *= -BOUND_COEFFICIENT;
				PlaySoundMem(g_se_hit_wall, DX_PLAYTYPE_BACK);
			}
			if (GameObject_Field_CollisionVertical(&g_field, &g_shells[i], CONNECTION_BARRIER, EDGESIDE_INNER))
			{
				g_shells[i].vel.y *= -BOUND_COEFFICIENT;
				PlaySoundMem(g_se_hit_wall, DX_PLAYTYPE_BACK);
			}
		}

		// 壁とパドル
		{
			GameObject_Field_CollisionHorizontal(&inner_field, &g_paddle, CONNECTION_BARRIER, EDGESIDE_INNER);
			GameObject_Field_CollisionVertical(&inner_field, &g_paddle, CONNECTION_BARRIER, EDGESIDE_INNER);
		}
	}

	if (!g_done)
	{
		int i = g_num_shells - 1;
		// パドルとコウラ
		if (GameObject_IsHit(&g_paddle, &g_shells[i]))
		{
			if (g_started)
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
				PlaySoundMem(g_se_smash, DX_PLAYTYPE_BACK);
				g_done = TRUE;
			}
		}
		else
			g_started = TRUE;
	}

	for (int iy = 0; iy < g_num_shells; iy++)
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

				// スコア
				{
					int score_plus;

					// スコア加算
					{
						g_score_combo = GetMin(24, g_score_combo + 1);
						score_plus = g_score_combo*10;
						if (ix >= NUM_SHELL_PINS && iy >= NUM_SHELL_PINS)
							score_plus *= 10;
						g_score += score_plus;
					}

					// スコア表示
					{
						g_labels[iy + ix].pos = Vec2_Create((shell_a->pos.x + shell_b->pos.x) / 2, (shell_a->pos.y + shell_b->pos.y) / 2);
						g_labels[iy + ix].score = score_plus;
						g_labels[iy + ix].time = LABEL_TIME;
					}
				}

				// 衝突
				{
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

				PlaySoundMem(g_se_hit_shell, DX_PLAYTYPE_BACK);
			}
		}
	}

	if (!g_bgm_loop_flag && !CheckSoundMem(g_bgm_start))
	{
		g_bgm_loop_flag = TRUE;
		PlaySoundMem(g_bgm_loop, DX_PLAYTYPE_LOOP);
	}
}

// プレイシーンの描画処理
void RenderPlay(void)
{
	{
		GameObject back = g_field;
		back.sprite = GameSprite_Create(GameTexture_Create(g_texture_background, Vec2_Create(), Vec2_Create(612, 408)), 1.2f);
		GameObject_Render(&back);
	}
	{
		GameObject back = g_field;
		back.size.x -= 10;
		back.size.y -= 10;
		back.sprite.color = COLOR_BLACK;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
		GameObject_Render(&back);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	for (int i = 0; i < g_num_shells; i++)
	{
		GameObject_Render(&g_shells[i]);
	}

	for (int i = 0; i < NUM_LABELS; i++)
	{
		if (g_labels[i].time > 0)
		{
			int width = GetDrawFormatStringWidthToHandle(g_font_main, "%d", g_labels[i].score);
			DrawFormatStringToHandle((int)(g_labels[i].pos.x - width / 2), (int)(g_labels[i].pos.y - (LABEL_TIME - g_labels[i].time)), COLOR_WHITE, g_font_main, "%d", g_labels[i].score);
		}
	}

	GameObject_Render(&g_paddle);

	{
		GameObject icon = GameObject_Create(Vec2_Create(SCREEN_RIGHT - 60, SCREEN_BOTTOM - 25));
		icon.sprite = GameSprite_Create(GameTexture_Create(g_texture_shell_green, Vec2_Create(), Vec2_Create(32, 32)), .75f);
		GameObject_Render(&icon);
	}
	DrawFormatStringToHandle(SCREEN_LEFT + 10, SCREEN_TOP + 10, COLOR_WHITE, g_font_main, "SCORE %d", g_score);
	DrawFormatStringToHandle(SCREEN_RIGHT - 60 + 15, SCREEN_BOTTOM - 25 - 8, COLOR_WHITE, g_font_main, "×%d", NUM_SHELLS - g_num_shells);
}

// プレイシーンの終了処理
void FinalizePlay(void)
{
	DeleteFontToHandle(g_font_main);

	DeleteGraph(g_texture_background);
	DeleteGraph(g_texture_shell_red);
	DeleteGraph(g_texture_shell_green);
	DeleteGraph(g_texture_paddle);

	StopSoundMem(g_bgm_start);
	StopSoundMem(g_bgm_loop);

	DeleteSoundMem(g_se_smash);
	DeleteSoundMem(g_se_hit_shell);
	DeleteSoundMem(g_se_hit_wall);
	DeleteSoundMem(g_bgm_start);
	DeleteSoundMem(g_bgm_loop);
}