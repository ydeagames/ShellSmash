#pragma once

// 構造体の宣言 ============================================================

// <ベクトル>
typedef struct
{
	float x;	// X座標
	float y;	// Y座標
} Vec2;

// 関数の宣言 ==============================================================

// <ベクトル作成>
Vec2 Vec2_Create(float x = 0.f, float y = 0.f);

// <ベクトルの長さ>
float Vec2_Length(Vec2* vec);

// <ベクトルの長さの二乗>
float Vec2_LengthSquare(Vec2* vec);

// <もう一方のベクトルとの内積>
float Vec2_Dot(Vec2* vec, Vec2* other);

// <もう一方のベクトルとの距離>
float Vec2_DistanceFrom(Vec2* vec, Vec2* other);

// <もう一方のベクトルとの距離の二乗>
float Vec2_DistanceSquareFrom(Vec2* vec, Vec2* other);

// <正規化（長さを1にした）ベクトル>
Vec2 Vec2_Normalized(Vec2* vec);
