#pragma once
#include <Windows.h>
#include <vector>
#include "../Game/game.h"
extern HINSTANCE hInst;
extern HWND mainWindowHandler;
extern HWND overlayHandler;
extern std::vector<std::vector<HWND> > childWindowsHandlers;
extern int boardSize;
extern int isNewGameAnimationPlaying;
extern int areGemsFalling;
extern bool debugMode;
extern game gameObj;