#include "SceneLogo.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "GameMain.h"
#include "GameUtils.h"

// �萔�̒�` ==============================================================

// ���S���[�r�[
#define MOVIE_LOGO "Resources\\Movies\\ydeagames.avi"

// �O���[�o���ϐ��̒�` ====================================================

int g_logo_count;
HGRP g_movie_logo;

// �֐��̒�` ==============================================================

// ���S�V�[���̏���������
void InitializeLogo(void)
{
	g_logo_count = 0;

	// �����ǂݍ���
	g_movie_logo = LoadGraph(MOVIE_LOGO);

	// ���S���[�r�[�Đ�
	PlayMovieToGraph(g_movie_logo);
}

// ���S�V�[���̍X�V����
void UpdateLogo(void)
{
	g_logo_count++;

	if (IsKeyPressed(PAD_INPUT_2) || g_logo_count >= 60 * 3.5f)
		RequestScene(SCENE_TITLE);
}

// ���S�V�[���̕`�揈��
void RenderLogo(void)
{
	// ���S���[�r�[�`��
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,
			(int)GetMinF(
				GetEasingValue(ESG_INCUBIC, GetPercentageRange((float)g_logo_count, 60 * 0, 60 * 0.5f), 255),		// �t�F�[�h�C��
				GetEasingValue(ESG_OUTCUBIC, 1 - GetPercentageRange((float)g_logo_count, 60 * 3, 60 * 3.5f), 255)	// �t�F�[�h�A�E�g
			)
		);
		DrawExtendGraph(SCREEN_LEFT, SCREEN_TOP, SCREEN_WIDTH, SCREEN_HEIGHT, g_movie_logo, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	//DrawFormatString(10, 10, COLOR_WHITE, "���S�V�[��");
	//DrawFormatString(10, 25, COLOR_WHITE, "�J�E���g: %3d / 180", g_logo_count);
}

// ���S�V�[���̏I������
void FinalizeLogo(void)
{
	// ���S���[�r�[�A�����[�h
	DeleteGraph(g_movie_logo);
}