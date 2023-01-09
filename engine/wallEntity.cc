#include "wallEntity.h"

WallEntity::WallEntity(string tag, Coordinate pos, Shape look, size_t height, Model *owner) :
  Entity{std::move(tag), std::move(pos), std::move(look), height, owner}, isKilled{false} {}

void WallEntity::reflectMove() {
  const auto& l = getMove();
  int n = l.size();
  for (int i = 0; i < n; ++i) {
    l[i]->reflectMove(getDir());
  }
}

void WallEntity::damage() {}
bool WallEntity::heal() { return false; }


void WallEntity::kill() { isKilled = true; }
// Direction& WallEntity::getDir() { 
//   Direction x = Direction::UP;
//   return x; 
// }
bool WallEntity::isDead() { return isKilled == true; }