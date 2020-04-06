#include "register.h"

#include "../Helpers/strings.h"
#include "../ProcFunctions/procFunctions.h"
#include "../Resource.h"

ATOM registerMainWindow(HINSTANCE hInstance) {
  WNDCLASSEXW wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = mainProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPIBEJEWELED));
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = NULL;
  wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINAPIBEJEWELED);
  wcex.lpszClassName = mainWindowClassName;
  wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

  return RegisterClassExW(&wcex);
}

ATOM registerChildWindow(HINSTANCE hInstance) {
  WNDCLASSEXW wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = childProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = NULL;
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = NULL;
  wcex.lpszMenuName = NULL;
  wcex.lpszClassName = childWindowClassName;
  wcex.hIconSm = NULL;

  return RegisterClassExW(&wcex);
}

ATOM registerOverlayWindow(HINSTANCE hInstance) {
  WNDCLASSEXW wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = overlayProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = NULL;
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = NULL;
  wcex.lpszMenuName = NULL;
  wcex.lpszClassName = overlayWindowClassName;
  wcex.hIconSm = NULL;

  return RegisterClassExW(&wcex);
}

void registerClasses(HINSTANCE hInstance) {
  registerMainWindow(hInstance);
  registerChildWindow(hInstance);
  registerOverlayWindow(hInstance);
}