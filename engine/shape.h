#ifndef SHAPE_H
#define SHAPE_H

#include <unordered_map>
#include <utility>
#include <ncurses.h>
#include "enums.h"

using namespace std;

enum class Color;

struct Coordinate {
  int x;
  int y;

  Coordinate(int x, int y);
  Coordinate(const Coordinate& other);
  bool operator==(const Coordinate& other) const;

  void incX();
  void decX();
  void incY();
  void decY();

  struct HashFunction {
    size_t operator()(const Coordinate& coor) const;
  };
};

class Shape {
  unordered_map<Coordinate, chtype, Coordinate::HashFunction> bitmap;
  Color col;

 public:
  Shape(unordered_map<Coordinate, chtype, Coordinate::HashFunction> bitmap, Color col = Color::WHITE);
  Shape(const Shape& other);
  Shape(Shape && other);
  Shape& operator=(Shape other);
  ~Shape() = default;
  const unordered_map<Coordinate, chtype, Coordinate::HashFunction>& getBitmap() const;
  const Color& getColor() const;
};


#endif
