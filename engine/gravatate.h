#ifndef GRAVATATE_H
#define GRAVATATE_H

#include "movement.h"

class Gravatate : public Movement {
  Direction dir;
   
  void doMove(Action cmd, Entity *target) override;
  void doReflectMove(Direction d) override;
  void doUnmove(Entity *target) override;

 public:
  Gravatate(Direction dir, size_t frequency);
  ~Gravatate() = default;
};






#endif

