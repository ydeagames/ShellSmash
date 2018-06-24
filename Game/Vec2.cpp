#include "Vec2.h"
#include <math.h>

// �萔�̒�` ==============================================================

#define FLOAT_EPSILON (10e-6f)

// �֐��̒�` ==============================================================

// <�x�N�g���쐬>
Vec2 Vec2_Create(float x, float y)
{
	return { x, y };
}

// <�x�N�g���̒���>
float Vec2_Length(Vec2* vec)
{
	return sqrtf(Vec2_LengthSquared(vec));
}

// <�x�N�g���̒����̓��>
float Vec2_LengthSquared(Vec2* vec)
{
	return vec->x * vec->x + vec->y * vec->y;
}

// <��������̃x�N�g���Ƃ̓���>
float Vec2_Dot(Vec2* vec, Vec2* other)
{
	return vec->x * other->x + vec->y * other->y;
}

// <��������̃x�N�g���Ƃ̋���>
float Vec2_LengthTo(Vec2* vec, Vec2* other)
{
	return sqrtf(Vec2_LengthSquaredTo(vec, other));
}

// <��������̃x�N�g���Ƃ̋����̓��>
float Vec2_LengthSquaredTo(Vec2* vec, Vec2* other)
{
	return (other->x - vec->x) * (other->x - vec->x) + (other->y - vec->y) * (other->y - vec->y);
}

// <���K���i������1�ɂ����j�x�N�g��>
Vec2 Vec2_Normalized(Vec2* vec)
{
	float length = Vec2_Length(vec);
	return Vec2_Create(vec->x / length , vec->y / length);
}

// <���l�̃x�N�g����>
BOOL Vec2_EqualsEpsilon(Vec2* vec, Vec2* other, float epsilon)
{
	return fabsf(vec->x - other->x) < epsilon && fabsf(vec->y - other->y) < epsilon;
}

// <���l�̃x�N�g����>
BOOL Vec2_Equals(Vec2* vec, Vec2* other)
{
	return Vec2_EqualsEpsilon(vec, other, FLOAT_EPSILON);
}

// <0�x�N�g����>
BOOL Vec2_IsZero(Vec2* vec)
{
	return Vec2_Equals(vec, &Vec2_Create());
}
