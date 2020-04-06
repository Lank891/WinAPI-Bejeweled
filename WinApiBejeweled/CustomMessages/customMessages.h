#pragma once
#include <Windows.h>

//wParam - x of clicked tile, lParam - y of clicked tile
#define WM_TILE_CLICKED (WM_USER + 0x1)
//wParam - nothing, lParam - nothing
#define WM_GEMS_EXPLODED (WM_USER + 0x2)
//wParam - nothing, lParam - nothing
#define WM_GEMS_SWAPPED (WM_USER + 0x3)