#include "SceneTitle.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "GameMain.h"
#include "GameUtils.h"

// �萔�̒�` ==============================================================

// �^�C�g���e�N�X�`��
#define TEXTURE_TITLE "Resources\\Textures\\Title\\title.png"
#define TEXTURE_TEXT "Resources\\Textures\\Title\\press_button.png"

// �O���[�o���ϐ��̒�` ====================================================

int g_title_count;
HGRP g_texture_title;
HGRP g_texture_text;

// �֐��̒�` ==============================================================

// �^�C�g���V�[���̏���������
void InitializeTitle(void)
{
	g_title_count = 0;

	// �e�N�X�`����ǂݍ���
	g_texture_title = LoadGraph(TEXTURE_TITLE);
	g_texture_text = LoadGraph(TEXTURE_TEXT);
}

// �^�C�g���V�[���̍X�V����
void UpdateTitle(void)
{
	g_title_count++;

	if (IsKeyPressed(PAD_INPUT_2))
		RequestScene(SCENE_PLAY);
}

// �^�C�g���V�[���̕`�揈��
void RenderTitle(void)
{
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,
			(int)GetEasingValue(ESG_INCUBIC, GetPercentageRange((float)g_title_count, 60 * 0, 60 * 0.5f), 255)		// �t�F�[�h�C��
		);
		DrawRotaGraph(SCREEN_CENTER_X, SCREEN_CENTER_Y, 1, 0, g_texture_title, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	{
		int duration = 90;
		int idle = 30;

		SetDrawBlendMode(DX_BLENDMODE_ALPHA,
			(int)GetEasingValue(ESG_INQUART, GetPercentageRange((float)g_title_count, 60 * 0, 60 * 0.5f), 255)
		);
		DrawRotaGraph(SCREEN_CENTER_X, SCREEN_CENTER_Y + 96, 1, 0, g_texture_text, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,
			(int)GetMinF(
				GetEasingValue(ESG_INQUART, GetPercentageRange((float)g_title_count, 60 * 0, 60 * 0.5f), 255),		// �t�F�[�h�C��
				GetEasingValueRange(ESG_INCIRC, GetPercentageRange((float)(g_title_count % (duration + idle)), 0, (float)duration), 255, 32)
			)
		);
		DrawRotaGraph(SCREEN_CENTER_X, SCREEN_CENTER_Y + 96,
			GetEasingValueRange(ESG_LINEAR, GetPercentageRange((float)(g_title_count % (duration + idle)), 0, (float)duration), 1.1f, 1.f),
			0, g_texture_text, TRUE
		);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	//DrawFormatString(10, 10, COLOR_WHITE, "�^�C�g���V�[��");
	//DrawFormatString(10, 25, COLOR_WHITE, "�J�E���g: %3d / 180", g_title_count);
}

// �^�C�g���V�[���̏I������
void FinalizeTitle(void)
{
	// �e�N�X�`���A�����[�h
	DeleteGraph(g_texture_title);
}