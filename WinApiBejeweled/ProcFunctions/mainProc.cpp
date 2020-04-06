#include "../Helpers/globalVariables.h"
#include "../Resource.h"
#include "../CustomMessages/customMessages.h"
#include "../Helpers/sizesAndColors.h"
#include "../Helpers/strings.h"
#include "procFunctions.h"

void checkSizeMenu(HWND, const int);
void resizeWindow(HWND, const int);
void createChildWindows(HWND, std::vector<std::vector<HWND> >&, const int);
void deleteChidWindows(HWND, std::vector<std::vector<HWND> >&);
void CALLBACK timerAnimateNewGame(HWND, UINT, UINT_PTR, DWORD);
void CALLBACK timerAnimateFalling(HWND, UINT, UINT_PTR, DWORD);
std::pair<HWND, tileInfo*> getChildAndItsInfo(const int row, const int column);
void updateChildren(bool = true, bool = true);

//This variables can be found in "./childProc.cpp"
extern bool isMouseTracked;

POINT clickedTile{-1, -1};

LRESULT CALLBACK mainProc(HWND hWnd,
                          UINT message,
                          WPARAM wParam,
                          LPARAM lParam) {
  switch (message) {
    case WM_COMMAND: {
      int wmId = LOWORD(wParam);
      switch (wmId) {
        case IDM_SMALL:
          if (isNewGameAnimationPlaying || areGemsFalling)
            break;
          checkSizeMenu(hWnd, 0);
          resizeWindow(hWnd, 0);
          break;
        case IDM_MEDIUM:
          if (isNewGameAnimationPlaying || areGemsFalling)
            break;
          checkSizeMenu(hWnd, 1);
          resizeWindow(hWnd, 1);
          break;
        case IDM_BIG:
          if (isNewGameAnimationPlaying || areGemsFalling)
            break;
          checkSizeMenu(hWnd, 2);
          resizeWindow(hWnd, 2);
          break;
        case IDM_VERYBIG:
          if (isNewGameAnimationPlaying || areGemsFalling)
            break;
          checkSizeMenu(hWnd, 3);
          resizeWindow(hWnd, 3);
          break;
        case IDM_DEBUG: {
          HMENU helpMenu = GetSubMenu(GetMenu(hWnd), 1);
          MENUITEMINFOW mii;
          mii.cbSize = sizeof(MENUITEMINFO);
          mii.fMask = MIIM_STATE;
          GetMenuItemInfoW(helpMenu, 1, TRUE, &mii);
          if (mii.fState & MFS_CHECKED) {
            debugMode = false;
            CheckMenuItem(helpMenu, 1, MF_BYPOSITION | MF_UNCHECKED);
          } else {
            debugMode = true;
            CheckMenuItem(helpMenu, 1, MF_BYPOSITION | MF_CHECKED);
          }
        } break;
        case IDM_NEWGAME: {
          if (isNewGameAnimationPlaying || areGemsFalling)
            break;

          //Change colors of all tiles to default one
          for (int i = 0; i < boardSizeTiles[boardSize]; i++) {
            for (int j = 0; j < boardSizeTiles[boardSize]; j++) {
              auto [childHandler, info] = getChildAndItsInfo(i, j);
              info->tileState = state::notInitialized;
              info->clicked = false;
              InvalidateRect(childHandler, NULL, TRUE);
            }
          }
          isNewGameAnimationPlaying =
              boardSizeTiles[boardSize] * boardSizeTiles[boardSize];
          SetTimer(hWnd, NULL, 20, timerAnimateNewGame);
        } break;
        case IDM_ABOUT:
          DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, aboutProc);
          break;
        case IDM_EXIT:
          DestroyWindow(hWnd);
          break;
        default:
          return DefWindowProcW(hWnd, message, wParam, lParam);
      }
    } break;
    case WM_TILE_CLICKED: {
      POINT tile = {(LONG)wParam, (LONG)lParam};
      if (clickedTile.x == -1) {
        clickedTile = tile;
      } else {
        gameObj.swapTiles(clickedTile.x, clickedTile.y, tile.x, tile.y);
        clickedTile = {-1, -1};
        updateChildren();
      }
    } break;
    case WM_GEMS_EXPLODED: {
      areGemsFalling = 1;
      InvalidateRect(hWnd, NULL, TRUE);
      updateChildren();
      SetTimer(hWnd, NULL, 500, timerAnimateFalling);
    } break;
    case WM_GEMS_SWAPPED: {
      updateChildren();
    } break;
    case WM_SIZE: {
      if (wParam == SIZE_MINIMIZED) {
        ShowWindow(overlayHandler, SW_HIDE);
      } else {
        ShowWindow(overlayHandler, SW_SHOW);
      }
    } break;
    /*case WM_ACTIVATE: {
      if (wParam == WA_ACTIVE || wParam == WA_CLICKACTIVE) {
        SetWindowPos(overlayHandler, HWND_TOPMOST, 0, 0, 0, 0,
                     SWP_NOSIZE | SWP_NOMOVE);
        SetWindowTextW(hWnd, L"xdd");
      } else {
        SetWindowTextW(hWnd, L"XDD");
        SetWindowPos(overlayHandler, HWND_TOP, 0, 0, 0, 0,
                     SWP_NOSIZE | SWP_NOMOVE);
      }
    } break;*/
    /*case WM_MOVE: {
      SetWindowPos(overlayHandler, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE);
      SetWindowPos(mainWindowHandler, overlayHandler, 0, 0, 0, 0,
                   SWP_NOSIZE | SWP_NOMOVE);
    } break; */
    /*case WM_WINDOWPOSCHANGED: {
      SetWindowPos(overlayHandler, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE);
      return DefWindowProcW(hWnd, message, wParam, lParam);
    } break;*/
    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hWnd, &ps);
      EndPaint(hWnd, &ps);
    } break;
    case WM_ERASEBKGND: {
      HBRUSH brush;
      HDC hDC = (HDC)wParam;
      RECT client;
      GetClientRect(hWnd, &client);
      brush = areGemsFalling ? CreateSolidBrush(color::darkBackground)
                             : CreateSolidBrush(color::lightBackground);
      FillRect(hDC, &client, brush);
      DeleteObject(brush);
    } break;
    case WM_DESTROY: {
      PostQuitMessage(0);
    } break;
    default:
      return DefWindowProcW(hWnd, message, wParam, lParam);
  }
  return 0;
}

