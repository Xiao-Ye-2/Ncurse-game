#include "concreteModel.h"
#include <limits.h>


const size_t fire_change = 20;
const size_t walker_generate = 4;
const size_t health_generate = 2;
const size_t snake_speed = 1;
const size_t stalker_speed = 6;
const size_t max_fire = 2;
const size_t minion_frequency = 100;


bool DeadEntity(Entity *e) {
  return e->isDead();
}

bool TagHealth(Entity *e) {
  return e->getTag() == "Health Pickups";
}
bool TagExit(Entity *e) {
  return e->getTag() == "Exit";
}
bool TagFire(Entity *e) {
  return e->getTag() == "Fire";
}
bool TagEnemy(Entity *e) {
  return e->getTag() == "Walker" || e->getTag() == "Stalker" || e->getTag() == "Snake" || e->getTag() == "Pop-Up" || e->getTag() == "boss" || e->getTag() == "Boss";
}
bool TagBullet(Entity *e) {
  return e->getTag() == "P:Bullet" || e->getTag() == "E:Bullet";
}


unique_ptr<Entity> createBoard() {
  // int height;
  // if (solid) height = INT_MAX; 
  // else { height = 0; }

  int cols = 80;
  int rows = 22;
  unordered_map<Coordinate, chtype, Coordinate::HashFunction> bitmap;
  for (int i = 1; i < cols - 1; ++i) {
    bitmap[Coordinate(i, 0)] = ACS_HLINE;
    bitmap[Coordinate(i, rows - 1)] = ACS_HLINE;
  }
  for (int i = 1; i < rows - 1; ++i) {
    bitmap[Coordinate(0, i)] = ACS_VLINE;
    bitmap[Coordinate(cols - 1, i)] = ACS_VLINE;
  }
  bitmap[Coordinate(0, 0)] = ACS_ULCORNER;
  bitmap[Coordinate(cols - 1, 0)] = ACS_URCORNER;
  bitmap[Coordinate(0, rows - 1)] = ACS_LLCORNER;
  bitmap[Coordinate(cols - 1, rows - 1)] = ACS_LRCORNER;
  unique_ptr<WallEntity> result = make_unique<WallEntity>("WALL", Coordinate{0,0}, Shape(bitmap, Color::WHITE), 0);
  return std::move(result);
}


