#include "Vec2.h"
#define _USE_MATH_DEFINES
#include <math.h>

// 定数の定義 ==============================================================

#define FLOAT_EPSILON (10e-6f)

// 関数の定義 ==============================================================

// <ベクトル作成>
Vec2 Vec2_Create(float x, float y)
{
	return{ x, y };
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
	return Vec2_Create(vec->x / length, vec->y / length);
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

// <ベクトルの角度>
float Vec2_Angle(Vec2* vec)
{
	return atan2f(vec->y, vec->x);
}

// <ベクトルを分解>
void Vec2_Decompose(Vec2* vec, Vec2* angle, Vec2* vec_a, Vec2* vec_b)
{
	float angle_rota = Vec2_Angle(angle);
	float vec_rota = Vec2_Angle(vec);
	float diff_rota = vec_rota - angle_rota;
	float vec_length = Vec2_Length(vec);

	float vec_a_length = vec_length * cosf(diff_rota);
	float vec_b_length = vec_length * sinf(diff_rota);
	float vec_a_rota = angle_rota;
	float vec_b_rota = angle_rota + (float)M_PI_2;

	*vec_a = Vec2_Create(vec_a_length * cosf(vec_a_rota), vec_a_length * sinf(vec_a_rota));
	*vec_b = Vec2_Create(vec_b_length * cosf(vec_b_rota), vec_b_length * sinf(vec_b_rota));
}

// <ベクトルを描画>
void Vec2_Render(Vec2* vec, Vec2* base, unsigned int color, float Thickness)
{
	float arrow_length = 20;
	float arrow_rota = Vec2_Angle(vec);
	float arrow_rota1 = arrow_rota + ToRadians(-150);
	float arrow_rota2 = arrow_rota + ToRadians(150);
	DrawLineAA(base->x, base->y, base->x + vec->x, base->y + vec->y, color, Thickness);
	DrawLineAA(base->x + vec->x, base->y + vec->y, base->x + vec->x + (arrow_length * cosf(arrow_rota1)), base->y + vec->y + (arrow_length * sinf(arrow_rota1)), color, Thickness);
	DrawLineAA(base->x + vec->x, base->y + vec->y, base->x + vec->x + (arrow_length * cosf(arrow_rota2)), base->y + vec->y + (arrow_length * sinf(arrow_rota2)), color, Thickness);
}