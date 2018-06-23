#include "Vec2.h"
#include <math.h>

// 関数の定義 ==============================================================

// <ベクトル作成>
Vec2 Vec2_Create(float x, float y)
{
	return { x, y };
}

// <ベクトルの長さ>
float Vec2_Length(Vec2* vec)
{
	return sqrtf(Vec2_LengthSquare(vec));
}

// <ベクトルの長さの二乗>
float Vec2_LengthSquare(Vec2* vec)
{
	return vec->x * vec->x + vec->y * vec->y;
}

// <もう一方のベクトルとの内積>
float Vec2_Dot(Vec2* vec, Vec2* other)
{
	return vec->x * other->x + vec->y * other->y;
}

// <もう一方のベクトルとの距離>
float Vec2_DistanceFrom(Vec2* vec, Vec2* other)
{
	return sqrtf((other->x - vec->x) * (other->x - vec->x) + (other->y - vec->y) * (other->y - vec->y));
}

// <正規化（長さを1にした）ベクトル>
Vec2 Vec2_Normalized(Vec2* vec)
{
	float length = Vec2_Length(vec);
	return Vec2_Create(vec->x / length , vec->y / length);
}