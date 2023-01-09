#include "collisionTable.h"
#include <iostream>


CollisionTable::CollisionTable() {}

bool CollisionTable::bounce(Entity *target, Movement *move, bool hasUndone) {
  if (!hasUndone) {
    move->unmove(target);
  }
  target->reflectMove();
  return true;
}

bool CollisionTable::stop(Entity *target, Movement *move, bool hasUndone) {
  if (!hasUndone) {
    move->unmove(target);
  }
  return true;
}

void CollisionTable::destroy(Entity *target) {
  target->kill();
}

void CollisionTable::damage(Entity *target) {
  target->damage();
}

void CollisionTable::heal(Entity *target) {
  target->heal();
}