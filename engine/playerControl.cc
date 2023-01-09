#include "playerControl.h"
#include <iostream>

PlayerControl::PlayerControl() : Movement{0, 1, "PlayerControl"}, lastMove{Action::INVALID} {}

void PlayerControl::doMove(Action cmd, Entity *target) {
  incCount(1);
  if (canMove()) {
    lastMove = cmd;
    if (cmd == Action::UP) {
      target->getDir() = Direction::UP;
      basicMove(target, Direction::UP);
    } else if (cmd == Action::DOWN) {
      target->getDir() = Direction::DOWN;
      basicMove(target, Direction::DOWN);
    } else if (cmd == Action::LEFT) {
      target->getDir() = Direction::LEFT;
      basicMove(target, Direction::LEFT);
    } else if (cmd == Action::RIGHT) {
      target->getDir() = Direction::RIGHT;
      basicMove(target, Direction::RIGHT);
    } else {

      return;
    }
    
  }
}

void PlayerControl::doReflectMove(Direction dir) {}

void PlayerControl::doUnmove(Entity *target) {
  if (lastMove == Action::RIGHT) {
    target->getPos().decX();
  } else if (lastMove == Action::UP) {
    target->getPos().incY();
  } else if (lastMove == Action::LEFT) {
    target->getPos().incX();
  } else if (lastMove == Action::DOWN) {
    target->getPos().decY();
  } else {
    // left open for furture cmds.
    return;
  }
}



