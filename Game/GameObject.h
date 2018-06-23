#pragma once
#include "Vec2.h"

// �񋓌^�̒�` ============================================================

// <�ʒu�֌W> ----------------------------------------------------------
typedef enum
{
	NONE = 0,
	CENTER_X = 1,				// X����
	LEFT = -2,					// ��
	RIGHT = 2,					// �E
	CENTER_Y = -1,				// Y����
	TOP = -3,					// ��
	BOTTOM = 3					// ��
} ObjectSide;

// <���̈ʒu> ----------------------------------------------------------
typedef enum
{
	EDGESIDE_CENTER = 0,		// ���̏�
	EDGESIDE_OUTER = -1,		// ���̊O��
	EDGESIDE_INNER = 1			// ���̓���
} ObjectEdgeSide;

// <���[���h�̂Ȃ���> ------------------------------------------------
typedef enum
{
	CONNECTION_NONE = 0,		// �q����Ȃ��A�����Ȃ���ԂɈړ�
	CONNECTION_BARRIER,			// �ǂ�����A�i�߂Ȃ�
	CONNECTION_LOOP				// ���Α�����o�Ă���
} ObjectConnection;

// <�I�u�W�F�N�g�̌`> --------------------------------------------------
typedef enum
{
	SHAPE_BOX,					// �����`
	SHAPE_OVAL,					// �ȉ~
} ObjectShape;

// �\���̂̐錾 ============================================================

// <�e�N�X�`��>
typedef struct
{
	HGRP texture;				// <�e�N�X�`��>
	Vec2 anchor;				// <�e�N�X�`����_>
	Vec2 size;					// <�e�N�X�`���T�C�Y>
	Vec2 center;				// <�e�N�X�`�����S>
} GameTexture;

// <�X�v���C�g�I�u�W�F�N�g>
typedef struct
{
	unsigned int color;			// <�F>
	GameTexture texture;		// <�e�N�X�`��>
	Vec2 offset;				// <�I�t�Z�b�g>
	float scale;				// <�X�P�[��>
	float angle;				// <��]>
} GameSprite;

// <�Q�[���I�u�W�F�N�g>
typedef struct
{
	Vec2 pos;					// <�ʒu>
	Vec2 vel;					// <���x>
	Vec2 size;					// <�傫��>
	ObjectShape shape;			// <�`>
	GameSprite sprite;			// <�X�v���C�g>
} GameObject;

// �萔�̒�` ==============================================================

// <�e�N�X�`��>
#define TEXTURE_MISSING -1		// �e�N�X�`����������܂���
#define TEXTURE_NONE -2			// �e�N�X�`���Ȃ�
// <�p�h��>
#define SHIP_VEL 2.3f			// �p�h���̑��x
// <�R�E��>
#define SHELL_VEL 2			// �R�E���̑��x

// �֐��̐錾 ==============================================================

// <<�e�N�X�`��>> ------------------------------------------------------

// <�e�N�X�`���쐬>
GameTexture GameTexture_Create(HGRP texture, Vec2 anchor, Vec2 size);

// <�e�N�X�`���Ȃ�>
GameTexture GameTexture_CreateNone();

// <<�X�v���C�g>> ------------------------------------------------------

// <�X�v���C�g�쐬>
GameSprite GameSprite_Create(GameTexture texture, float scale = 1, float angle = 0);

// <�X�v���C�g�Ȃ�>
GameSprite GameSprite_CreateNone();

// <<�I�u�W�F�N�g>> ----------------------------------------------------

// <�I�u�W�F�N�g�쐬>
GameObject GameObject_Create(Vec2 pos = Vec2_Create(), Vec2 vel = Vec2_Create(), Vec2 size = Vec2_Create());

// <�I�u�W�F�N�g���W�X�V>
void GameObject_UpdatePosition(GameObject* obj);

// <�I�u�W�F�N�gX�I�t�Z�b�g>
float GameObject_OffsetX(GameObject* obj, ObjectSide side, float pos = 0.f, float margin = 0.f);

// <�I�u�W�F�N�gX�I�t�Z�b�g>
float GameObject_OffsetY(GameObject* obj, ObjectSide side, float pos = 0.f, float margin = 0.f);

// <�I�u�W�F�N�gX�ʒu�Q�b�g>
float GameObject_GetX(GameObject* obj, ObjectSide side, float margin = 0.f);

// <�I�u�W�F�N�gY�ʒu�Q�b�g>
float GameObject_GetY(GameObject* obj, ObjectSide side, float margin = 0.f);

// <�I�u�W�F�N�g�����蔻��>
BOOL GameObject_IsHit(GameObject* obj1, GameObject* obj2);

// <�I�u�W�F�N�g�`��>
void GameObject_Render(GameObject* obj);

// <<�R�E���I�u�W�F�N�g>> ----------------------------------------------

// <�R�E���I�u�W�F�N�g�쐬>
GameObject GameObject_Shell_Create(void);

// <�R�E���I�u�W�F�N�g���WX�f�t�H���g>
void GameObject_Shell_SetPosXDefault(GameObject* obj, GameObject* field);

// <�R�E���I�u�W�F�N�g���WY�f�t�H���g>
void GameObject_Shell_SetPosYDefault(GameObject* obj, GameObject* field);

// <�R�E���I�u�W�F�N�g�ړ�X�f�t�H���g>
void GameObject_Shell_SetVelXDefault(GameObject* obj);

// <<�p�h���I�u�W�F�N�g>> ----------------------------------------------

// <�p�h���I�u�W�F�N�g�쐬>
GameObject GameObject_Paddle_Create(void);

// <�p�h���I�u�W�F�N�g���WY�f�t�H���g>
void GameObject_Paddle_SetPosYDefault(GameObject* obj, GameObject* field);

// <<�t�B�[���h�I�u�W�F�N�g>> ------------------------------------------

// <�t�B�[���h�I�u�W�F�N�g�쐬>
GameObject GameObject_Field_Create(void);

// <�t�B�[���h�㉺�Փˏ���>
ObjectSide GameObject_Field_CollisionVertical(GameObject* field, GameObject* obj, ObjectConnection connection, ObjectEdgeSide edge);

// <�t�B�[���h���E�Փˏ���>
ObjectSide GameObject_Field_CollisionHorizontal(GameObject* field, GameObject* obj, ObjectConnection connection, ObjectEdgeSide edge);

// <�t�B�[���h�`��>
void GameObject_Field_Render(GameObject* field);