#ifndef MODEL_H
#define MODEL_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <set>
#include "entity.h"
#include "enums.h"
#include "collisionTable.h"
#include "curseControl.h"
#include "curseView.h"
#include "concreteCollision.h"
#include "genericEntity.h"
#include "wallEntity.h"
#include "playerControl.h"
#include "straightLine.h"
#include "transform.h"
#include "gravatate.h"
#include "forwardShot.h"

const size_t length = 80;
const size_t width = 22;
// struct cmp {
//   bool operator() (const Entity* a, const Entity* b) const { return a->getHeight() < b->getHeight(); }
// };

class Model {
  vector<unique_ptr<View>> views;
  vector<unique_ptr<Entity>> entities;
  unique_ptr<Controller> control;
  unique_ptr<CollisionTable> collision;
  unordered_map<Coordinate, vector<Entity *>, Coordinate::HashFunction> entityMap;

 protected:
  void addView(unique_ptr<View> v);
  void addController(unique_ptr<Controller> c);
  void addCollision(unique_ptr<CollisionTable> t);

  Action getAction();
  size_t numEntities();

  void addPositions(Entity *e);
  void erasePositions(Entity *e);
  void eraseEntity(Entity *target);

  void incOutside(Entity *e);
  GameStatus moveEntity(Action cmd, Entity *target);
  GameStatus moveEntities(Action cmd);
  unordered_set<Entity*> inquireEntities(unordered_set<string> lst);
  void clean(bool (*f)(Entity *));

  void updateViews(const vector<Coordinate>& allPos);
  void updateViews(const std::string& msg, StatusLine lineNumber);
  void displayViews();

 public:
  virtual ~Model() = default;
  unordered_set<Entity*> getCollision(Entity *target);
  void addEntity(unique_ptr<Entity> e);
  virtual void go() = 0;
};



#endif
