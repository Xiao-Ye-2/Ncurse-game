#include "curseControl.h"
#include <string>

CurseKeyboard::CurseKeyboard(std::map<int,Action> map): mapping{std::move(map)} {
  setlocale(LC_ALL, "");
}

Action CurseKeyboard::action(){
  int n;
  n = getch();
  flushinp();
  if ( mapping.find(n) != mapping.end() ){
    return mapping[n];
  }
  return Action{};
}
