#include "concreteCollision.h"
#include <algorithm>
#include <iostream>

class InsufficientTable{};


template<typename T> vector<T> intersection(vector<T> v1, vector<T> v2){
  vector<T> v3;
  for (const T a : v1) {
    for (const T b : v2) {
      if (a == b) { 
        v3.emplace_back(b); 
        break;
      }
    }
  }
  return v3;
}


ConcreteCollision::ConcreteCollision(map<pair<string, string>, pair<vector<CollideType>, vector<CollideType>>> t): table{std::move(t)} {}

GameStatus ConcreteCollision::collide(Entity *source, Entity *target, Movement *move, bool &hasUndone) {
  GameStatus result = GameStatus::CONTINUE;
  vector<CollideType> sourceCollision;
  vector<CollideType> targetCollision;

  bool sourceNotDead = !source->isDead();
  bool targetNotDead = !target->isDead();

  auto it = table.find(make_pair(source->getTag(), target->getTag()));
  if (it != table.end()) {
    sourceCollision = it->second.first;
    targetCollision = it->second.second;
  } else {
    it = table.find(make_pair(target->getTag(), source->getTag()));
    if (it != table.end()) {
      targetCollision = it->second.first;
      sourceCollision = it->second.second;
    } else {
      it = table.find(make_pair(source->getTag(), "*"));
      if (it != table.end()) {
        sourceCollision = it->second.first;
        targetCollision = it->second.second;
      } else {
        it = table.find(make_pair(target->getTag(), "*"));
        if (it != table.end()) {
          targetCollision = it->second.first;
          sourceCollision = it->second.second;
        } else {
          targetCollision = {};
          sourceCollision = {};
          // throw InsufficientTable{};
        }
      }
    }
  }

  for (auto effect : sourceCollision) {
    if (effect == CollideType::BOUNCE) {
      hasUndone = bounce(source, move, hasUndone);
    } else if (effect == CollideType::STOP) {
      hasUndone = stop(source, move, hasUndone);
    } else if (effect == CollideType::DESTROY) {
      if (targetNotDead) { destroy(source); }
    } else if (effect == CollideType::DAMAGE) {
      if (targetNotDead) {
        if (target->getTag() == "Fire")  { continue; }
        if (source->getTag() == "Snake") {
        auto lst = intersection(source->getAllpos(), target->getAllpos());
        auto pos = source->getPos();
        for (const auto& coor : lst) {
          if (source->getShape().getBitmap().at(Coordinate(coor.x - pos.x, coor.y - pos.y)) != 'Z') {
            damage(source);
            break;
          }
        }
        } else { damage(source); }
      }
    } else if (effect == CollideType::HEAL) {
      if (targetNotDead) { heal(source); }
    } else if (effect == CollideType::WIN) {
      result = GameStatus::WIN;
    } else if (effect == CollideType::LOSE) {
      result = GameStatus::LOSE;
    }
  }

  for (auto effect : targetCollision) { 
    if (effect == CollideType::BOUNCE) {
      bounce(target, move, true);
    } else if (effect == CollideType::STOP) {
      stop(target, move, true);
    } else if (effect == CollideType::DESTROY) {
      if (sourceNotDead) { destroy(target); }
    } else if (effect == CollideType::DAMAGE) {
      if (sourceNotDead) {
        if (target->getTag() == "Snake") {
        auto lst = intersection(source->getAllpos(), target->getAllpos());
        auto pos = target->getPos();
        for (const auto& coor : lst) {
          if (target->getShape().getBitmap().at(Coordinate(coor.x - pos.x, coor.y - pos.y)) != 'Z') {
            damage(target);
            break;
          }
        }
        } else { damage(target); }
      }
    } else if (effect == CollideType::HEAL) {
      if (sourceNotDead) { heal(target); }
    } else if (effect == CollideType::WIN) {
      result = GameStatus::WIN;
    } else if (effect == CollideType::LOSE) {
      result = GameStatus::LOSE;
    }
  }


  if (source->getTag() == "Snake" && (target->getTag() == "WALL" || target->getTag() == "Player")) { 
    Color col = source->getShape().getColor();
    if (source->getDir() == Direction::UP) { source->setShape(Shape({{{0,0}, 'z'}, {{0, 1}, 'z'}, {{0,2}, 'z'}, {{0,3}, 'Z'}}, col)); }
    if (source->getDir() == Direction::DOWN) { source->setShape(Shape({{{0,0}, 'Z'}, {{0, 1}, 'z'}, {{0,2}, 'z'}, {{0,3}, 'z'}}, col)); }
    if (source->getDir() == Direction::LEFT) { source->setShape(Shape({{{0,0}, 'z'}, {{1, 0}, 'z'}, {{2,0}, 'z'}, {{3,0}, 'Z'}}, col)); }
    if (source->getDir() == Direction::RIGHT) { source->setShape(Shape({{{0,0}, 'Z'}, {{1, 0}, 'z'}, {{2,0}, 'z'}, {{3,0}, 'z'}}, col)); }
  } 

  return result;
}
