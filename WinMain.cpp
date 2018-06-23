//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   WinMain.cpp
//!
//! @brief  �Q�[���v���O�����̃G���g���[�|�C���g�̃\�[�X�t�@�C��
//!
//! @date   2018/06/24
//!
//! @author GF1 26 �R������
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// �w�b�_�t�@�C���̓ǂݍ��� ================================================
#include "Game\GameMain.h"
#include "Resources\Icon\Icon.h"


// �֐���` ================================================================

// <�v���O�����̃G���g���[�|�C���g>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ���g�p�����̌x���΍�
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(nCmdShow);

#if defined(_DEBUG)
	// �������[���[�N���o�@�\�̃Z�b�g�A�b�v
	SetUpMemoryLeakDetector();
#endif

	// �f�o�b�O���p���O�t�@�C���̏o�̗͂}��
	SetOutApplicationLogValidFlag(FALSE);

	// �N�����[�h�̐ݒ�
#if defined(_DEBUG)
	// �E�C���h�E���[�h�Ŏ��s
	ChangeWindowMode(TRUE);
	// �E�C���h�E�^�C�g���̐ݒ�
	SetMainWindowText(GAME_TITLE);
	// �E�C���h�E�A�C�R���̐ݒ�
	SetWindowIconID(IDI_ICON1);
#else
	// �t���X�N���[���Ŏ��s
	ChangeWindowMode(FALSE);
#endif

	// <DX���C�u�����̏���������>
	{
		// ������Ԃ̉�ʃ��[�h�̐ݒ�
		SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32);
		// ��ʊO�Ƀt�H�[�J�X���Ă�����
		//SetAlwaysRunFlag(TRUE);
		// ���d�N���\
		SetDoubleStartValidFlag(TRUE);

		// ������
		if (DxLib_Init() == -1)
			return -1;

		// �`���𗠉�ʂɐݒ�
		SetDrawScreen(DX_SCREEN_BACK);
	}

	// <�Q�[���̏���>
	{
		// �Q�[���̏���������
		InitializeGame();

		while (!ProcessMessage() && !CheckHitKey(EXIT_KEY))
		{
			// �V�X�e���̍X�V
			UpdateSystem();

			// �Q�[���̍X�V����
			UpdateGame();
			// �Q�[���̕`�揈��
			RenderGame();

			// ����ʂ̓��e��\��ʂɔ��f
			ScreenFlip();
			// ����ʂ̏���
			ClearDrawScreen();
		}

		// �Q�[���̏I������
		FinalizeGame();
	}

	// <DX���C�u�����̏I������>
	{
		// �I��
		DxLib_End();
	}

	return 0;    // ����I��
}
