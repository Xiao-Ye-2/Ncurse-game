#ifndef RANDWALK_H
#define RANDWALK_H

#include "../engine/movement.h"
#include "../engine/enums-user.h"

class RandWalk : public Movement {
  Direction dir;
  size_t numProceed;
  size_t changeAfter;
  size_t probability;

  void doMove(Action cmd, Entity *target) override;
  void doReflectMove(Direction d) override;
  void doUnmove(Entity *target) override;

 public:
  RandWalk(size_t frequency, size_t changeAfter, size_t probability);
  ~RandWalk() = default;
};


#endif


