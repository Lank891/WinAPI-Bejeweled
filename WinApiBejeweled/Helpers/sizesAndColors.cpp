#include "sizesAndColors.h"

RECT calculateMainWindowRect(const int size) {
  RECT area;
  area.left = (GetSystemMetrics(SM_CXSCREEN) - clientSize[size]) / 2;
  area.top = (GetSystemMetrics(SM_CYSCREEN) - clientSize[size]) / 2;
  area.right = area.left + clientSize[size];
  area.bottom = area.top + clientSize[size];
  AdjustWindowRect(
      &area,
      (WS_TILED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX),
      TRUE);
  return area;
}

namespace color {
color uninitializedTile("#082567");
color lightBackground("#d1d1d1");
color darkBackground("#1e1e1e");
color frame("#cf2376");
color red("#2c2c2c");
color blue("#0f16e0");
color yellow("#e3af35");
color green("#96b09e");
color pink("#65161f");
color cyan("#cae00f");
color purple("#ff00ff");
color grey("#777777");
color transparencyKey("#000000");
color debug("#ff0000");
};  // namespace color
