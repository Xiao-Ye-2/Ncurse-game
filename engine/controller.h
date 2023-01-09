#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <string>

enum class Action;

class Controller {
  virtual Action action() = 0;
 public:
  Action getAction();
  virtual ~Controller() = default;
};
#endif
