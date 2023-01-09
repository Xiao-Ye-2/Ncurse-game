#include "view.h"

void View::update(int row, int col, chtype ch, Color c) {
  doUpdate(row, col, ch, c);
}

void View::update(const std::string& msg, StatusLine lineNumber) {
  doUpdate(msg, lineNumber);
}

void View::displayView(){
  doDisplayView();
}