void checkSizeMenu(HWND hWnd, const int sizeIndex) {
  HMENU sizeMenu = GetSubMenu(GetSubMenu(GetMenu(hWnd), 0), 2);
  for (int i = 0; i < 4; i++) {
    CheckMenuItem(sizeMenu, i,
                  MF_BYPOSITION | (i == sizeIndex ? MF_CHECKED : MF_UNCHECKED));
  }
}

void resizeWindow(HWND hWnd, const int size) {
  boardSize = size;
  RECT sizeRect = calculateMainWindowRect(size);
  gameObj.cleanTiles();
  gameObj.isPlayed = false;
  MoveWindow(hWnd, sizeRect.left, sizeRect.top, sizeRect.right - sizeRect.left,
             sizeRect.bottom - sizeRect.top, TRUE);
  deleteChidWindows(hWnd, childWindowsHandlers);
  createChildWindows(hWnd, childWindowsHandlers, size);
}

void deleteChidWindows(HWND hWnd, std::vector<std::vector<HWND> >& chldHwnd) {
  for (auto& row : chldHwnd) {
    for (auto& item : row) {
      DestroyWindow(item);
    }
    row.clear();
  }
  chldHwnd.clear();
}

void createChildWindows(HWND hWnd, std::vector<std::vector<HWND> > &chldHwnd, const int size) {
  for (int i = 0; i < boardSizeTiles[size]; i++) {
    std::vector<HWND> row;
    for (int j = 0; j < boardSizeTiles[size]; j++) {
      row.emplace_back(CreateWindowW(
          childWindowClassName, L"",
          WS_CHILD |  WS_VISIBLE,
          5 + i * (tileSize[size] + 10), 5 + j * (tileSize[size] + 10),
          tileSize[size], tileSize[size], hWnd, NULL, hInst, NULL));
    }
    chldHwnd.emplace_back(row);
  }
  isMouseTracked = false;
}

void CALLBACK timerAnimateNewGame(HWND hWnd, UINT message, UINT_PTR timerId, DWORD msCount) {
  int tiles = boardSizeTiles[boardSize];
  int childrenNumber = tiles * tiles - isNewGameAnimationPlaying;

  if (childrenNumber == 0)
    gameObj.resetGame(boardSizeTiles[boardSize]);

  int y = childrenNumber / tiles;
  int x = childrenNumber % tiles;

  auto [childHandler, info] = getChildAndItsInfo(x, y);
  info->tileState = gameObj.getTile(x, y);
  info->lastColor = info->tileState;
  InvalidateRect(childHandler, NULL, TRUE);

  isNewGameAnimationPlaying--;
  if (isNewGameAnimationPlaying == 0) {
    KillTimer(hWnd, timerId);
    gameObj.forceExplodeTiles(); //Check for instant explosions
    updateChildren(); //Update children so exploded things will be indeed exploded
  }
  
}

void CALLBACK timerAnimateFalling(HWND hWnd, UINT message, UINT_PTR timerId, DWORD msCount) {

  for (int x = 0; x < gameObj.size; x++) {
    for (int y = gameObj.size - 2; y >= 0; y--) {
      if (gameObj.getTile(x, y + 1) == state::empty) {
        std::swap(gameObj.getTile(x, y + 1), gameObj.getTile(x, y));
      }
    }
    if (gameObj.getTile(x, 0) == state::empty) {
      gameObj.getTile(x, 0) = gameObj.generateTile();
    }
  }

  updateChildren();

  //If everything exploded we want to play one more animation frame
  if (gameObj.isFull() && areGemsFalling == 1) {
    areGemsFalling = 2;
  } else if (gameObj.isFull() && areGemsFalling == 2) {
    KillTimer(hWnd, timerId);
    areGemsFalling = 0;
    InvalidateRect(hWnd, NULL, TRUE);
    gameObj.forceExplodeTiles();
  }
}

std::pair<HWND, tileInfo*> getChildAndItsInfo(const int row, const int column) {
  HWND childHandler = childWindowsHandlers.at(row).at(column);
  tileInfo* info = (tileInfo*)GetWindowLongPtrW(childHandler, GWLP_USERDATA);
  return std::make_pair(childHandler, info);
}

void updateChildren(bool resetClicks, bool resetColors) {
  int size = boardSizeTiles[boardSize];

  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      auto [hWnd, info] = getChildAndItsInfo(i, j);
      if (resetColors) {
        info->tileState = gameObj.getTile(i, j);
        if (info->tileState != state::empty &&
            info->tileState != state::notInitialized) {
          info->lastColor = info->tileState;
        }
      } 
      if (resetClicks)
        info->clicked = false;
      InvalidateRect(hWnd, NULL, TRUE);
    }
  }
}