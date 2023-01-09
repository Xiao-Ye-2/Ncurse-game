CXX = g++
CXXFLAGS = -std=c++14 -Wall -MMD

EXEC1 = game1
EXEC2 = game2
EXEC3 = game3

OBJECTS = engine/collisionTable.o engine/concreteCollision.o engine/genericEntity.o engine/wallEntity.o engine/controller.o engine/curseControl.o engine/curseView.o engine/entity.o engine/gravatate.o engine/model.o engine/forwardShot.o engine/movement.o engine/playerControl.o engine/shape.o engine/straightLine.o engine/transform.o engine/view.o
OBJECTS1 = game_folder1/main1.o game_folder1/concreteModel.o game_folder1/randWalk.o game_folder1/popupMove.o game_folder1/targetChase.o
OBJECTS2 = game_folder2/main2.o game_folder2/concreteModel2.o
OBJECTS3 = game_folder3/main3.o game_folder3/concreteModel3.o

DEPENDS = ${OBJECTS:.o=.d}
DEPENDS1 = ${OBJECTS1:.o=.d}
DEPENDS2 = ${OBJECTS2:.o=.d}
DEPENDS3 = ${OBJECTS3:.o=.d}

all: ${EXEC1} ${EXEC2} ${EXEC3}

${EXEC1}: ${OBJECTS} ${OBJECTS1}
	${CXX} ${OBJECTS} ${OBJECTS1} -o ${EXEC1} -lncurses
-include ${DEPENDS} ${DEPENDS1} 

${EXEC2}: ${OBJECTS} ${OBJECTS2}
	${CXX} ${OBJECTS} ${OBJECTS2} -o ${EXEC2} -lncurses
-include ${DEPENDS} ${DEPENDS2} 

${EXEC3}: ${OBJECTS} ${OBJECTS3}
	${CXX} ${OBJECTS} ${OBJECTS3} -o ${EXEC3} -lncurses
-include ${DEPENDS}${DEPENDS3} 

.PHONY: clean
clean:
	rm ${EXEC1} ${EXEC2} ${EXEC3} ${DEPENDS} ${DEPENDS1} ${DEPENDS2} ${DEPENDS3} ${OBJECTS} ${OBJECTS1} ${OBJECTS2} ${OBJECTS3}

