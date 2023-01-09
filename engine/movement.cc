#include "movement.h"
#include <iostream>


Movement::Movement(size_t c, size_t f, const std::string& t) : count{c}, frequency{f}, tag{t} {}



void Movement::incCount(size_t n) {
  count += n;
}

size_t Movement::getFrequency() {
  return frequency;
}

// void Movement::setFrequency(size_t newFrequency) {
//   frequency = newFrequency;
// }

bool Movement::canMove() {
  if (count < frequency) {
    return false;
  } else {
    count -= frequency;
    return true;
  }
}

void Movement::incFrequency(size_t n) {
  if (frequency <= n) {
    frequency = 0;
  } else {
    frequency -= n;
  }
}

void Movement::basicMove(Entity *target, Direction d) {
  if (d == Direction::UP) {
        target->getPos().decY();
    } else if (d == Direction::DOWN) {
        target->getPos().incY();
    } else if (d == Direction::LEFT) {
        target->getPos().decX();
    } else if (d == Direction::RIGHT) {
        target->getPos().incX();
    } else {
        // open for other moves
        // or create a new subclass of move
        return;
    }
}

void Movement::basicReverseMove(Entity *target, Direction d) {
  if (d ==  Direction::UP) {
    target->getPos().incY();
  } else if (d == Direction::DOWN) {
    target->getPos().decY();
  } else if (d == Direction::LEFT) {
    target->getPos().incX();
  } else if (d == Direction::RIGHT ) {
    target->getPos().decX();
  } else {
    return;
  }
}

void Movement::move(Action cmd, Entity *target) {
  doMove(cmd, target);
}

void Movement::reflectMove(Direction dir) {
  doReflectMove(dir);
}

void Movement::unmove(Entity *target) {
  doUnmove(target);
}

const std::string& Movement::getTag() const {
  return tag;
}


