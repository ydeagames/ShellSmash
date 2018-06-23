//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   WinMain.cpp
//!
//! @brief  ゲームプログラムのエントリーポイントのソースファイル
//!
//! @date   2018/06/24
//!
//! @author GF1 26 山口寛雅
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み ================================================
#include "Game\GameMain.h"
#include "Resources\Icon\Icon.h"


// 関数定義 ================================================================

// <プログラムのエントリーポイント>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 未使用引数の警告対策
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(nCmdShow);

#if defined(_DEBUG)
	// メモリーリーク検出機構のセットアップ
	SetUpMemoryLeakDetector();
#endif

	// デバッグ情報用ログファイルの出力の抑制
	SetOutApplicationLogValidFlag(FALSE);

	// 起動モードの設定
#if defined(_DEBUG)
	// ウインドウモードで実行
	ChangeWindowMode(TRUE);
	// ウインドウタイトルの設定
	SetMainWindowText(GAME_TITLE);
	// ウインドウアイコンの設定
	SetWindowIconID(IDI_ICON1);
#else
	// フルスクリーンで実行
	ChangeWindowMode(FALSE);
#endif

	// <DXライブラリの初期化処理>
	{
		// 初期状態の画面モードの設定
		SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32);
		// 画面外にフォーカスしても動作
		//SetAlwaysRunFlag(TRUE);
		// 多重起動可能
		SetDoubleStartValidFlag(TRUE);

		// 初期化
		if (DxLib_Init() == -1)
			return -1;

		// 描画先を裏画面に設定
		SetDrawScreen(DX_SCREEN_BACK);
	}

	// <ゲームの処理>
	{
		// ゲームの初期化処理
		InitializeGame();

		while (!ProcessMessage() && !CheckHitKey(EXIT_KEY))
		{
			// システムの更新
			UpdateSystem();

			// ゲームの更新処理
			UpdateGame();
			// ゲームの描画処理
			RenderGame();

			// 裏画面の内容を表画面に反映
			ScreenFlip();
			// 裏画面の消去
			ClearDrawScreen();
		}

		// ゲームの終了処理
		FinalizeGame();
	}

	// <DXライブラリの終了処理>
	{
		// 終了
		DxLib_End();
	}

	return 0;    // 正常終了
}
