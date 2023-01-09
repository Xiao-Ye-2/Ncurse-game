#include "straightLine.h"
#include <iostream>

StraightLine::StraightLine(Direction dir, size_t frequency) : Movement{0, frequency, "StraightLine"}, dir{dir} {}

void StraightLine::doMove(Action cmd, Entity *target) {
  incCount(1);
  if (canMove()) {
    if (dir == Direction::UP || dir == Direction::DOWN || dir == Direction::LEFT || dir == Direction::RIGHT) {
      target->getDir() = dir;
    }
    basicMove(target, dir);
  }
}

void StraightLine::doReflectMove(Direction d) {
  if (dir == Direction::UP && d == Direction::UP) {
    dir = Direction::DOWN;
  } else if (dir == Direction::LEFT && d == Direction::LEFT) {
    dir = Direction::RIGHT;
  } else if (dir == Direction::RIGHT && d == Direction::RIGHT) {
    dir = Direction::LEFT;
  } else if (dir == Direction::DOWN && d == Direction::DOWN) {
    dir = Direction::UP;
  }
}

void StraightLine::doUnmove(Entity *target) {
  basicReverseMove(target, dir);
}


