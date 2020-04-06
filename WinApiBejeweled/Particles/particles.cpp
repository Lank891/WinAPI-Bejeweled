#include "particles.h"
#include "../RNG/rng.h"
#include "../Helpers/globalVariables.h"

constexpr double PI = 3.141592;

std::vector<particle> allParticles = {};

bool particle::isOffScreen() {
  return x < 0 || x > GetSystemMetrics(SM_CXFULLSCREEN) || y < 0 ||
         y > GetSystemMetrics(SM_CYFULLSCREEN);
}

bool particle::updateParticle() {
  x = x + (int)(xVelocityNormalized * velocityScalar);
  y = y + (int)(yVelocityNormalized * velocityScalar);

  return isOffScreen();
}

void addParticlesFromDestroyedGem(POINT startPoint, color::color _color) {
  for (int i = 0; i < 100; i++) {
  
    auto [_x, _y] = startPoint;
    auto angle = generateDouble(0, 2 * PI);
    auto _vxNormal = cos(angle);
    auto _vyNormal = sin(angle);
    auto _vScalar = generateDouble(12, 15);

    allParticles.emplace_back(
        particle(_x, _y, _vxNormal, _vyNormal, _vScalar, _color));
  }
}