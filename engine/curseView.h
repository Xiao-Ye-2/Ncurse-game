#ifndef CURSEVIEW_H
#define CURSEVIEW_H

#include "view.h"
#include <vector>
#include <iostream>
#include <map>
#include <utility>

using namespace std;

enum class Color;

class Curses: public View {
  WINDOW * view;
  bool grid;
  int length;
  int width;

  void doUpdate(int row, int col, chtype ch, Color c) override;
  void doUpdate(const std::string& msg, StatusLine lineNumber) override;
  void doDisplayView() override;
 public:
  Curses(int start_y, int start_x, bool grid);
  ~Curses();
};


#endif
