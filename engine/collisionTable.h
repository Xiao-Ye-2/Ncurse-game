#ifndef COLLISIONTABLE_H
#define COLLISIONTABLE_H

#include <map>
#include <string>
#include <vector>
#include <utility>
#include "movement.h"
#include "entity.h"
#include "enums.h"

using namespace std;

class Movement;
class Entity;


class CollisionTable {
 protected:
  bool bounce(Entity *target, Movement *move, bool hasUndone);
  bool stop(Entity *target, Movement *move, bool hasUndone);
  void destroy(Entity *target);
  void damage(Entity *target);
  void heal(Entity *target);
  
 public:
  CollisionTable();
  virtual ~CollisionTable() = default;
  virtual GameStatus collide(Entity *source, Entity *target, Movement *move, bool &hasUndone) = 0;
};


#endif
