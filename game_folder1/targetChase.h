#ifndef TARGETCHASE_H
#define TARGETCHASE_H

#include "../engine/movement.h"

class TargetChase : public Movement {
  Direction dir;
  Entity *chase;

  void doMove(Action cmd, Entity *target) override;
  void doReflectMove(Direction d) override;
  void doUnmove(Entity *target) override;

 public:
  TargetChase(Entity *chase, size_t frequency);
  ~TargetChase() = default;
};


#endif



