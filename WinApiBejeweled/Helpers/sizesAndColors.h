#pragma once
#include <Windows.h>
#include <string>

constexpr int boardSizeTiles[] = {8, 10, 12, 14};
constexpr int tileSize[] = {80, 70, 60, 50};
constexpr int clientSize[] = {8 * 90, 10 * 80, 12 * 70, 14 * 60};
RECT calculateMainWindowRect(const int);


namespace color {

struct color {
  UINT8 r;
  UINT8 g;
  UINT8 b;

  color(UINT8 _r, UINT8 _g, UINT8 _b) : r(_r), g(_g), b(_b) {};

  //Assumes string in format "#xxxxxx" where x - b16 digit, no validation - throws exception
  color(std::string clr) { 
    std::string r_str = clr.substr(1, 2);
    std::string g_str = clr.substr(3, 2);
    std::string b_str = clr.substr(5, 2);

    r = (UINT8)std::stoi(r_str, NULL, 16);
    g = (UINT8)std::stoi(g_str, NULL, 16);
    b = (UINT8)std::stoi(b_str, NULL, 16);
  }
  operator COLORREF() const { return RGB(r, g, b); };
  bool operator==(const color& a) const {
    return r == a.r && g == a.g && b == a.b;
  }
};

extern color uninitializedTile;
extern color lightBackground;
extern color darkBackground;
extern color frame;
extern color red;
extern color blue;
extern color yellow;
extern color green;
extern color pink;
extern color cyan;
extern color purple;
extern color grey;
extern color transparencyKey;
extern color debug;
}