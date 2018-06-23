#include "GameObject.h"
#include "GameMain.h"
#include "GameUtils.h"

// 定数の定義 ==============================================================

// <デバッグ用当たり判定表示>
#define DEBUG_HITBOX FALSE

// <コウラ>
#define SHELL_WIDTH 32
#define SHELL_HEIGHT 32

// <パドル>
#define PADDLE_WIDTH  64
#define PADDLE_HEIGHT 8

// 関数の定義 ==============================================================

// <<テクスチャ>> ------------------------------------------------------

// <テクスチャ作成>
GameTexture GameTexture_Create(HGRP texture, Vec2 anchor, Vec2 size)
{
	return { texture, anchor, size, Vec2_Create(size.x / 2, size.y / 2) };
}

// <テクスチャなし>
GameTexture GameTexture_CreateNone()
{
	return GameTexture_Create(TEXTURE_NONE, Vec2_Create(), Vec2_Create());
}

// <<スプライト>> ------------------------------------------------------

// <スプライト作成>
GameSprite GameSprite_Create(GameTexture texture, float scale, float angle)
{
	return { COLOR_WHITE, texture, Vec2_Create(), scale, angle };
}

// <スプライトなし>
GameSprite GameSprite_CreateNone()
{
	return GameSprite_Create(GameTexture_CreateNone(), 0, 0);
}

// <<オブジェクト>> ----------------------------------------------------

// <オブジェクト作成>
GameObject GameObject_Create(Vec2 pos, Vec2 vel, Vec2 size)
{
	return{ pos, vel, size, SHAPE_BOX, GameSprite_CreateNone() };
}

// <オブジェクト座標更新>
void GameObject_UpdatePosition(GameObject* obj)
{
	obj->pos.x += obj->vel.x;
	obj->pos.y += obj->vel.y;
}

// <オブジェクトXオフセット>
float GameObject_OffsetX(GameObject* obj, ObjectSide side, float pos, float padding)
{
	float offset = 0;
	switch (side)
	{
	case LEFT:
		offset = -(padding + obj->size.x / 2.f);
		break;
	case RIGHT:
		offset = (padding + obj->size.x / 2.f);
		break;
	}
	return pos + offset;
}

// <オブジェクトXオフセット>
float GameObject_OffsetY(GameObject* obj, ObjectSide side, float pos, float padding)
{
	float offset = 0;
	switch (side)
	{
	case TOP:
		offset = -(padding + obj->size.y / 2.f);
		break;
	case BOTTOM:
		offset = (padding + obj->size.y / 2.f);
		break;
	}
	return pos + offset;
}

// <オブジェクトX位置ゲット>
float GameObject_GetX(GameObject* obj, ObjectSide side, float padding)
{
	return GameObject_OffsetX(obj, side, obj->pos.x, padding);
}

// <オブジェクトY位置ゲット>
float GameObject_GetY(GameObject* obj, ObjectSide side, float padding)
{
	return GameObject_OffsetY(obj, side, obj->pos.y, padding);
}
#include <math.h>
// <オブジェクト当たり判定>
BOOL GameObject_IsHit(GameObject* obj1, GameObject* obj2)
{
	if (obj1->shape == SHAPE_BOX && obj2->shape == SHAPE_BOX)
		return (
			GameObject_GetX(obj2, LEFT) < GameObject_GetX(obj1, RIGHT) &&
			GameObject_GetX(obj1, LEFT) < GameObject_GetX(obj2, RIGHT) &&
			GameObject_GetY(obj2, TOP) < GameObject_GetY(obj1, BOTTOM) &&
			GameObject_GetY(obj1, TOP) < GameObject_GetY(obj2, BOTTOM)
			);
	else if (obj1->shape == SHAPE_OVAL && obj2->shape == SHAPE_OVAL)
	{
		// STEP1 : E2を単位円にする変換をE1に施す
		float DefAng = 0;
		float Cos = cosf(DefAng);
		float Sin = sinf(DefAng);
		float nx = obj2->size.x * Cos;
		float ny = -obj2->size.x * Sin;
		float px = obj2->size.y * Sin;
		float py = obj2->size.y * Cos;
		float ox = cosf(0)*(obj2->pos.x - obj1->pos.x) + sinf(0)*(obj2->pos.y - obj1->pos.y);
		float oy = -sinf(0)*(obj2->pos.x - obj1->pos.x) + cosf(0)*(obj2->pos.y - obj1->pos.y);

		// STEP2 : 一般式A～Gの算出
		float rx_pow2 = 1 / (obj1->size.x*obj1->size.x);
		float ry_pow2 = 1 / (obj1->size.y*obj1->size.y);
		float A = rx_pow2 * nx*nx + ry_pow2 * ny*ny;
		float B = rx_pow2 * px*px + ry_pow2 * py*py;
		float D = 2 * rx_pow2*nx*px + 2 * ry_pow2*ny*py;
		float E = 2 * rx_pow2*nx*ox + 2 * ry_pow2*ny*oy;
		float F = 2 * rx_pow2*px*ox + 2 * ry_pow2*py*oy;
		float G = (ox / obj1->size.x)*(ox / obj1->size.x) + (oy / obj1->size.y)*(oy / obj1->size.y) - 1;

		// STEP3 : 平行移動量(h,k)及び回転角度θの算出
		float tmp1 = 1 / (D*D - 4 * A*B);
		float h = (F*D - 2 * E*B)*tmp1;
		float k = (E*D - 2 * A*F)*tmp1;
		float Th = (B - A) == 0 ? 0 : atanf(D / (B - A)) * 0.5f;

		// STEP4 : +1楕円を元に戻した式で当たり判定
		float CosTh = cosf(Th);
		float SinTh = sinf(Th);
		float A_tt = A * CosTh*CosTh + B * SinTh*SinTh - D * CosTh*SinTh;
		float B_tt = A * SinTh*SinTh + B * CosTh*CosTh + D * CosTh*SinTh;
		float KK = A * h*h + B * k*k + D * h*k - E * h - F * k + G;
		if (KK>0) KK = 0;  // 念のため
		float Rx_tt = 1 + sqrtf(-KK / A_tt);
		float Ry_tt = 1 + sqrtf(-KK / B_tt);
		float x_tt = CosTh * h - SinTh * k;
		float y_tt = SinTh * h + CosTh * k;
		float JudgeValue = x_tt * x_tt / (Rx_tt*Rx_tt) + y_tt * y_tt / (Ry_tt*Ry_tt);

		if (JudgeValue <= 1)
			return TRUE;    // 衝突

		return FALSE;
	}
	else
	{
		return (
			GameObject_GetX(obj2, LEFT) < GameObject_GetX(obj1, RIGHT) &&
			GameObject_GetX(obj1, LEFT) < GameObject_GetX(obj2, RIGHT) &&
			GameObject_GetY(obj2, TOP) < GameObject_GetY(obj1, BOTTOM) &&
			GameObject_GetY(obj1, TOP) < GameObject_GetY(obj2, BOTTOM)
			);
	}
}

