#ifndef PLAYERCONTROL_H
#define PLAYERCONTROL_H

#include "movement.h"

class PlayerControl : public Movement {
  Action lastMove;

  void doMove(Action cmd, Entity *target) override;
  void doReflectMove(Direction dir) override;
  void doUnmove(Entity *target) override;
 public:
  PlayerControl();
  ~PlayerControl() = default;
};


#endif
