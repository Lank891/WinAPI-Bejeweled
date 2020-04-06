#include "rng.h"
#include <random>
#include <Windows.h>
#include "../Helpers/globalVariables.h"

std::random_device rd;
std::mt19937 gen(rd());

int generateInt(int lowerBoundClosed, int upperBoundClosed) {
  std::uniform_int_distribution<> dis(lowerBoundClosed, upperBoundClosed);
  return dis(gen);
}

double generateDouble(double lowerBoundClosed, double upperBoundOpen) {
  std::uniform_real_distribution<double> dis(lowerBoundClosed, upperBoundOpen);
  return dis(gen);
}