unique_ptr<CollisionTable> CreateCollision() {
  map<pair<string, string>, pair<vector<CollideType>, vector<CollideType>>> t{};
  t[make_pair("WALL", "*")] = make_pair<vector<CollideType>, vector<CollideType>> ({}, {CollideType::STOP});
  t[make_pair("WALL","Snake")] = make_pair<vector<CollideType>, vector<CollideType>> ({}, {CollideType::STOP, CollideType::BOUNCE}); 
  t[make_pair("WALL","boss")] = make_pair<vector<CollideType>, vector<CollideType>> ({}, {CollideType::STOP, CollideType::BOUNCE}); 
  t[make_pair("WALL","Boss")] = make_pair<vector<CollideType>, vector<CollideType>> ({}, {CollideType::STOP, CollideType::BOUNCE}); 

  t[make_pair("P:Bullet", "*")] = make_pair<vector<CollideType>, vector<CollideType>> ({CollideType::DESTROY}, {CollideType::DAMAGE});
  t[make_pair("E:Bullet", "WALL")] = make_pair<vector<CollideType>, vector<CollideType>> ({CollideType::DESTROY}, {});
  t[make_pair("P:Bullet", "Player")] = make_pair<vector<CollideType>, vector<CollideType>> ({}, {});
  t[make_pair("Exit", "Player")] = make_pair<vector<CollideType>, vector<CollideType>> ({CollideType::WIN}, {});

  t[make_pair("Snake", "Player")] = make_pair<vector<CollideType>, vector<CollideType>> ({CollideType::BOUNCE, CollideType::DAMAGE}, {CollideType::STOP, CollideType::DAMAGE});
  t[make_pair("Walker", "Player")] = make_pair<vector<CollideType>, vector<CollideType>> ({CollideType::STOP, CollideType::DAMAGE}, {CollideType::STOP, CollideType::DAMAGE});
  t[make_pair("Stalker", "Player")] = make_pair<vector<CollideType>, vector<CollideType>> ({CollideType::STOP, CollideType::DAMAGE}, {CollideType::STOP, CollideType::DAMAGE});
  t[make_pair("Pop-Up", "Player")] = make_pair<vector<CollideType>, vector<CollideType>> ({CollideType::STOP, CollideType::DAMAGE}, {CollideType::STOP, CollideType::DAMAGE});
  t[make_pair("boss", "Player")] = make_pair<vector<CollideType>, vector<CollideType>> ({CollideType::BOUNCE}, {CollideType::STOP, CollideType::DAMAGE});
  t[make_pair("Boss", "Player")] = make_pair<vector<CollideType>, vector<CollideType>> ({CollideType::BOUNCE}, {CollideType::STOP, CollideType::DAMAGE});
  t[make_pair("E:Bullet", "Player")] = make_pair<vector<CollideType>, vector<CollideType>> ({CollideType::DESTROY}, {CollideType::DAMAGE});

  t[make_pair("Fire", "*")] = make_pair<vector<CollideType>, vector<CollideType>> ({}, {CollideType::DAMAGE});

  t[make_pair("P:Bullet", "E:Bullet")] = make_pair<vector<CollideType>, vector<CollideType>> ({}, {});
  t[make_pair("P:Bullet", "Fire")] = make_pair<vector<CollideType>, vector<CollideType>> ({CollideType::DESTROY}, {CollideType::DESTROY});
  t[make_pair("E:Bullet", "Fire")] = make_pair<vector<CollideType>, vector<CollideType>> ({CollideType::DESTROY}, {CollideType::DESTROY});
  t[make_pair("P:Bullet", "exit")] = make_pair<vector<CollideType>, vector<CollideType>> ({}, {});
  t[make_pair("P:Bullet", "Exit")] = make_pair<vector<CollideType>, vector<CollideType>> ({}, {});
  t[make_pair("Health Pickups", "Player")] = make_pair<vector<CollideType>, vector<CollideType>> ({CollideType::DESTROY}, {CollideType::HEAL});
  t[make_pair("Health Pickups", "P:Bullet")] = make_pair<vector<CollideType>, vector<CollideType>> ({}, {});
  return make_unique<ConcreteCollision>(t);
}

void CreateFire(Model * m) {
  while (true) {
    int x = rand() % 78 + 1;
    int y = rand() % 20 + 1;
    unique_ptr<Entity> fire = make_unique<WallEntity>("Fire", Coordinate {x,y}, Shape({{Coordinate(0,0), 'x'}}, Color::RED), 1);
    if (m->getCollision(fire.get()).size() == 0) {
      deque<Shape> newlook {Shape({{Coordinate(0,0), 'X'}}, Color::RED)};
      fire->addMove(make_unique<Transform>(newlook, fire_change));
      m->addEntity(std::move(fire));
      break;
    }
  }
}

void CreateHealthPk(Model *m, Coordinate pos) {
  unique_ptr<Entity> health_pickup = make_unique<WallEntity>("Health Pickups", pos, Shape({{Coordinate(0,0), 'h'}}, Color::MAGENTA), 1);
  m->addEntity(std::move(health_pickup));
}

void CreateUnrevealExit(Model *m) {
  while (true) {
    int x = rand() % 78 + 1;
    int y = rand() % 20 + 1;
    unique_ptr<Entity> exit = make_unique<WallEntity>("exit", Coordinate {x,y}, Shape({{Coordinate(0,0), 'e'}}, Color::BLUE), 0);
    if (m->getCollision(exit.get()).size() == 0) {
      m->addEntity(std::move(exit));
      break;
    }
  }
}

void CreateRevealExit(Model *m, Coordinate pos) {
  unique_ptr<Entity> Exit = make_unique<WallEntity>("Exit", pos, Shape({{Coordinate(0,0), 'E'}}, Color::BLUE), 1);
  m->addEntity(std::move(Exit));
}

