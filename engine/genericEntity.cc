#include "genericEntity.h"

// GenericEntity::GenericEntity(string tag, size_t h, Coordinate pos, Shape look, size_t height, Model *owner) :
//   Entity{std::move(tag), std::move(pos), std::move(look), height, owner}, hp{int(h)}, maxHp{int(h)}, dir{Direction::UP} {}


GenericEntity::GenericEntity(string tag, size_t h, Coordinate pos, Shape look, size_t height, Model *owner) :
  Entity{std::move(tag), std::move(pos), std::move(look), height, owner}, hp{int(h)}, maxHp{int(h)}, vulnerable{true} {}
  
void GenericEntity::reflectMove() {
  const auto& l = getMove();
  int n = l.size();
  for (int i = 0; i < n; ++i) {
    l[i]->reflectMove(getDir());
  }
}

void GenericEntity::damage() { if (vulnerable) {--hp;} }

bool GenericEntity::heal() {
  if (hp == maxHp) { return false; }
  else {
    ++hp;
    return true;
  }
}

bool GenericEntity::getVulnerable() { return vulnerable; };
void GenericEntity::setVulnerable(bool newState) { vulnerable = newState; }

// void GenericEntity::setHp(int newHp) {
//   if (newHp > maxHp) { return; }
//   else { hp = newHp;}
// }
int GenericEntity::getHp() const { return hp; }
int GenericEntity::getMaxHp() const { return maxHp; }
void GenericEntity::kill() { hp = 0; }
// Direction GenericEntity::getDir() { return dir; }
// void GenericEntity::setDir(Direction d) { dir = d; }

bool GenericEntity::isDead() { return hp <= 0; }