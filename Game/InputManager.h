#pragma once

// 関数の定義 ==============================================================

// キー更新
void UpdateInputManager(void);

// キーが押されているか
BOOL IsKeyDown(int key);

// キーが離されているか
BOOL IsKeyUp(int key);

// キーを押した直後か
BOOL IsKeyPressed(int key);

// キーを離した直後か
BOOL IsKeyReleased(int key);
