#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <utility>
#include <memory>
#include <string>
#include "shape.h"
#include "enums.h"
#include "movement.h"

using namespace std;

class Movement;
class Model;

class Entity {
  vector<unique_ptr<Movement>> moves;
  string tag;
  size_t height;
  Coordinate pos;
  Direction dir;
  Shape look;
  Model *owner;
  size_t outsideBoundary = 0;

 public:
  Entity(string tag, Coordinate pos, Shape look, size_t height, Model *owner = nullptr);
  virtual ~Entity() = default;

  vector<unique_ptr<Movement>>& getMove();
  const string& getTag() const;

  void addMove(unique_ptr<Movement> move);
  void eraseMove(const string& type);
  void eraseMoves(const string& type);

  void incOutsideBoundary();
  void resetOutsideBoundary();
  size_t getOutsideBoundary();

  size_t getHeight() const;
  Coordinate& getPos();
  Direction& getDir();
  vector<Coordinate> getAllpos();

  const Shape& getShape() const;
  void setShape(Shape&& newS); // setShape will destroy the given shape

  Model *getOwner() const;
  void setOwner(Model *newOwner);

  virtual void reflectMove() = 0;
  virtual void damage() = 0;
  virtual bool heal() = 0;
  virtual void kill() = 0;
  virtual bool isDead() = 0;
};





#endif
