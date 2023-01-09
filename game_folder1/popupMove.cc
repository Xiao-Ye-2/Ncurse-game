#include "popupMove.h"



PopUp::PopUp(string name, size_t frequency, size_t changeAfter, size_t probability, Shape look) :
  Movement{0, frequency, "PopUp"}, numProceed{0}, changeAfter{changeAfter} , probability{probability},
  look(std::move(look)), tag{std::move(name)} {} 

void PopUp::doMove(Action cmd, Entity *target) {
  if (!dynamic_cast<GenericEntity *> (target)) { return; }
  incCount(1);
  if (canMove()) {
    GenericEntity * e = static_cast<GenericEntity *> (target);
    if (numProceed >= changeAfter) {
      int change = rand() % probability;
      if (change == 0) {
        numProceed -= changeAfter;

        auto oldLook = e->getShape();
        e->setShape(std::move(look));
        look = std::move(oldLook);

        e->setVulnerable(!e->getVulnerable());
      }
    } 
    ++numProceed;
    if (e->getVulnerable()) {
      size_t h = target->getHeight();
      if (h > 0) { --h; }
      Direction dirLst[4] = {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};
      for (auto d : dirLst) {
        unique_ptr<Entity> projectile = make_unique<WallEntity>(tag, target->getPos(), Shape({{{0,0}, 'O'}}, Color::WHITE), h);
        projectile->addMove(make_unique<StraightLine>(d, 1));
        target->getOwner()->addEntity(std::move(projectile));
      } 
    }
  }
}

void PopUp::doReflectMove(Direction d) {}

void PopUp::doUnmove(Entity *target) {
  auto oldLook = target->getShape();
  target->setShape(std::move(look));
  look = std::move(oldLook);

  if (dynamic_cast<GenericEntity *> (target)) {
    auto e = static_cast<GenericEntity *> (target);
    e->setVulnerable(!e->getVulnerable());
  }
}
