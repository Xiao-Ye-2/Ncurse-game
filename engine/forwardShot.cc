#include "forwardShot.h"
#include <limits.h>

ForwardShot::ForwardShot(Direction d, Action bind, size_t frequency, string name, Shape look, size_t speed) : Movement{0, frequency, "ForwardShot"}, dir{d},
  bind{bind}, tag{std::move(name)}, bulletLook{std::move(look)}, bulletSpeed{speed} {}


void ForwardShot::doMove(Action cmd, Entity *target) {
  if (bind == Action::ALL || bind == cmd) { incCount(1); }
  if (canMove()) {
    Model *owner = target->getOwner();
    size_t h = target->getHeight();
    if (h > 0) { --h; }
    
    unique_ptr<Entity> newEntity = make_unique<WallEntity>(tag , target->getPos(), bulletLook, h);
    if (dir == Direction::PLAYER) { 
      newEntity->addMove(make_unique<StraightLine>(target->getDir(), bulletSpeed));
    } else {
      newEntity->addMove(make_unique<StraightLine>(dir, bulletSpeed));
    }
    owner->addEntity(std::move(newEntity));
  }
}
  
void ForwardShot::doReflectMove(Direction d) {}
void ForwardShot::doUnmove(Entity *target) { incCount(getFrequency()); }