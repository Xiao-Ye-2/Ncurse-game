#ifndef CONCRETEMODEL2_H
#define CONCRETEMODEL2_H

#include "../engine/model.h"

class ConcreteModel_SPACE : public Model {
  size_t enemy_height;

  void CreateArmy(size_t level);
  void CreateFence();
  
 public:
  ConcreteModel_SPACE();
  void go() override; 
};


#endif