// <オブジェクト描画>
void GameObject_Render(GameObject* obj)
{
	// テクスチャを確認
	if (obj->sprite.texture.texture == TEXTURE_NONE)
	{
		switch (obj->shape)
		{
		default:
		case SHAPE_BOX:
			// 矩形描画
			DrawBoxAA(GameObject_GetX(obj, LEFT), GameObject_GetY(obj, TOP), GameObject_GetX(obj, RIGHT), GameObject_GetY(obj, BOTTOM), obj->sprite.color, TRUE);
			break;
		case SHAPE_OVAL:
			// 楕円
			DrawOvalAA(GameObject_GetX(obj, CENTER_X), GameObject_GetY(obj, CENTER_Y), obj->size.x, obj->size.y, 120, obj->sprite.color, TRUE);
			break;
		}
	}
	else
	{
		if (obj->sprite.texture.texture != TEXTURE_MISSING)
		{
			// デバッグ当たり判定枠を表示
			if (DEBUG_HITBOX)
				DrawBoxAA(GameObject_GetX(obj, LEFT), GameObject_GetY(obj, TOP), GameObject_GetX(obj, RIGHT), GameObject_GetY(obj, BOTTOM), obj->sprite.color, FALSE, .5f);
			// スプライト描画
			DrawRectRotaGraph2F(
				GameObject_GetX(obj, CENTER_X) + obj->sprite.offset.x, GameObject_GetY(obj, CENTER_Y) + obj->sprite.offset.y,
				(int)obj->sprite.texture.anchor.x, (int)obj->sprite.texture.anchor.y,
				(int)obj->sprite.texture.size.x, (int)obj->sprite.texture.size.y,
				obj->sprite.texture.center.x, obj->sprite.texture.center.y,
				(double)obj->sprite.scale,
				(double)obj->sprite.angle,
				obj->sprite.texture.texture,
				TRUE
			);
		}
		else
		{
			// NULLテクスチャを表示
			DrawBoxAA(GameObject_GetX(obj, LEFT), GameObject_GetY(obj, TOP), GameObject_GetX(obj, RIGHT), GameObject_GetY(obj, BOTTOM), COLOR_BLACK, TRUE);
			DrawBoxAA(GameObject_GetX(obj, LEFT), GameObject_GetY(obj, TOP), GameObject_GetX(obj, CENTER_X), GameObject_GetY(obj, CENTER_Y), COLOR_FUCHSIA, TRUE);
			DrawBoxAA(GameObject_GetX(obj, CENTER_X), GameObject_GetY(obj, CENTER_Y), GameObject_GetX(obj, RIGHT), GameObject_GetY(obj, BOTTOM), COLOR_FUCHSIA, TRUE);
			DrawBoxAA(GameObject_GetX(obj, LEFT), GameObject_GetY(obj, TOP), GameObject_GetX(obj, RIGHT), GameObject_GetY(obj, BOTTOM), obj->sprite.color, FALSE, .5f);
		}
	}
}

// <<コウラオブジェクト>> ----------------------------------------------

// <コウラオブジェクト作成>
GameObject GameObject_Shell_Create(void)
{
	GameObject obj = GameObject_Create(Vec2_Create(), Vec2_Create(), Vec2_Create(SHELL_WIDTH, SHELL_HEIGHT));
	obj.shape = SHAPE_OVAL;
	return obj;
}

