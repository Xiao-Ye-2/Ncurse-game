#ifndef VIEW_H
#define VIEW_H

#include <string>
#include "entity.h"

enum class StatusLine;

class View {
  virtual void doUpdate(int row, int col, chtype ch, Color c) = 0;
  virtual void doUpdate(const std::string& msg, StatusLine lineNumber) = 0;
  virtual void doDisplayView() = 0;

 public:
  virtual ~View() = default;
  void update(int row, int col, chtype ch, Color c);
  void update(const std::string& msg, StatusLine lineNumber);
  void displayView();
};

#endif
