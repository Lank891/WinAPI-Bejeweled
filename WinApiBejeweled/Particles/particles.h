#pragma once
#include <Windows.h>
#include <vector>
#include "../Helpers/sizesAndColors.h"

struct particle {
  int x;
  int y;

  double xVelocityNormalized;
  double yVelocityNormalized;

  double velocityScalar;

  color::color color;

  bool isOffScreen();

  //Returns true if particle is off screen after update
  bool updateParticle();

  particle(int _x,
           int _y,
           double _vxNormal,
           double _vyNormal,
           double _vScalar,
           color::color _color)
      : x(_x),
        y(_y),
        xVelocityNormalized(_vxNormal),
        yVelocityNormalized(_vyNormal),
        velocityScalar(_vScalar),
        color(_color){};
};

extern std::vector<particle> allParticles;

void addParticlesFromDestroyedGem(POINT, color::color);