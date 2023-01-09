#include "transform.h"

Transform::Transform(deque<Shape> l, size_t frequency) : Movement{0, frequency, "Transform"}, looks{std::move(l)} {}

void Transform::addShape(Shape newShape) {
  looks.push_back(std::move(newShape));
}

void Transform::clean() {
  looks = {};
}

void Transform::doMove(Action cmd, Entity *target) {
  incCount(1);
  if (canMove()) {
    addShape(target->getShape());
    target->setShape(std::move(looks[0]));
    looks.pop_front();
  }
}

void Transform::doReflectMove(Direction d) {}

void Transform::doUnmove(Entity *target) {
  looks.push_front(std::move(target->getShape()));
  target->setShape(std::move(looks[looks.size() - 1]));
  looks.pop_back();
}




