#ifndef GENERICENTITY_H
#define GENERICENTIIY_H

#include "entity.h"

class Entity;

class GenericEntity : public Entity {
  int hp;
  int maxHp;
  bool vulnerable;
  // Direction dir;
 
//  protected:
//   void setHp(int newHp);
 public:

  GenericEntity(string tag, size_t h, Coordinate pos, Shape look, size_t height, Model *owner = nullptr);
  ~GenericEntity() = default;

  bool getVulnerable();
  void setVulnerable(bool newState);
  int getHp() const;
  int getMaxHp() const;
  void reflectMove() override;
  void damage() override;
  bool heal() override;
  void kill() override;
  // Direction getDir() override;
  // void setDir(Direction d) override;
  bool isDead() override;
};



#endif
