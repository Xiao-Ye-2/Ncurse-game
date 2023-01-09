#include "randWalk.h"
#include <stdlib.h>

Direction getRandDir() {
  int randchoice = rand() % 4;
  Direction dir;
  if (randchoice == 0) {
    dir = Direction::UP;
  } else if (randchoice == 1) {
    dir = Direction::DOWN;
  } else if (randchoice == 2) {
    dir = Direction::LEFT;
  } else {
    dir = Direction::RIGHT;
  }
  return dir;
}

RandWalk::RandWalk(size_t frequency, size_t changeAfter, size_t probability): Movement{0, frequency, "RandWalk"}, 
  numProceed{0}, changeAfter{changeAfter}, probability{probability} {
  srand (time(NULL));
  dir = getRandDir();
}

void RandWalk::doMove(Action cmd, Entity *target) {
  incCount(1);
  if (canMove()) {
    if (numProceed >= changeAfter) {
      int change = rand() % probability;
      if (change == 0) {
        numProceed -= changeAfter;
        dir = getRandDir();
      }
    } 
    ++numProceed;
      
    target->getDir() = dir;
    basicMove(target, dir);
  }
}

void RandWalk::doUnmove(Entity *target) {
  basicReverseMove(target, dir);
  Direction directions[4] = {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};
  while (true) {
    int choice = rand() % 4;
    if (directions[choice] != dir) {
      dir = directions[choice];
      break;
    }
  }

}

void RandWalk::doReflectMove(Direction d) {}