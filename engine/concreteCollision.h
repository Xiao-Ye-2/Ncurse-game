#ifndef CONCRETECOLLISION_H
#define CONCRETECOLLISION_H

#include "collisionTable.h"

class ConcreteCollision : public CollisionTable {
  map<pair<string, string>, pair<vector<CollideType>, vector<CollideType>>> table;
  
 public:
  ConcreteCollision(map<pair<string, string>, pair<vector<CollideType>, vector<CollideType>>> t);
  ~ConcreteCollision() = default;
  GameStatus collide(Entity *source, Entity *target, Movement *move, bool &hasUndone) override;
};



#endif

