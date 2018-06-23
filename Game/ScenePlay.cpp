#include "ScenePlay.h"
#include "SceneManager.h"
#include "InputManager.h"

// �O���[�o���ϐ��̒�` ====================================================

int g_play_count;

// �֐��̒�` ==============================================================

// �v���C�V�[���̏���������
void InitializePlay(void)
{
	g_play_count = 0;
}

// �v���C�V�[���̍X�V����
void UpdatePlay(void)
{
	//if (g_play_count++ >= 180)
	if (IsKeyDown(PAD_INPUT_2))
		RequestScene(SCENE_RESULT);
}

// �v���C�V�[���̕`�揈��
void RenderPlay(void)
{
	DrawFormatString(10, 10, COLOR_WHITE, "�v���C�V�[��");
	//DrawFormatString(10, 25, COLOR_WHITE, "�J�E���g: %3d / 180", g_play_count);
	DrawFormatString(10, 25, COLOR_WHITE, "PRESS X TO END");
}

// �v���C�V�[���̏I������
void FinalizePlay(void)
{

}