void CreateWalker(Model *m) {
  while (true) {
    int x = rand() % 77 + 1;
    int y = rand() % 19 + 1;
    unique_ptr<Entity> walker = make_unique<GenericEntity>("Walker", 3, Coordinate(x,y), Shape({{Coordinate(0,0), 'a'}, {Coordinate(1,0), 'a'}, {Coordinate(0,1), 'a'}, {Coordinate(1,1), 'a'}}, Color::MAGENTA), 1);
    if (m->getCollision(walker.get()).size() == 0) {
      walker->addMove(make_unique<RandWalk>(3, 3, 3));
      m->addEntity(std::move(walker));
      break;
    }
  }
}
void CreateWalker(Model *m, Coordinate pos) {
  unique_ptr<Entity> walker = make_unique<GenericEntity>("Walker", 3, pos, Shape({{Coordinate(0,0), 'a'}, {Coordinate(1,0), 'a'}, {Coordinate(0,1), 'a'}, {Coordinate(1,1), 'a'}}, Color::MAGENTA), 1);
  walker->addMove(make_unique<RandWalk>(3, 3, 3));
  m->addEntity(std::move(walker));
}

void CreateStalker(Model *m, Entity *player) {
  while (true) {
    int x = rand() % 77 + 1;
    int y = rand() % 19 + 1;
    unique_ptr<Entity> stalker = make_unique<GenericEntity>("Stalker", 2, Coordinate(x,y), Shape({{Coordinate(0,0), 'b'}, {Coordinate(1,0), 'b'}, {Coordinate(0,1), 'b'}, {Coordinate(1,1), 'b'}}, Color::CYAN), 1);
    if (m->getCollision(stalker.get()).size() == 0) {
      stalker->addMove(make_unique<TargetChase>(player, stalker_speed));
      m->addEntity(std::move(stalker));
      break;
    }
  }
}

void CreatePopUp(Model *m) {
  while (true) {
    int x = rand() % 78 + 1;
    int y = rand() % 20 + 1;
    unique_ptr<Entity> popUp = make_unique<GenericEntity>("Pop-Up", 4, Coordinate{x,y}, Shape({{{0,0}, 'c'}}, Color::YELLOW), 1);
    if (m->getCollision(popUp.get()).size() == 0) {
      popUp->addMove(make_unique<PopUp>("E:Bullet", 5, 10, 4, Shape({{{0,0}, 'D'}}, Color::YELLOW)));
      m->addEntity(std::move(popUp));
      break;
    }
  }
}

void CreateSnake(Model *m) {
  int choice = rand() % 4;
  Color col = Color::GREEN;
  Shape looks[4] = {
    Shape({{{0,0}, 'Z'}, {{0, 1}, 'z'}, {{0,2}, 'z'}, {{0,3}, 'z'}}, col),
    Shape({{{0,0}, 'z'}, {{0, 1}, 'z'}, {{0,2}, 'z'}, {{0,3}, 'Z'}}, col),
    Shape({{{0,0}, 'Z'}, {{1, 0}, 'z'}, {{2,0}, 'z'}, {{3,0}, 'z'}}, col),
    Shape({{{0,0}, 'z'}, {{1, 0}, 'z'}, {{2,0}, 'z'}, {{3,0}, 'Z'}}, col)
  };
  Direction dir[4] = {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};
  if (choice == 0 || choice == 1) {
    while (true) {
      int x = rand() % 78 + 1;
      int y = rand() % 17 + 1;
      unique_ptr<Entity> snake = make_unique<GenericEntity>("Snake", 3, Coordinate{x,y}, looks[choice], 1);
      if (m->getCollision(snake.get()).size() == 0) {
        snake->addMove(make_unique<StraightLine>(dir[choice], snake_speed));
        m->addEntity(std::move(snake));
        break;
      }
    }
  } else {
    while (true) {
      int x = rand() % 75 + 1;
      int y = rand() % 20 + 1;
      unique_ptr<Entity> snake = make_unique<GenericEntity>("Snake", 3, Coordinate{x,y}, looks[choice], 1);
      if (m->getCollision(snake.get()).size() == 0) {
        snake->addMove(make_unique<StraightLine>(dir[choice], snake_speed));
        m->addEntity(std::move(snake));
        break;
      }
    }
  }
}

