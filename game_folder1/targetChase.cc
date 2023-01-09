#include "targetChase.h"
#include <cmath>
#include <limits.h>

class InvalidChaseDir {};

int SumTwoSquare(int a, int aa, int b, int bb) {
  return pow(a - aa, 2) + pow(b - bb, 2);
}

TargetChase::TargetChase(Entity *chase, size_t frequency) : Movement{0, frequency, "TargetChase"}, dir{Direction::UP}, chase{chase} {}

void TargetChase::doReflectMove(Direction d) {}

void TargetChase::doMove(Action cmd, Entity *target) {
  incCount(1);
  if (canMove()) {
    int choice;
    int distance = INT_MAX;
    auto pos = target->getPos();
    auto movePos = chase->getPos();
    if (SumTwoSquare(pos.x, movePos.x, pos.y, movePos.y) < distance) {
      distance = SumTwoSquare(pos.x, movePos.x, pos.y, movePos.y);
      choice = 0;
    }
    if (SumTwoSquare(pos.x, movePos.x, pos.y - 1, movePos.y) < distance) {
      distance = SumTwoSquare(pos.x, movePos.x, pos.y - 1, movePos.y);
      choice = 1;
    }
    if (SumTwoSquare(pos.x, movePos.x, pos.y + 1, movePos.y) < distance) {
      distance = SumTwoSquare(pos.x, movePos.x, pos.y + 1, movePos.y);
      choice = 2;
    }
    if (SumTwoSquare(pos.x - 1, movePos.x, pos.y, movePos.y) < distance) {
      distance = SumTwoSquare(pos.x - 1, movePos.x, pos.y, movePos.y);
      choice = 3;
    }
    if (SumTwoSquare(pos.x + 1, movePos.x, pos.y, movePos.y) < distance) {
      choice = 4;
    }
    if (choice != 0) {
      if (choice == 1) { dir = Direction::UP; }
			else if (choice == 2) { dir = Direction::DOWN; }
			else if (choice == 3) { dir = Direction::LEFT; }
			else if (choice == 4) { dir = Direction::RIGHT; }
			else { throw InvalidChaseDir{}; }
			target->getDir() = dir;
			basicMove(target, dir);
    }
  }
} 


void TargetChase::doUnmove(Entity *target) {
	basicReverseMove(target, dir);
}