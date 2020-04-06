#include "../Helpers/globalVariables.h"
#include "../Resource.h"
#include "../CustomMessages/customMessages.h"
#include "../Helpers/sizesAndColors.h"
#include "../Helpers/strings.h"
#include "../Particles/particles.h"
#include "procFunctions.h"
LRESULT CALLBACK overlayProc(HWND hWnd,
                          UINT message,
                          WPARAM wParam,
                          LPARAM lParam) {
  switch (message) {
    case WM_CREATE: {
      SetTimer(hWnd, NULL, 25, NULL);
    } break;
    case WM_TIMER: {
      auto end = std::remove_if(allParticles.begin(), allParticles.end(),
                                [](particle& p) { return p.updateParticle(); });
      allParticles.erase(end, allParticles.end());
      InvalidateRect(hWnd, NULL, TRUE);
    } break;
    case WM_ERASEBKGND: {
      return 1;
    } break;
    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hWnd, &ps);

      HDC offDc = CreateCompatibleDC(hdc);  // Offscreen context
      HBITMAP offBmp =
          CreateCompatibleBitmap(hdc, GetSystemMetrics(SM_CXFULLSCREEN),
          GetSystemMetrics(SM_CYFULLSCREEN));  // Offscreen bitmap
      HBITMAP offOldBmp = (HBITMAP)SelectObject(offDc, offBmp);  // Old bitmap
      
      HBRUSH blueBrush = CreateSolidBrush(color::blue);
      HBRUSH yellowBrush = CreateSolidBrush(color::yellow);
      HBRUSH greenBrush = CreateSolidBrush(color::green);
      HBRUSH redBrush = CreateSolidBrush(color::red);
      HBRUSH pinkBrush = CreateSolidBrush(color::pink);
      HBRUSH cyanBrush = CreateSolidBrush(color::cyan);
      HBRUSH transparentBrush = CreateSolidBrush(color::transparencyKey);

      RECT bitmapRect = {0, 0, GetSystemMetrics(SM_CXFULLSCREEN),
                               GetSystemMetrics(SM_CYFULLSCREEN)};
      FillRect(offDc, &bitmapRect, transparentBrush);

      for (auto item : allParticles) {
        RECT square = {item.x - 4, item.y - 4, item.x + 4, item.y + 4};
        
        if (item.color == color::blue)
          FillRect(offDc, &square, blueBrush);
        else if (item.color == color::yellow)
          FillRect(offDc, &square, yellowBrush);
        else if (item.color == color::green)
          FillRect(offDc, &square, greenBrush);
        else if (item.color == color::red)
          FillRect(offDc, &square, redBrush);
        else if (item.color == color::pink)
          FillRect(offDc, &square, pinkBrush);
        else
          FillRect(offDc, &square, cyanBrush);
      }

      if (debugMode) {
        HFONT font =
            CreateFontW(90, 40, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                        ANSI_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DECORATIVE, NULL);
        HFONT oldFont = (HFONT)SelectObject(offDc, font);

        SetBkMode(offDc, TRANSPARENT);
        SetTextColor(offDc, color::debug);
        wchar_t textValue[256];
        swprintf_s(textValue, 256, L"Particles: %d", allParticles.size());

        DrawTextW(offDc, textValue, (int)wcslen(textValue), &bitmapRect,
                  DT_SINGLELINE | DT_TOP | DT_CENTER | DT_NOCLIP);

        SelectObject(offDc, oldFont);
        DeleteObject(font);
      }

      BitBlt(hdc, 0, 0, GetSystemMetrics(SM_CXFULLSCREEN),
             GetSystemMetrics(SM_CYFULLSCREEN), offDc, 0, 0, SRCCOPY);

      DeleteObject(transparentBrush);
      DeleteObject(cyanBrush);
      DeleteObject(pinkBrush);
      DeleteObject(redBrush);
      DeleteObject(greenBrush);
      DeleteObject(yellowBrush);
      DeleteObject(blueBrush);
      SelectObject(offDc, offOldBmp);
      DeleteObject(offBmp);
      DeleteDC(offDc);
      EndPaint(hWnd, &ps);
    } break;
    default:
      return DefWindowProcW(hWnd, message, wParam, lParam);
  }
  return 0;
}