void CreateBoss1(Model *m) {
  while (true) {
    int x = rand() % 66 + 1;
    int y = rand() % 14 + 1;
    Shape BossLook = Shape({
      {{4,0}, '-'}, {{5,0},'-'}, {{6,0},'-'}, {{7,0},'-'}, {{8,0},'-'},
      {{2,1},'.'}, {{3,1},'\''}, {{4,1},' '}, {{5,1},' '}, {{6,1},' '}, {{7,1},' '}, {{8,1},' '}, {{9,1},'\''}, {{10,1},'.'},
      {{1,2},'/'}, {{2,2},' '}, {{3,2},' '}, {{4,2},'o'}, {{5,2},' '}, {{6,2},' '}, {{7,2},' '}, {{8,2}, 'o'}, {{9,2},' '}, {{10,2},' '}, {{11,2},'\\'},
      {{0,3},'|'}, {{1,3},' '}, {{2,3},' '},{{3,3},' '}, {{4,3},' '}, {{5,3},' '}, {{6,3},' '}, {{7,3},' '}, {{8,3},' '}, {{9,3},' '}, {{10,3},' '}, {{11,3},' '}, {{12,3},'|'},
      {{0,4},'|'}, {{1,4},' '}, {{2,4},' '}, {{3,4},'\\'}, {{4,4},' '}, {{5,4},' '}, {{6,4},' '},  {{7,4},' '}, {{8,4},' '}, {{9,4}, '/'}, {{10,4},' '}, {{11,4},' '},  {{12,4},'|'},
      {{1,5},'\\'}, {{2,5},' '}, {{3,5},' '},  {{4,5},'\''}, {{5,5},'-'}, {{6,5}, '-'}, {{7,5}, '-'}, {{8,5}, '\''}, {{9,5},' '},  {{10,5},' '},  {{11,5},'/'},
      {{2,6},'\''}, {{3,6},'.'}, {{4,6}, '_'}, {{5,6},'_'}, {{6,6},'_'}, {{7,6},'_'}, {{8,6},'_'}, {{9,6},'.'}, {{10,6},'\''},
    }, Color::MAGENTA);
    unique_ptr<Entity> boss = make_unique<GenericEntity>("boss", 20, Coordinate{x,y}, BossLook, 1);
    if (m->getCollision(boss.get()).size() == 0) {
      boss->addMove(make_unique<StraightLine>(Direction::UP, 5));
      boss->addMove(make_unique<StraightLine>(Direction::RIGHT, 3));
      m->addEntity(std::move(boss));
      break;
    }
  }
}

void CreateBoss2(Model *m, Coordinate pos) {
  Shape BossLook = Shape({
    {{4,0}, '-'}, {{5,0},'-'}, {{6,0},'-'}, {{7,0},'-'}, {{8,0},'-'},
    {{2,1},'.'}, {{3,1},'\''}, {{4,1}, 'V'}, {{5,1},'V'}, {{6,1},'V'}, {{7,1},'V'}, {{8,1},'V'}, {{9,1},'\''}, {{10,1},'.'},
    {{1,2},'/'}, {{4,2},'o'}, {{8,2}, 'o'}, {{11,2},'\\'},
    {{0,3},'|'}, {{3,3},'\''}, {{9,3}, '\''}, {{12,3},'|'},
    {{0,4},'|'}, {{4,4},'.'}, {{5,4}, '-'}, {{6,4}, '-'}, {{7,4}, '-'}, {{8,4}, '.'}, {{12,4},'|'},
    {{1,5},'\\'}, {{4,5},'\''}, {{5,5},'-'}, {{6,5}, '-'}, {{7,5}, '-'}, {{8,5}, '\''}, {{11,5},'/'},
    {{2,6},'\''}, {{3,6},'.'}, {{4,6}, '_'}, {{5,6},'_'}, {{6,6},'_'}, {{7,6},'_'}, {{8,6},'_'}, {{9,6},'.'}, {{10,6},'\''},
  }, Color::RED);
  unique_ptr<Entity> boss = make_unique<GenericEntity>("Boss", 30, pos, BossLook, 1);
  boss->addMove(make_unique<StraightLine>(Direction::UP, 3));
  boss->addMove(make_unique<StraightLine>(Direction::RIGHT, 1));
  m->addEntity(std::move(boss));
}

