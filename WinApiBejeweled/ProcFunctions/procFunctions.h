#pragma once
#include <Windows.h>
#include "../Game/game.h"

LRESULT CALLBACK mainProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK childProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK overlayProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK aboutProc(HWND, UINT, WPARAM, LPARAM);

struct tileInfo {
  state tileState = state::notInitialized;
  state lastColor = state::notInitialized;
  bool clicked = false;
};