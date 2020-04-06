#include "../Helpers/globalVariables.h"
#include "../Helpers/sizesAndColors.h"
#include "../CustomMessages/customMessages.h"
#include "../Resource.h"
#include "procFunctions.h"
#include <windowsx.h>

void makeBigger(HWND);
BOOL makeSmaller(HWND);
void trackMouse(HWND);
RECT childSizeInMainClient(HWND);
POINT getTileUnderMouseCoords(const int);

bool isMouseTracked;

LRESULT CALLBACK childProc(HWND hWnd,
                          UINT message,
                          WPARAM wParam,
                          LPARAM lParam) {
  switch (message) {
    case WM_ERASEBKGND:
      return 1;
    case WM_CREATE: {
      SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LONG)(new tileInfo));
    } break;
    case WM_DESTROY: {
      tileInfo* info = (tileInfo*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
      delete info;
    } break;
    case WM_MOUSEMOVE: {
      POINT chosenTile = getTileUnderMouseCoords(boardSize);
      if (isNewGameAnimationPlaying || areGemsFalling)
        break;

      if (!isMouseTracked) {
        
        isMouseTracked = true;
        makeBigger(hWnd);
        trackMouse(hWnd);
      }
    } break;
    case WM_MOUSEHOVER: {
      RECT size = childSizeInMainClient(hWnd);
      if (size.right - size.left == tileSize[boardSize]) { //&& !isMouseTracked) {
        isMouseTracked = true;
        makeBigger(hWnd);
        trackMouse(hWnd);
      } else {
        trackMouse(hWnd);
      }
    } break;
    case WM_MOUSELEAVE: {
      SetTimer(hWnd, NULL, 50, NULL);
      isMouseTracked = false;
    } break;
    case WM_LBUTTONDOWN: {
      if (isNewGameAnimationPlaying || areGemsFalling || !gameObj.isPlayed)
        break;
      tileInfo* info = (tileInfo*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
      info->clicked = !info->clicked;
      InvalidateRect(hWnd, NULL, TRUE);
      POINT chosenTile = getTileUnderMouseCoords(boardSize);
      SendMessageW(mainWindowHandler, WM_TILE_CLICKED, chosenTile.x,
                   chosenTile.y);
    } break;
    case WM_TIMER: {
      if (makeSmaller(hWnd))
        KillTimer(hWnd, wParam);
    } break;
    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hWnd, &ps);

      RECT colorRect;
      RECT frameRect;
      GetClientRect(hWnd, &frameRect);
      int windowSize = frameRect.right - frameRect.left;
      colorRect = frameRect;
      tileInfo* info = (tileInfo*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
      if (info->clicked) {
        colorRect.left += 4;
        colorRect.top += 4;
        colorRect.right -= 4;
        colorRect.bottom -= 4;
      }

      HDC offDc = CreateCompatibleDC(hdc);  // Offscreen context
      HBITMAP offBmp = CreateCompatibleBitmap(hdc, windowSize,
                                              windowSize);  // Offscreen bitmap
      HBITMAP offOldBmp = (HBITMAP)SelectObject(offDc, offBmp);  // Old bitmap

      HBRUSH frameBrush = CreateSolidBrush(color::frame);
      HBRUSH backgroundBrush = areGemsFalling
                                   ? CreateSolidBrush(color::darkBackground)
                                   : CreateSolidBrush(color::lightBackground);
      HBRUSH backBrush = CreateSolidBrush(stateToColor(info->tileState));

      HPEN crossPen = CreatePen(PS_SOLID, 1, stateToColor(info->lastColor));
      HPEN oldPen = (HPEN)SelectObject(offDc, crossPen);

      //Draw frame and color (if tile is clicked color will be smaller to reveal frame)
      FillRect(offDc, &frameRect, frameBrush);
      FillRect(offDc, &colorRect, backBrush);

      //If gems are falling and tile is bigger then normaly draw a frame with background color around it
      //So it looks like it has normal size
      int sizeDifference = (windowSize - tileSize[boardSize]) / 2;
      if (sizeDifference != 0 && areGemsFalling) {
        RECT l = {0, 0, sizeDifference, windowSize};
        RECT t = {0, 0, windowSize, sizeDifference};
        RECT r = {windowSize - sizeDifference, 0, windowSize, windowSize};
        RECT b = {0, windowSize - sizeDifference, windowSize, windowSize};
        FillRect(offDc, &l, backgroundBrush);
        FillRect(offDc, &t, backgroundBrush);
        FillRect(offDc, &r, backgroundBrush);
        FillRect(offDc, &b, backgroundBrush);      
      }

      //Draw crosses (and think about when tile is bigger, so crosses will always look normal
      if (info->tileState == state::empty) {
        for (int i = 10 + sizeDifference; i < windowSize - sizeDifference; i += 10) {
          MoveToEx(offDc, i, sizeDifference, NULL);
          LineTo(offDc, i, windowSize - sizeDifference);
          MoveToEx(offDc, sizeDifference, i, NULL);
          LineTo(offDc, windowSize - sizeDifference, i);
        }
      }

      BitBlt(hdc, 0, 0, windowSize, windowSize, offDc, 0, 0, SRCCOPY);

      SelectObject(offDc, offOldBmp);
      SelectObject(offDc, oldPen);
      DeleteDC(offDc);
      DeleteObject(offBmp);
      DeleteObject(frameBrush);
      DeleteObject(backgroundBrush);
      DeleteObject(backBrush);
      DeleteObject(crossPen);
      EndPaint(hWnd, &ps);
    } break;
    default:
      return DefWindowProcW(hWnd, message, wParam, lParam);
  }
  return 0;
}

void makeBigger(HWND hWnd) {
  RECT size = childSizeInMainClient(hWnd);
  if ((size.right - size.left) > tileSize[boardSize])
    return;
  size.top -= 4;
  size.bottom += 4;
  size.left -= 4;
  size.right += 4;
  MoveWindow(hWnd, size.left, size.top, size.right - size.left,
             size.bottom - size.top, TRUE);
}

BOOL makeSmaller(HWND hWnd) {
  RECT size = childSizeInMainClient(hWnd);
  size.top += 1;
  size.bottom -= 1;
  size.left += 1;
  size.right -= 1;
  MoveWindow(hWnd, size.left, size.top, size.right - size.left,
             size.bottom - size.top, TRUE);
  return (size.right - size.left) <= tileSize[boardSize];
}

void trackMouse(HWND hWnd) {
  TRACKMOUSEEVENT tme;
  tme.cbSize = sizeof(TRACKMOUSEEVENT);
  tme.dwFlags = TME_LEAVE | TME_HOVER;
  tme.dwHoverTime = 1;
  tme.hwndTrack = hWnd;
  TrackMouseEvent(&tme);
}

RECT childSizeInMainClient(HWND child) {
  RECT size;
  GetWindowRect(child, &size);
  POINT lt = {size.left, size.top};
  POINT rb = {size.right, size.bottom};
  ScreenToClient(mainWindowHandler, &lt);
  ScreenToClient(mainWindowHandler, &rb);
  size.left = lt.x;
  size.top = lt.y;
  size.right = rb.x;
  size.bottom = rb.y;
  return size;
}

POINT getTileUnderMouseCoords(const int size) {
  POINT p;
  GetCursorPos(&p);
  ScreenToClient(mainWindowHandler, &p);

  p.x /= (tileSize[size] + 10);
  p.y /= (tileSize[size] + 10);

  if (p.x < 0 || p.y < 0 || p.x >= boardSizeTiles[size] ||
      p.y >= boardSizeTiles[size])
    p.x = p.y = -1;

  return p;
}