unique_ptr<CurseKeyboard> CreateControl() {
  std::map<int, Action> mapping;
  mapping['w'] = Action::UP;
  mapping['d'] = Action::RIGHT;
  mapping['s'] = Action::DOWN;
  mapping['a'] = Action::LEFT;
  mapping['j'] = Action::SHOOT;
  mapping[KEY_UP] = Action::UP;
  mapping[KEY_RIGHT] = Action::RIGHT;
  mapping[KEY_DOWN] = Action::DOWN;
  mapping[KEY_LEFT] = Action::LEFT;
  return make_unique<CurseKeyboard>(mapping);
}

ConcreteModel_ARLG::ConcreteModel_ARLG(bool developer) : developer{developer} {
  srand(time(NULL));
  int player_health = 5;
  if (developer) { player_health = 500; }
  addController(CreateControl());
  addView(std::move(std::make_unique<Curses>(0, 0, true)));
  addView(std::move(std::make_unique<Curses>(23, 0, false)));
  addCollision(CreateCollision());

  int x = rand() % 78 + 1;
  int y = rand() % 20 + 1;
  unique_ptr<Entity> player = make_unique<GenericEntity>("Player", player_health, Coordinate(x,y), Shape({{Coordinate(0,0), 'p'}}, Color::BLUE), 27);
  player->addMove(make_unique<PlayerControl>());
  player->addMove(make_unique<ForwardShot>(Direction::PLAYER, Action::SHOOT, 1, "P:Bullet", Shape({{Coordinate(0,0), 'O'}}, Color::BLUE), 1));

  addEntity(std::move(player));
  addEntity(createBoard());
}


GameStatus ConcreteModel_ARLG::runLevel(const size_t curLevel) {
  GameStatus status = GameStatus::CONTINUE;
  int SpawnMinion = 0;

  while (status == GameStatus::CONTINUE) {
    Entity * p = *inquireEntities({"Player"}).begin();

    Action cmd = getAction();
    status = moveEntities(cmd);
    string msg1 = "Level " + to_string(curLevel) + "/6";
    string msg2 = "Health: " + to_string((static_cast<GenericEntity *>(p))->getHp());
    updateViews(msg1, StatusLine::ZERO);
    updateViews(msg2, StatusLine::ONE);
  
    if (inquireEntities({"Boss", "boss"}).size() > 0) { ++SpawnMinion; }
    if (SpawnMinion >= int(minion_frequency)) {
      SpawnMinion -= minion_frequency;
      int choice;
      if (inquireEntities({"boss"}).size() > 0) {  choice = rand() % 2; } 
      else if (inquireEntities({"Boss"}).size() > 0) { choice = rand() % 4; }
        
      if (choice == 0) { CreateWalker(this); }
      else if (choice == 1) { CreateStalker(this, p); }
      else if (choice == 2) { CreatePopUp(this); }
      else { CreateSnake(this); }
    }

    string msg3 = "";
    if (inquireEntities({"boss"}).size() > 0) {
      GenericEntity * boss = static_cast<GenericEntity *>(*inquireEntities({"boss"}).begin());
      msg3 = "Boss Health: " + to_string(boss->getHp()) + "/" + to_string(boss->getMaxHp()); 
    } else if (inquireEntities({"Boss"}).size() > 0) { 
      GenericEntity * boss = static_cast<GenericEntity *>(*inquireEntities({"Boss"}).begin());
      msg3 = "Boss Health: " + to_string(boss->getHp()) + "/" + to_string(boss->getMaxHp()); 
    }
    updateViews(msg3, StatusLine::TWO);


    if (p->isDead()) { status = GameStatus::LOSE; }

    for (const auto& fire : inquireEntities({"Fire"})) {
      if (fire->isDead()) {
        int probability = rand() % health_generate;
        if (probability == 0) {
          CreateHealthPk(this, fire->getPos());
        }
      }
    }

    for (const auto& stalker : inquireEntities({"Stalker"})) {
      if (stalker->isDead()) {
        int probability = rand() % walker_generate;
        if (probability == 0) {
          CreateWalker(this, stalker->getPos());
        }
      }
    }

    for (const auto& boss : inquireEntities({"boss"})) {
      if (boss->isDead()) {
        CreateBoss2(this, boss->getPos());
      }
    }

    if ((inquireEntities({"Walker", "Stalker", "Pop-Up", "Snake", "boss", "Boss"}).size() == 0 || developer) && inquireEntities({"exit"}).size() > 0) {
      Entity * exit = *inquireEntities({"exit"}).begin();
      CreateRevealExit(this, exit->getPos());
      eraseEntity(exit);
    }

    clean(&DeadEntity);

    displayViews();
    napms(40);
  }
  return status;
}

