#include "Vec2.h"
#include <math.h>

// 定数の定義 ==============================================================

#define FLOAT_EPSILON (10e-6f)

// 関数の定義 ==============================================================

// <ベクトル作成>
Vec2 Vec2_Create(float x, float y)
{
	return { x, y };
}

// <ベクトルの長さ>
float Vec2_Length(Vec2* vec)
{
	return sqrtf(Vec2_LengthSquared(vec));
}

// <ベクトルの長さの二乗>
float Vec2_LengthSquared(Vec2* vec)
{
	return vec->x * vec->x + vec->y * vec->y;
}

// <もう一方のベクトルとの内積>
float Vec2_Dot(Vec2* vec, Vec2* other)
{
	return vec->x * other->x + vec->y * other->y;
}

// <もう一方のベクトルとの距離>
float Vec2_LengthTo(Vec2* vec, Vec2* other)
{
	return sqrtf(Vec2_LengthSquaredTo(vec, other));
}

// <もう一方のベクトルとの距離の二乗>
float Vec2_LengthSquaredTo(Vec2* vec, Vec2* other)
{
	return (other->x - vec->x) * (other->x - vec->x) + (other->y - vec->y) * (other->y - vec->y);
}

// <正規化（長さを1にした）ベクトル>
Vec2 Vec2_Normalized(Vec2* vec)
{
	float length = Vec2_Length(vec);
	return Vec2_Create(vec->x / length , vec->y / length);
}

// <同値のベクトルか>
BOOL Vec2_EqualsEpsilon(Vec2* vec, Vec2* other, float epsilon)
{
	return fabsf(vec->x - other->x) < epsilon && fabsf(vec->y - other->y) < epsilon;
}

// <同値のベクトルか>
BOOL Vec2_Equals(Vec2* vec, Vec2* other)
{
	return Vec2_EqualsEpsilon(vec, other, FLOAT_EPSILON);
}

// <0ベクトルか>
BOOL Vec2_IsZero(Vec2* vec)
{
	return Vec2_Equals(vec, &Vec2_Create());
}