// <コウラオブジェクト座標Xデフォルト>
void GameObject_Shell_SetPosXDefault(GameObject* obj, GameObject* field)
{
	obj->pos.x = GetRandRangeF(GameObject_GetX(field, LEFT), GameObject_GetX(field, RIGHT));
}

// <コウラオブジェクト座標Yデフォルト>
void GameObject_Shell_SetPosYDefault(GameObject* obj, GameObject* field)
{
	obj->pos.y = GetRandRangeF(GameObject_GetY(field, TOP), GameObject_GetY(field, BOTTOM, -85));
}

// <コウラオブジェクト移動Xデフォルト>
void GameObject_Shell_SetVelXDefault(GameObject* obj)
{
	obj->vel.x = (float)(GetRand(1) * 2 - 1)*SHELL_VEL;
}

// <<パドルオブジェクト>> ----------------------------------------------

// <パドルオブジェクト作成>
GameObject GameObject_Paddle_Create(void)
{
	return GameObject_Create(Vec2_Create(), Vec2_Create(), Vec2_Create(PADDLE_WIDTH, PADDLE_HEIGHT));
}

// <パドルオブジェクト座標Yデフォルト>
void GameObject_Paddle_SetPosYDefault(GameObject* obj, GameObject* field)
{
	obj->pos.y = GameObject_OffsetY(obj, TOP, GameObject_GetY(field, BOTTOM), 16);
}

// <<フィールドオブジェクト>> ------------------------------------------

// <フィールドオブジェクト作成>
GameObject GameObject_Field_Create(void)
{
	return GameObject_Create(Vec2_Create(SCREEN_CENTER_X, SCREEN_CENTER_Y), Vec2_Create(), Vec2_Create(SCREEN_WIDTH, SCREEN_HEIGHT));
}

// <フィールド上下衝突処理>
ObjectSide GameObject_Field_CollisionVertical(GameObject* field, GameObject* obj, ObjectConnection connection, ObjectEdgeSide edge)
{
	// ヒットサイド
	ObjectSide side_hit = NONE;

	// コウラ・上下壁当たり判定
	{
		// 縁に応じてパディングを調整
		float padding_top = GameObject_GetY(field, TOP);
		float padding_bottom = GameObject_GetY(field, BOTTOM);
		switch (edge)
		{
		case EDGESIDE_INNER:
			padding_top = GameObject_OffsetY(obj, BOTTOM, padding_top);
			padding_bottom = GameObject_OffsetY(obj, TOP, padding_bottom);
			break;
		case EDGESIDE_OUTER:
			padding_top = GameObject_OffsetY(obj, TOP, padding_top);
			padding_bottom = GameObject_OffsetY(obj, BOTTOM, padding_bottom);
			break;
		}

		// 当たり判定
		if (obj->pos.y < padding_top)
			side_hit = TOP;
		else if (padding_bottom <= obj->pos.y)
			side_hit = BOTTOM;

		// フィールドのつながり
		switch (connection)
		{
		case CONNECTION_BARRIER:
			// 壁にあたったら調整
			obj->pos.y = ClampF(obj->pos.y, padding_top, padding_bottom);
			break;
		case CONNECTION_LOOP:
			// 壁にあたったらループ
			obj->pos.y = GetLoopRangeF(obj->pos.y, padding_top, padding_bottom);
			break;
		}
	}

	return side_hit;
}

// <フィールド左右衝突処理>
ObjectSide GameObject_Field_CollisionHorizontal(GameObject* field, GameObject* obj, ObjectConnection connection, ObjectEdgeSide edge)
{
	// ヒットサイド
	ObjectSide side_hit = NONE;

	// コウラ・左右壁当たり判定
	{
		// 縁に応じてパディングを調整
		float padding_left = GameObject_GetX(field, LEFT);
		float padding_right = GameObject_GetX(field, RIGHT);
		switch (edge)
		{
		case EDGESIDE_INNER:
			padding_left = GameObject_OffsetX(obj, RIGHT, padding_left);
			padding_right = GameObject_OffsetX(obj, LEFT, padding_right);
			break;
		case EDGESIDE_OUTER:
			padding_left = GameObject_OffsetX(obj, LEFT, padding_left);
			padding_right = GameObject_OffsetX(obj, RIGHT, padding_right);
			break;
		}

		// 当たり判定
		if (obj->pos.x < padding_left)
			side_hit = LEFT;
		else if (padding_right <= obj->pos.x)
			side_hit = RIGHT;

		// フィールドのつながり
		switch (connection)
		{
		case CONNECTION_BARRIER:
			// 壁にあたったら調整
			obj->pos.x = ClampF(obj->pos.x, padding_left, padding_right);
			break;
		case CONNECTION_LOOP:
			// 壁にあたったらループ
			obj->pos.x = GetLoopRangeF(obj->pos.x, padding_left, padding_right);
			break;
		}
	}

	return side_hit;
}

// <フィールド描画>
void GameObject_Field_Render(GameObject* field)
{
}

