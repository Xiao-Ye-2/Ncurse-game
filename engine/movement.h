#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "enums.h"
#include "entity.h"
#include "enums-user.h"

class Entity;

class Movement {
  size_t count;
  size_t frequency;
  const string tag;

  virtual void doMove(Action cmd, Entity *target) = 0;
  virtual void doReflectMove(Direction dir) = 0;
  virtual void doUnmove(Entity *target) = 0;
 protected:
  void incCount(size_t n);
  size_t getFrequency();
  // void setFrequency(size_t newFrequency);
  bool canMove();
  void basicMove(Entity *target, Direction d);
  void basicReverseMove(Entity *target, Direction d);

 public:
  Movement(size_t c, size_t f, const string& t);
  virtual ~Movement() = default;
  void incFrequency(size_t n);
  void move(Action cmd, Entity *target);
  void reflectMove(Direction dir);
  void unmove(Entity *target);
  const string& getTag() const;
};





#endif