int LevelChoice() {
  string choices[6] = {"Level - 1", "Level - 2", "Level - 3", "Level - 4", "Level - 5", "Level - 6 (Boss)"};
  int choice;
  int highlight = 0;

  while (1) {
		for (int i = 0; i < 6; i++) {
			if (i == highlight) {
				attron(A_REVERSE);
			}
			mvprintw(i+3, 3, choices[i].c_str());
			attroff(A_REVERSE);
		}
    mvprintw(1, 3, "Choose the level you want to start with: ");
    mvprintw(15, 3, "Game (ARLG) by Jason Ye.");
		choice = wgetch(stdscr);
			
		if (choice == KEY_UP || choice == 'w' || choice == 'a') {
			highlight--;
			if (highlight == -1) { highlight = 0; }
		} else if (choice == KEY_DOWN || choice == 's' || choice == 'd') {
			highlight++;
			if (highlight == 6) { highlight = 5; }
		}
		if (choice == 10) {
			break;
		}
	}
  return highlight + 1;
}

void ConcreteModel_ARLG::go() {
  GameStatus status = GameStatus::CONTINUE;

  int level = LevelChoice();

  while (level >= 1 && level <= 6) {
    Entity * p = *inquireEntities({"Player"}).begin();
    clean(&TagHealth);
    clean(&TagFire);
    clean(&TagExit);
    clean(&TagEnemy);
    clean(&TagBullet);

    CreateUnrevealExit(this);
    if (1 <= level && level <= 5) {
      int numFire = rand() % (max_fire + 1);
      for (int i = 0; i < numFire; ++i) { CreateFire(this); }
    }

    if (level == 1) {
      int numWalkers = rand() % 6 + 5; 
      for (int i = 0; i < numWalkers; ++i) { CreateWalker(this); }
    } else if (level >= 2 && level <= 5) {
      int totalEnemy = rand() % 6 + 5; 
      int enemy1 = rand() % totalEnemy;
      int enemy2 = totalEnemy - enemy1;
      if (level == 2) {
        for (int i = 0; i < enemy1; ++i) { CreateWalker(this); }
        for (int i = 0; i < enemy2; ++i) { CreateStalker(this, p); }
      } else if (level == 3) {
        for (int i = 0; i < enemy1; ++i) { CreateWalker(this); }
        for (int i = 0; i < enemy2; ++i) { CreatePopUp(this); }
      } else if (level == 4) {
        for (int i = 0; i < enemy1; ++i) { CreateWalker(this); }
        for (int i = 0; i < enemy2; ++i) { CreateSnake(this); }
      } else {
        if (enemy1 == 0) {
          ++enemy1;
          --enemy2;
        } else if (enemy2 == 0) {
          ++enemy2;
          --enemy1;
        }
        int partOne = rand() % enemy1;
        int partTwo = enemy1 - partOne;
        int partThree = rand() % enemy2;
        int partFour = enemy2 - partThree;
        for (int i = 0; i < partOne; ++i) { CreateWalker(this); }
        for (int i = 0; i < partTwo; ++i) { CreateStalker(this, p); }
        for (int i = 0; i < partThree; ++i) { CreatePopUp(this); }
        for (int i = 0; i < partFour; ++i) { CreateSnake(this); }
      }
    } else {
      CreateBoss1(this);
    }

    while (getch() == ERR) {
      updateViews("Are you ready to start?", StatusLine::ZERO);
      updateViews("You can press any key to start!", StatusLine::ONE);
      updateViews("", StatusLine::TWO);
      displayViews();
    }

    status = runLevel(level);
    if (status == GameStatus::WIN) { ++level; }
    else if (status == GameStatus::LOSE) { break; }
  }

  string result;
  if (status == GameStatus::WIN) result = "You win!!";
  else {result = "Sorry, you have lost the combat."; }
  updateViews(result, StatusLine::ZERO);
  updateViews("Please Like, Subscribe, and Comment!!!!", StatusLine::ONE);
  updateViews("", StatusLine::TWO);
  displayViews();

  napms(2000);
  flushinp();
  nodelay(stdscr,false);
  getch();
}