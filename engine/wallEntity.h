#ifndef WALLENTITY_H
#define WALLENTITY_H

#include "entity.h"

class Entity;

class WallEntity : public Entity {
  bool isKilled;

 public:
  WallEntity(string tag, Coordinate pos, Shape look, size_t height, Model *owner = nullptr);
  ~WallEntity() = default;

  void reflectMove() override;
  void damage() override;
  bool heal() override;
  void kill() override;
  // Direction& getDir() override; // garbage value
  bool isDead() override;
};


#endif
