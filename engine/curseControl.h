#ifndef CURSECONTROL_H
#define CURSECONTROL_H

#include "controller.h"
#include <iostream>
#include <string>
#include <map>
#include <ncurses.h>

class Mapping;
enum class Action;

class CurseKeyboard: public Controller {
  std::map<int,Action> mapping; 

  Action action() override;
 public:
  CurseKeyboard(std::map<int,Action> map);
  ~CurseKeyboard() = default;
};
#endif
