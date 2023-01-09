#include "concreteModel.h"


int main(int argc, char **argv) {
  if (argc > 1 && string(argv[1]) == "-cheat") {
    ConcreteModel_ARLG m {true};
    m.go();
  } else {
    ConcreteModel_ARLG m {false};
    m.go();
  }
  return 0;
}