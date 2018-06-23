#include "Vec2.h"
#include <math.h>

// �֐��̒�` ==============================================================

// <�x�N�g���쐬>
Vec2 Vec2_Create(float x, float y)
{
	return { x, y };
}

// <�x�N�g���̒���>
float Vec2_Length(Vec2* vec)
{
	return sqrtf(Vec2_LengthSquare(vec));
}

// <�x�N�g���̒����̓��>
float Vec2_LengthSquare(Vec2* vec)
{
	return vec->x * vec->x + vec->y * vec->y;
}

// <��������̃x�N�g���Ƃ̓���>
float Vec2_Dot(Vec2* vec, Vec2* other)
{
	return vec->x * other->x + vec->y * other->y;
}

// <��������̃x�N�g���Ƃ̋���>
float Vec2_DistanceFrom(Vec2* vec, Vec2* other)
{
	return sqrtf((other->x - vec->x) * (other->x - vec->x) + (other->y - vec->y) * (other->y - vec->y));
}

// <���K���i������1�ɂ����j�x�N�g��>
Vec2 Vec2_Normalized(Vec2* vec)
{
	float length = Vec2_Length(vec);
	return Vec2_Create(vec->x / length , vec->y / length);
}