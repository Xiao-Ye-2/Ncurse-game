#include "entity.h"
#include <iostream>

Entity::Entity(string tag, Coordinate pos, Shape look, size_t height, Model *owner):
  moves{}, tag{std::move(tag)}, height{height}, pos{std::move(pos)}, dir{Direction::UP}, look{std::move(look)}, owner{owner} {}

vector<unique_ptr<Movement>>& Entity::getMove() {
  return moves;
}

void Entity::addMove(unique_ptr<Movement> move) {
  moves.emplace_back(std::move(move));
}

void Entity::eraseMove(const string& tag) {
  for (int i = moves.size() - 1; i >= 0; --i) {
    if (moves[i]->getTag() == tag) {
      moves.erase(moves.begin() + i);
      return;
    } 
  }
}
  
void Entity::eraseMoves(const string& tag) {
  for (int i = moves.size() - 1; i >= 0; --i) {
    if (moves[i]->getTag() == tag) {
      moves.erase(moves.begin() + i);
    } 
  }
}

const string& Entity::getTag() const { return tag; }

void Entity::incOutsideBoundary() { ++outsideBoundary; }
void Entity::resetOutsideBoundary() { outsideBoundary = 0; }
size_t Entity::getOutsideBoundary() { return outsideBoundary; }

size_t Entity::getHeight() const { return height; }
Coordinate& Entity::getPos() { return pos; }
Direction& Entity::getDir() { return dir; }

const Shape& Entity::getShape() const { return look; }
void Entity::setShape(Shape&& newS) { look = std::move(newS); }

Model *Entity::getOwner() const { return owner; }
void Entity::setOwner(Model *newOwner) { owner = newOwner; }

vector<Coordinate> Entity::getAllpos() {
  vector<Coordinate> result;
  int x = getPos().x;
  int y = getPos().y;
  for (const auto& coor : getShape().getBitmap()) {
    int newx = x + coor.first.x;
    int newy = y + coor.first.y;
    result.emplace_back(newx, newy);
  }
  return result;
}

