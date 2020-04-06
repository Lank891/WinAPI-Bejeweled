#include "WinApiBejeweled.h"
#include "Helpers/strings.h"
#include "Register/register.h"
#include "Helpers/sizesAndColors.h"
#include "CustomMessages/customMessages.h"
#include "Game/game.h"
#include "framework.h"
#include <vector>

HINSTANCE hInst;
HWND mainWindowHandler;
HWND overlayHandler;
std::vector<std::vector<HWND> > childWindowsHandlers;
int boardSize;
int isNewGameAnimationPlaying;
int areGemsFalling;
bool debugMode;
game gameObj;

//This function can be found in "./ProcFunctions/mainProc.cpp"
void createChildWindows(HWND, std::vector<std::vector<HWND> >&, const int);

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
  hInst = hInstance;
  boardSize = 0;
  isNewGameAnimationPlaying = 0;
  areGemsFalling = 0;
  debugMode = false;

  RECT size = calculateMainWindowRect(0);

  mainWindowHandler = CreateWindowW(mainWindowClassName, applicationTitle,
      (WS_TILED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN), size.left,
      size.top, size.right - size.left, size.bottom - size.top, nullptr,
      nullptr, hInstance, nullptr);

  if (!mainWindowHandler) {
    return FALSE;
  }

  createChildWindows(mainWindowHandler, childWindowsHandlers, 0);

  overlayHandler = CreateWindowExW(
      WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST, overlayWindowClassName, //WS_EX_TOPMOST
      L"", WS_POPUP, 0,
      0, GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYFULLSCREEN),
      HWND_DESKTOP, NULL, NULL, NULL);
  SetLayeredWindowAttributes(overlayHandler, color::transparencyKey, 0,
                             LWA_COLORKEY);
  ShowWindow(overlayHandler, nCmdShow);
  UpdateWindow(overlayHandler);
  ShowWindow(mainWindowHandler, nCmdShow);
  UpdateWindow(mainWindowHandler);

  return TRUE;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  registerClasses(hInstance);

  if (!InitInstance(hInstance, nCmdShow)) {
    return FALSE;
  }

  HACCEL hAccelTable =
      LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPIBEJEWELED));

  MSG msg;
  while (GetMessage(&msg, nullptr, 0, 0)) {
    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  return (int)msg.wParam;
}