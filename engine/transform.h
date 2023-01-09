#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "movement.h"
#include "shape.h"
#include <deque>

class Transform : public Movement {
  deque<Shape> looks;

  void doMove(Action cmd, Entity *target) override;
  void doReflectMove(Direction d) override;
  void doUnmove(Entity *target) override;

 public:
  Transform(deque<Shape> l, size_t frequency);
  ~Transform() = default;
  void addShape(Shape newShape);
  void clean();
};


#endif
