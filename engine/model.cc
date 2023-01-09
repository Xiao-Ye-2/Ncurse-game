#include "model.h"

template<typename T> void vectorErase(vector<T>& v, const T& val) {
  int n = v.size();
  for (int i = n - 1; i >= 0; --i) {
    if (val == v[i]) {
      v.erase(v.begin() + i);
    }
  }
}


void Model::addView(std::unique_ptr<View> v){
  views.emplace_back(std::move(v));
}
void Model::addController(std::unique_ptr<Controller> c){
  control = std::move(c);
}

void Model::addPositions(Entity *e) {
  const auto& allPos = e->getAllpos();
  for (const auto& coor : allPos) {
    entityMap[coor].emplace_back(e);
  }
}

void Model::addEntity(unique_ptr<Entity> e) {
  addPositions(e.get());
  updateViews(e->getAllpos());
  e->setOwner(this);
  entities.emplace_back(std::move(e));
}

void Model::addCollision(unique_ptr<CollisionTable> t) {
  collision = std::move(t);
}

Action Model::getAction(){
  return control->getAction();
}

unordered_set<Entity*> Model::getCollision(Entity *target) {
  unordered_set<Entity*> result;
  auto allPos = target->getAllpos();
  for (const auto& coor : allPos) {
    for (Entity *ptr : entityMap[coor]) {
      if (ptr != target) {;
        result.insert(ptr);
      }
    }
  }
  return result;
}

void Model::erasePositions(Entity *e) {
  const auto& allPos = e->getAllpos();
  for (const auto& coor : allPos) {
    vectorErase(entityMap[coor], e);
  }
}

void Model::eraseEntity(Entity *e) {
  // e->setOwner(nullptr);
  erasePositions(e);
  updateViews(e->getAllpos());
  int n = entities.size();
  for (int i = n - 1; i >= 0; --i) {
    if (entities[i].get() == e) {
      entities.erase(entities.begin() + i);
    }
  }
  return;
}

void Model::incOutside(Entity *e) {
  auto pos = e->getPos();
  if (pos.x >= 0 && pos.y >=0 && pos.x < int(length) && pos.y < int(width)) {
    e->resetOutsideBoundary();
  } else {
    e->incOutsideBoundary();
  }
}

GameStatus Model::moveEntity(Action cmd, Entity *target) {
  GameStatus result = GameStatus::CONTINUE;

  auto& moves = target->getMove();
  erasePositions(target);
  updateViews(target->getAllpos());
  for (const auto& move : moves) {
    move->move(cmd, target);
    const auto& collides = getCollision(target);
    bool moveUndone = false;
    for (Entity *obstacle : collides) {
      GameStatus temp = collision->collide(target, obstacle, move.get(), moveUndone);
      if (temp != GameStatus::CONTINUE) { result = temp; }
    }
  }
  addPositions(target);
  updateViews(target->getAllpos());

  incOutside(target);
  return result;
}

GameStatus Model::moveEntities(Action cmd) {
  GameStatus result = GameStatus::CONTINUE;

  vector<Entity *> foo;
  for (const auto& e : entities) { foo.push_back(e.get()); }

  for (Entity *e : foo) {
    GameStatus temp = moveEntity(cmd, e);
    if (temp != GameStatus::CONTINUE) { result = temp; }
  }
  return result;
}

unordered_set<Entity*> Model::inquireEntities(unordered_set<string> lst) {
  unordered_set<Entity *> result;
  int n = numEntities();
  for (int i = 0; i < n; ++i) {
    if (lst.find(entities[i]->getTag()) != lst.end()) {
      result.insert(entities[i].get());
    } 
  }
  return result;
}

void Model::clean(bool (*f)(Entity *)) {
  vector<Entity *> collection;
  for (const auto& e : entities) {
    if (f(e.get())) {
      collection.emplace_back(e.get());
    }
  }
  for (Entity *ptr : collection) {
    eraseEntity(ptr);
  }
}


size_t Model::numEntities() {
  return entities.size();
}

void Model::updateViews(const vector<Coordinate>& allPos) {
  for (const auto& cell : allPos) {
    chtype ch = ' ';
    Color c = Color::WHITE;
    int height = -1;
    for (const auto e : entityMap[cell]) {
      if (int(e->getHeight()) > height) {
        const auto& pos = e->getPos();
        ch = e->getShape().getBitmap().at(Coordinate(cell.x - pos.x, cell.y - pos.y));
        c = e->getShape().getColor();
        height = e->getHeight();
      }
    }
    for(auto& view: views) {
      view->update(cell.y, cell.x, ch, c);
    }
  }
}

void Model::updateViews(const std::string& msg, StatusLine lineNumber) {
  for( auto& view: views ){
    view->update(msg, lineNumber);
  }
}

void Model::displayViews(){
  for( auto& view: views ){
    view->displayView();
  }
}