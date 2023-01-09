#include "curseView.h"
#include <ncurses.h>


Curses::Curses(int start_y, int start_x, bool grid) : grid{grid}, length{80} {
  setlocale(LC_ALL, "");
  initscr();
  nodelay(stdscr, true);
  keypad(stdscr, true);
  noecho();
  curs_set(0);
  cbreak();

  if (grid) { 
    width = 22; 
  } else {
    width = 3;
  }
  view = newwin(width, length, start_y, start_x);
  refresh();

  // initialize the color
  if (has_colors()) { start_color(); }
  init_pair(0, COLOR_BLACK, COLOR_BLACK);
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
  init_pair(4, COLOR_BLUE, COLOR_BLACK);
  init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(6, COLOR_CYAN, COLOR_BLACK);
  init_pair(7, COLOR_WHITE, COLOR_BLACK);

  // initialize the cells
  clear();
}

void Curses::doDisplayView() {
  wrefresh(view);
}

void Curses::doUpdate(int row, int col, chtype ch, Color c) {
  if (!grid) return;
  if (row < 0 || col < 0 || row >= width || col >= length) return;
  wattron(view, COLOR_PAIR(c));
  mvwaddch(view, row, col, ch);
  wattroff(view, COLOR_PAIR(c));
}

void Curses::doUpdate(const std::string& msg, StatusLine lineNumber){
  if (grid) return;
  mvwaddstr(view, int(lineNumber), 0, string(length, ' ').c_str());
  mvwaddstr(view, int(lineNumber), 0, msg.c_str());
}

Curses::~Curses() { endwin(); }
