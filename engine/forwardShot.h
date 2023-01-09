#ifndef FORWARDSHOT_H
#define FORWARDSHOT_H

#include "movement.h"
#include "straightLine.h" 
#include "entity.h"
#include "model.h"

class Model;
class Entity;
class StraightLine;

class ForwardShot : public Movement {
  Direction dir;
  // below are info stored for the new entity;
  Action bind;
  string tag;
  Shape bulletLook;
  size_t bulletSpeed;
  

  void doMove(Action cmd, Entity *target) override;
  void doReflectMove(Direction d) override;
  void doUnmove(Entity *target) override;

 public:
  ForwardShot(Direction d, Action bind, size_t frequency, string name, Shape look, size_t speed);
  ~ForwardShot() = default;
};


#endif
