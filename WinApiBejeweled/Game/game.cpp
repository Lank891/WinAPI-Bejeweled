#include "game.h"
#include "../RNG/rng.h"
#include "../Helpers/globalVariables.h"
#include "../CustomMessages/customMessages.h"
#include "../Particles/particles.h"

state game::generateTile() {
  if (size <= 8)
    return static_cast<state>(generateInt(1, 4));
  if (size <= 10)
    return static_cast<state>(generateInt(1, 5));
  if (size <= 12)
    return static_cast<state>(generateInt(1, 6));
  return static_cast<state>(generateInt(1, 8));
}

void game::cleanTiles() {
  for (auto& row : tileStates)
    row.clear();
  tileStates.clear();
  for (auto& row : checkExplode)
    row.clear();
  checkExplode.clear();
}

void game::resetGame(const int newSize) {
  cleanTiles();
  size = newSize;
  for (int i = 0; i < newSize; i++) {
    std::vector<state> row;
    std::vector<bool> chRow;
    for (int j = 0; j < newSize; j++) {
      row.emplace_back(generateTile());
      chRow.emplace_back(false);
    }
    tileStates.emplace_back(row);
    checkExplode.emplace_back(chRow);
  }
  isPlayed = true;
}

void game::resetExplode() {
  for (auto& row : checkExplode)
    for (unsigned int i = 0; i < row.size(); i++)
      row.at(i) = false;
}

state& game::getTile(int x, int y) {
  return tileStates.at(x).at(y);
}

void game::swapTiles(int x1, int y1, int x2, int y2) {
  //Invalid places
  if (x1 < 0 || x1 >= size || y1 < 0 || y1 >= size || x2 < 0 || x2 >= size ||
      y2 < 0 || y2 >= size)
    return;

  //Distances between tiles
  int xDist = abs(x1 - x2);
  int yDist = abs(y1 - y2);

  //Not neighbours
  if (abs(x1 - x2) > 1 || abs(y1 - y2) > 1)
    return;
  //Diagonal neighbours
  if (xDist == 1 && yDist == 1)
    return;

  std::swap(getTile(x1, y1), getTile(x2, y2));
  SendMessageW(mainWindowHandler, WM_GEMS_SWAPPED, 0, 0);

  //Force explode, if nothing exploed swap tiles back
   if(!forceExplodeTiles()) {
     std::swap(getTile(x1, y1), getTile(x2, y2));
     SendMessageW(mainWindowHandler, WM_GEMS_SWAPPED, 0, 0);
  }
}

bool game::forceExplodeTiles() {
  // Check if some tiles should explode, if yes empty tiles and reset vector
  bool didSomethingExploded = checkIfExploded();
  if (didSomethingExploded) {
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        if (checkExplode.at(i).at(j)) {
          //Creating particles
          RECT windowPosition;
          GetClientRect(childWindowsHandlers.at(i).at(j), &windowPosition);
          POINT explodePosition = {
              windowPosition.right / 2,
              windowPosition.bottom / 2};
          ClientToScreen(childWindowsHandlers.at(i).at(j), &explodePosition);
          addParticlesFromDestroyedGem(explodePosition,
                                       stateToColor(getTile(i, j)));
          //Change tile to empty
          getTile(i, j) = state::empty;
        }
      }
    }
    resetExplode();
    SendMessageW(mainWindowHandler, WM_GEMS_EXPLODED, 0, 0);
  }
  //Return if something exploded or not
  return didSomethingExploded;
}

bool game::checkIfExploded() {
  bool didSthExploded = false;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      state currentTileSate = getTile(i, j);
      //It is not actual color - continue
      if (currentTileSate == state::notInitialized ||
          currentTileSate == state::empty) {
        continue;
      }
      //In a row (i cannot be 0 and size-1)
      if (i != 0 && i != (size - 1) &&
          getTile(i - 1, j) == currentTileSate &&
          getTile(i + 1, j) == currentTileSate) {
        checkExplode.at(i - 1).at(j) = checkExplode.at(i).at(j) =
            checkExplode.at(i + 1).at(j) = true;
        didSthExploded = true;
      }
      //In a column (j cannot be 0 and size-1)
      if (j != 0 && j != (size - 1) &&
          getTile(i, j - 1) == currentTileSate &&
          getTile(i, j + 1) == currentTileSate) {
        checkExplode.at(i).at(j - 1) = checkExplode.at(i).at(j) =
            checkExplode.at(i).at(j + 1) = true;
        didSthExploded = true;
      }
    }
  }
  return didSthExploded;
}

bool game::isFull() {
  for (int x = 0; x < size; x++)
    for (int y = 0; y < size; y++)
      if (getTile(x, y) == state::empty)
        return false;
  return true;
}