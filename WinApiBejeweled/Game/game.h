#pragma once
#include <vector>
#include <Windows.h>
#include "../Helpers/sizesAndColors.h"

enum class state {
  notInitialized = -1,
  empty = 0,
  red = 1,
  blue = 2,
  yellow = 3,
  green = 4,
  pink = 5,
  cyan = 6,
  purple = 7,
  grey = 8
};

color::color stateToColor(const state tileState);

struct game {
  int size;
  std::vector<std::vector<state> > tileStates;
  std::vector<std::vector<bool> > checkExplode;
  bool isPlayed = false;
  
  state generateTile();

  void resetGame(const int);
  void cleanTiles();
  void resetExplode();

  state& getTile(int, int);

  void swapTiles(int, int, int, int);
  bool forceExplodeTiles();
  bool checkIfExploded();

  bool isFull();
};