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
float Vec2_LengthSquared(Vec2* vec);

// <もう一方のベクトルとの内積>
float Vec2_Dot(Vec2* vec, Vec2* other);

// <もう一方のベクトルとの距離>
float Vec2_LengthTo(Vec2* vec, Vec2* other);

// <もう一方のベクトルとの距離の二乗>
float Vec2_LengthSquaredTo(Vec2* vec, Vec2* other);

// <正規化（長さを1にした）ベクトル>
Vec2 Vec2_Normalized(Vec2* vec);

// <同値のベクトルか>
BOOL Vec2_EqualsEpsilon(Vec2* vec, Vec2* other, float epsilon);

// <同値のベクトルか>
BOOL Vec2_Equals(Vec2* vec, Vec2* other);

// <0ベクトルか>
BOOL Vec2_IsZero(Vec2* vec);


// <ベクトルの角度>
float Vec2_Angle(Vec2* vec);

// <ベクトルを分解>
void Vec2_Decompose(Vec2* vec, Vec2* angle, Vec2* vec_a, Vec2* vec_b);

// <ベクトルを描画>
void Vec2_Render(Vec2* vec, Vec2* base, unsigned int color, float Thickness = 1.f);
