#pragma once

// �\���̂̐錾 ============================================================

// <�x�N�g��>
typedef struct
{
	float x;	// X���W
	float y;	// Y���W
} Vec2;

// �֐��̐錾 ==============================================================

// <�x�N�g���쐬>
Vec2 Vec2_Create(float x = 0.f, float y = 0.f);

// <�x�N�g���̒���>
float Vec2_Length(Vec2* vec);

// <�x�N�g���̒����̓��>
float Vec2_LengthSquare(Vec2* vec);

// <��������̃x�N�g���Ƃ̓���>
float Vec2_Dot(Vec2* vec, Vec2* other);

// <��������̃x�N�g���Ƃ̋���>
float Vec2_DistanceFrom(Vec2* vec, Vec2* other);

// <��������̃x�N�g���Ƃ̋����̓��>
float Vec2_DistanceSquareFrom(Vec2* vec, Vec2* other);

// <���K���i������1�ɂ����j�x�N�g��>
Vec2 Vec2_Normalized(Vec2* vec);
