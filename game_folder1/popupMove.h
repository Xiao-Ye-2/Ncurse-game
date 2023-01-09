#ifndef POPUPMOVE_H
#define POPUPMOVE_H

#include "../engine/movement.h"
#include "../engine/straightLine.h" 
#include "../engine/entity.h"
#include "../engine/model.h"

class Model;
class Entity;
class StraightLine;

class PopUp : public Movement {
  size_t numProceed;
  size_t changeAfter;
  size_t probability;
  Shape look;
  string tag;


  void doMove(Action cmd, Entity *target) override;
  void doReflectMove(Direction d) override;
  void doUnmove(Entity *target) override;

 public:
  PopUp(string name, size_t frequency, size_t changeAfter, size_t probability, Shape look);
  ~PopUp() = default;
};

#endif
