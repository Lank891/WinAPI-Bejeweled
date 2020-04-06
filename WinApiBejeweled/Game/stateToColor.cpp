#include "game.h"

color::color stateToColor(const state tileState) {
  switch (tileState) {
    case state::blue:
      return color::blue;
    case state::yellow:
      return color::yellow;
    case state::green:
      return color::green;
    case state::red:
      return color::red;
    case state::pink:
      return color::pink;
    case state::cyan:
      return color::cyan;
    case state::purple:
      return color::purple;
    case state::grey:
      return color::grey;
    case state::notInitialized:
      return color::uninitializedTile;
    case state::empty:
    default:
      return color::lightBackground;
  }
}