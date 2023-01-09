#include "shape.h"

Coordinate::Coordinate(int x, int y) : x{x}, y{y} {}

Coordinate::Coordinate(const Coordinate& other) {
  x = other.x;
  y = other.y;
}

bool Coordinate::operator==(const Coordinate& other) const {
  return x == other.x && y == other.y;
}

size_t Coordinate::HashFunction::operator()(const Coordinate& coor) const {
  size_t x = std::hash<int>()(coor.x);
  size_t y = std::hash<int>()(coor.y) << 1;
  return x ^ y;
}

void Coordinate::incX() { ++x; }
void Coordinate::decX() { --x; }
void Coordinate::incY() { ++y; }
void Coordinate::decY() { --y; }



Shape::Shape(unordered_map<Coordinate, chtype, Coordinate::HashFunction> bitmap, Color col) : bitmap{std::move(bitmap)}, col{col} {}

Shape::Shape(const Shape& other) {
  bitmap = other.bitmap;
  col = other.col;
}
  
Shape::Shape(Shape&& other) {
  bitmap = std::move(other.bitmap);
  col = std::move(other.col);
}

Shape& Shape::operator=(Shape other) {
  swap(bitmap, other.bitmap);
  swap(col, other.col);
  return *this;
}

const unordered_map<Coordinate, chtype, Coordinate::HashFunction>& Shape::getBitmap() const {
  return bitmap;
}

const Color& Shape::getColor() const {
  return col;
}

