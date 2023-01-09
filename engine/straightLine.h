#ifndef STRAIGHTLINE_H
#define STRAIGHTLINE_H

#include "movement.h"

class StraightLine : public Movement {
  Direction dir;
   
  void doMove(Action cmd, Entity *target) override;
  void doReflectMove(Direction d) override;
  void doUnmove(Entity *target) override;

 public:
  StraightLine(Direction dir, size_t frequency);
  ~StraightLine() = default;
};




#endif

