#ifndef CONCRETEMODEL_H
#define CONCRETEMODEL_H

#include "popupMove.h"
#include "randWalk.h"
#include "targetChase.h"
#include "../engine/model.h"

class WallEntity;
class GenericEntity;

class ConcreteModel_ARLG : public Model {
  bool developer;

  GameStatus runLevel(const size_t curLevel);
 public:
  ConcreteModel_ARLG(bool developer);
  void go() override;
};


#endif
