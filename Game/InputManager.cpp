#include "InputManager.h"

// グローバル変数の定義 ====================================================

static int g_input_state;
static int s_input_state_last;

// 関数の定義 ==============================================================

// キー更新
void UpdateInputManager(void)
{
	s_input_state_last = g_input_state;
	g_input_state = GetJoypadInputState(DX_INPUT_KEY_PAD1);
}

// キーが押されているか
BOOL IsKeyDown(int key)
{
	return g_input_state & key;
}

// キーが離されているか
BOOL IsKeyUp(int key)
{
	return !IsKeyDown(key);
}

// キーを押した直後か
BOOL IsKeyPressed(int key)
{
	return !(s_input_state_last & key) && (g_input_state & key);
}

// キーを離した直後か
BOOL IsKeyReleased(int key)
{
	return (s_input_state_last & key) && !(g_input_state & key);
}
