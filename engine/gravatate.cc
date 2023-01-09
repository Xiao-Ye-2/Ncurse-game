#include "gravatate.h"

Gravatate::Gravatate(Direction dir, size_t frequency) : Movement{0, frequency, "Gravatate"}, dir{dir} {}

void Gravatate::doMove(Action cmd, Entity *target) {
  incCount(1);
  if (canMove()) {
    if (dir == Direction::UP || dir == Direction::DOWN || dir == Direction::LEFT || dir == Direction::RIGHT) {
      target->getDir() = dir;
    }
    basicMove(target, dir);
  }
}

void Gravatate::doReflectMove(Direction d) {}

void Gravatate::doUnmove(Entity *target) {
  basicReverseMove(target, dir);
}


