#include "concreteModel2.h"
const size_t bullet_speed = 2;

const size_t level_max = 7;

const size_t shoot_frequency_player = 3;
const size_t shoot_frequency_big = 50;
const size_t shoot_frequency_medium = 100;
const size_t shoot_frequency_small = 200;
const size_t move_frequency = 18;

const size_t initial_grava = 1035;
const size_t grava_inc_per_level = 60;
const size_t shoot_inc_per_level = 2;
const size_t move_inc_per_level = 1;

bool DeadEntity(Entity *e) {
  return e->isDead();
}
bool Bullet(Entity *e) {
  string s = e->getTag();
  return s == "P:Bullet" || s == "E:Bullet1" || s == "E:Bullet2" || s == "E:Bullet3";
}
Shape bullet_enemy = Shape {{
  {{0,0}, '|'}
}};

Shape bullet_player = Shape {{
  {{0,0}, '|'}
}, Color::GREEN};

Shape fence_look = Shape {{{{0,0}, '#'}}};


bool TagOutside20(Entity *e) {
  return (e->getOutsideBoundary()) >= 20;
}

unique_ptr<Entity> createBoard() {
  const int cols = 71;
  const int rows = 22;
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
  t[make_pair("P:Bullet", "WALL")] = make_pair<vector<CollideType>, vector<CollideType>> ({}, {});

  t[make_pair("P:Bullet", "Big Minion")] = make_pair<vector<CollideType>, vector<CollideType>> ({CollideType::DESTROY}, {CollideType::DESTROY});
  t[make_pair("P:Bullet", "Medium Minion")] = make_pair<vector<CollideType>, vector<CollideType>> ({CollideType::DESTROY}, {CollideType::DESTROY});
  t[make_pair("P:Bullet", "Small Minion")] = make_pair<vector<CollideType>, vector<CollideType>> ({CollideType::DESTROY}, {CollideType::DESTROY});

  t[make_pair("E:Bullet1", "WALL")] = make_pair<vector<CollideType>, vector<CollideType>> ({}, {});
  t[make_pair("E:Bullet2", "WALL")] = make_pair<vector<CollideType>, vector<CollideType>> ({}, {});
  t[make_pair("E:Bullet3", "WALL")] = make_pair<vector<CollideType>, vector<CollideType>> ({}, {});

  t[make_pair("WALL", "Big Minion")] = make_pair<vector<CollideType>, vector<CollideType>> ({}, {CollideType::STOP, CollideType::BOUNCE}); 
  t[make_pair("WALL", "Medium Minion")] = make_pair<vector<CollideType>, vector<CollideType>> ({}, {CollideType::STOP, CollideType::BOUNCE}); 
  t[make_pair("WALL", "Small Minion")] = make_pair<vector<CollideType>, vector<CollideType>> ({}, {CollideType::STOP, CollideType::BOUNCE}); 

  t[make_pair("E:Bullet1", "Medium Minion")] = make_pair<vector<CollideType>, vector<CollideType>> ({CollideType::DESTROY}, {});  
  t[make_pair("E:Bullet1", "Small Minion")] = make_pair<vector<CollideType>, vector<CollideType>> ({CollideType::DESTROY}, {});  
  t[make_pair("E:Bullet2", "Small Minion")] = make_pair<vector<CollideType>, vector<CollideType>> ({CollideType::DESTROY}, {}); 

  t[make_pair("E:Bullet1", "Fence")] = make_pair<vector<CollideType>, vector<CollideType>> ({CollideType::DESTROY}, {CollideType::DAMAGE});  
  t[make_pair("E:Bullet2", "Fence")] = make_pair<vector<CollideType>, vector<CollideType>> ({CollideType::DESTROY}, {CollideType::DAMAGE});  
  t[make_pair("E:Bullet3", "Fence")] = make_pair<vector<CollideType>, vector<CollideType>> ({CollideType::DESTROY}, {CollideType::DAMAGE});  
  t[make_pair("P:Bullet", "Fence")] = make_pair<vector<CollideType>, vector<CollideType>> ({CollideType::DESTROY}, {CollideType::DAMAGE}); 

  t[make_pair("E:Bullet1", "Player")] = make_pair<vector<CollideType>, vector<CollideType>> ({CollideType::DESTROY}, {CollideType::DESTROY});  
  t[make_pair("E:Bullet2", "Player")] = make_pair<vector<CollideType>, vector<CollideType>> ({CollideType::DESTROY}, {CollideType::DESTROY});  
  t[make_pair("E:Bullet3", "Player")] = make_pair<vector<CollideType>, vector<CollideType>> ({CollideType::DESTROY}, {CollideType::DESTROY});  

  t[make_pair("Big Minion", "Big Minion")] = make_pair<vector<CollideType>, vector<CollideType>> ({CollideType::STOP, CollideType::BOUNCE}, {CollideType::STOP, CollideType::BOUNCE}); 
  t[make_pair("Medium Minion", "Medium Minion")] = make_pair<vector<CollideType>, vector<CollideType>> ({CollideType::STOP, CollideType::BOUNCE}, {CollideType::STOP, CollideType::BOUNCE}); 
  t[make_pair("Small Minion", "Small Minion")] = make_pair<vector<CollideType>, vector<CollideType>> ({CollideType::STOP, CollideType::BOUNCE}, {CollideType::STOP, CollideType::BOUNCE}); 
  return make_unique<ConcreteCollision>(t);
}

void CreateBigMinion(Model *m, Coordinate pos, Direction d, size_t level) {
  if (d != Direction::LEFT && d != Direction::RIGHT) { return; }
  Shape look = {{
    {{-1,0}, '-'}, {{0,0}, '^'}, {{1,0}, '-'},
    {{-1,1}, '/'}, {{0,1}, '-'}, {{1,1}, '\\'}
  }, Color::RED};
  Shape alter = {{
    {{-1,0}, '-'}, {{0,0}, '^'}, {{1,0}, '-'},
    {{-1,1}, '<'}, {{0,1}, ' '}, {{1,1}, '>'}
  }, Color::RED};
  unique_ptr<Entity> bigMinion = make_unique<WallEntity>("Big Minion", pos, look, 5);
  bigMinion->addMove(make_unique<Transform>(deque<Shape> {alter}, move_frequency - level * move_inc_per_level));
  bigMinion->addMove(make_unique<StraightLine>(d, move_frequency - level * move_inc_per_level));
  size_t shoot = (rand() % (shoot_frequency_big / 2)) + shoot_frequency_big - level * shoot_inc_per_level;
  bigMinion->addMove(make_unique<ForwardShot>(Direction::DOWN, Action::ALL, shoot, "E:Bullet1", bullet_enemy, bullet_speed));
  bigMinion->addMove(make_unique<Gravatate>(Direction::DOWN, initial_grava + 50 - level * grava_inc_per_level));
  m->addEntity(std::move(bigMinion));
}

void CreateMediumMinion(Model *m, Coordinate pos, Direction d, size_t level) {
  if (d != Direction::LEFT && d != Direction::RIGHT) { return; }
  Shape look = {{
    {{-1,0}, '{'}, {{0,0}, '"'}, {{1,0}, '}'},
    {{-1,1}, '/'}, {{0,1}, ' '}, {{1,1}, '\\'}
  }, Color::BLUE};
  Shape alter = {{
    {{-1,0}, ':'}, {{0,0}, '\''}, {{1,0}, ':'},
    {{-1,1}, '<'}, {{0,1}, ' '}, {{1,1}, '>'}
  }, Color::BLUE};
  unique_ptr<Entity> MediumMinion = make_unique<WallEntity>("Medium Minion", pos, look, 5);
  MediumMinion->addMove(make_unique<Transform>(deque<Shape> {alter}, move_frequency - level * move_inc_per_level));
  MediumMinion->addMove(make_unique<StraightLine>(d, move_frequency - level * move_inc_per_level));
  size_t shoot = (rand() % (shoot_frequency_medium / 2)) + shoot_frequency_medium - level * shoot_inc_per_level;
  MediumMinion->addMove(make_unique<ForwardShot>(Direction::DOWN, Action::ALL, shoot, "E:Bullet2", bullet_enemy, bullet_speed));
  MediumMinion->addMove(make_unique<Gravatate>(Direction::DOWN, initial_grava + 50 - level * grava_inc_per_level));
  m->addEntity(std::move(MediumMinion));
}


void CreateSmallMinion(Model *m, Coordinate pos, Direction d, size_t level) {
  if (d != Direction::LEFT && d != Direction::RIGHT) { return; }
  Shape look = {{
    {{-1,0}, '<'}, {{0,0}, '+'}, {{1,0}, '>'},
    {{-1,1}, '/'}, {{0,1}, '-'}, {{1,1}, '\\'}
  }};
  Shape alter = {{
    {{-1,0}, '<'}, {{0,0}, '+'}, {{1,0}, '>'},
    {{-1,1}, '-'}, {{0,1}, '-'}, {{1,1}, '-'}
  }};
  unique_ptr<Entity> SmallMinion = make_unique<WallEntity>("Small Minion", pos, look, 5);
  SmallMinion->addMove(make_unique<Transform>(deque<Shape> {alter}, move_frequency - level * move_inc_per_level));
  SmallMinion->addMove(make_unique<StraightLine>(d, move_frequency - level * move_inc_per_level));
  size_t shoot = (rand() % (shoot_frequency_small / 2)) + shoot_frequency_small- level * shoot_inc_per_level;
  SmallMinion->addMove(make_unique<ForwardShot>(Direction::DOWN, Action::ALL, shoot, "E:Bullet3", bullet_enemy, bullet_speed));
  SmallMinion->addMove(make_unique<Gravatate>(Direction::DOWN, initial_grava + 50 - level * grava_inc_per_level));
  m->addEntity(std::move(SmallMinion));
}

void CreatePlayer(Model *m) {
  Shape look = {{
    {{-1,0}, '/'}, {{0,0}, '|'}, {{1,0}, '\\'},
    {{-2,1},'<'}, {{-1,1}, '#'}, {{0,1}, '='}, {{1,1}, '#'}, {{2,1},'>'} 
  }, Color::GREEN};
  unique_ptr<Entity> player = make_unique<WallEntity>("Player", Coordinate(3,19), look, 2);
  player->addMove(make_unique<PlayerControl>());
  player->addMove(make_unique<ForwardShot>(Direction::UP, Action::SHOOT, shoot_frequency_player, "P:Bullet", bullet_player, bullet_speed));
  m->addEntity(std::move(player));
}



void ConcreteModel_SPACE::CreateFence() {
  for (int i = 0; i < 3; ++i)
    for (int j = 6 + i * 21; j < (i + 1) * 21; ++j) {
      unique_ptr<Entity> fence1 = make_unique<GenericEntity>("Fence", 4, Coordinate(j,18), fence_look, 0);
      unique_ptr<Entity> fence2 = make_unique<GenericEntity>("Fence", 4, Coordinate(j,17), fence_look, 0);
      addEntity(std::move(fence1));
      addEntity(std::move(fence2));
    }
}
void ConcreteModel_SPACE::CreateArmy(size_t level) {
  Direction dirs[2] = {Direction::LEFT, Direction::RIGHT}; 
  for (int i = 0; i < 10; ++i) {
    Direction d = dirs[i % 2];
    int x = (2 * i + 1) * 3 + 1;
    CreateBigMinion(this, Coordinate(x, level + 1), d, level);
    CreateMediumMinion(this, Coordinate(x, level + 3), d, level);
    CreateSmallMinion(this, Coordinate(x, level + 5), d, level);
  }
}

unique_ptr<CurseKeyboard> CreateControl() {
  std::map<int, Action> mapping;
  mapping[KEY_LEFT] = Action::LEFT;
  mapping[KEY_RIGHT] = Action::RIGHT;
  mapping[' '] = Action::SHOOT;
  return make_unique<CurseKeyboard>(mapping);
}


ConcreteModel_SPACE::ConcreteModel_SPACE() {
  srand(time(NULL));
  addController(CreateControl());
  addView(std::move(std::make_unique<Curses>(0, 0, true)));
  addView(std::move(std::make_unique<Curses>(23, 0, false)));
  addCollision(CreateCollision());

  addEntity(std::move(createBoard()));

}




int LevelChoice() {
  string choices[4] = {"Diffculty : Normal", "Diffculty : Hard", "Diffculty : Even Harder", "Diffculty : Harder than HARDEEEEEEER"};
  int choice;
  int highlight = 0;

  while (1) {
		for (int i = 0; i < 4; i++) {
			if (i == highlight) {
				attron(A_REVERSE);
			}
			mvprintw(i+3, 3, choices[i].c_str());
			attroff(A_REVERSE);
		}
    mvprintw(1, 3, "Choose a difficulty to start with: ");
    mvprintw(12, 3, "Game (Space Invaders) by Jason Ye.");
		choice = wgetch(stdscr);
			
		if (choice == KEY_UP || choice == 'w' || choice == 'a') {
			highlight--;
			if (highlight == -1) { highlight = 0; }
		} else if (choice == KEY_DOWN || choice == 's' || choice == 'd') {
			highlight++;
			if (highlight == 4) { highlight = 3; }
		}
		if (choice == 10) {
			break;
		}
	}
  return 3 * highlight;
}




void ConcreteModel_SPACE::go() {
  GameStatus status = GameStatus::CONTINUE;
  CreatePlayer(this);

  int level = LevelChoice();
  if (level == 9) {
    level = 6;
  } else {
    CreateFence();
  }
  size_t score = 0;
  int life = 3;

  while (status != GameStatus::LOSE) {
    if (status == GameStatus::WIN) {
      status = GameStatus::CONTINUE;
      ++life;
      if (level <= int(level_max)) { ++level; }
      clean(&DeadEntity);
      clean(&Bullet);
    }

    CreateArmy(level);

    while (getch() == ERR) {
      updateViews("Are you ready to start?", StatusLine::ZERO);
      updateViews("You can press any key to start!", StatusLine::ONE);
      updateViews("", StatusLine::TWO);
      displayViews();
    }

    int tick = 0;
    int incTickTime = initial_grava - level * grava_inc_per_level;
    while (status == GameStatus::CONTINUE) {
      ++tick;
      if (tick >= incTickTime) {
        tick -= incTickTime;
        if (incTickTime > int(grava_inc_per_level)) {
          incTickTime -= grava_inc_per_level;
          for (auto &enemy : inquireEntities({"Big Minion", "Medium Minion", "Small Minion"})) {
            for (auto &move : enemy->getMove()) {
              string s = move->getTag();
              if (s == "StraightLine" || s == "Transform") {
                move->incFrequency(move_inc_per_level);
              } else if (s == "ForwardShot") {
                move->incFrequency(shoot_inc_per_level);
              } else if (s == "Gravatate") {
                move->incFrequency(grava_inc_per_level);
              }
            }
          }
        }
      }

      Entity * p = *inquireEntities({"Player"}).begin();

      Action cmd = getAction();
      status = moveEntities(cmd);

      for (auto& fence : inquireEntities({"Fence"})) {
        GenericEntity *e = static_cast<GenericEntity *> (fence);
        if (e->getHp() == 3) { e->setShape(Shape{{{{0,0}, '+'}}}); }
        else if (e->getHp() == 2) { e->setShape(Shape{{{{0,0}, '='}}}); }
        else if (e->getHp() == 1) { e->setShape(Shape{{{{0,0}, '.'}}}); }
      }

      for (auto & enemy : inquireEntities({"Big Minion", "Medium Minion", "Small Minion"})) {
        if (enemy->getPos().y > 17) {
          status = GameStatus::LOSE;
        }
        if (enemy->isDead()) {
          if (enemy->getTag() == "Big Minion") { score += 40; }
          else if (enemy->getTag() == "Medium Minion") { score += 20; }
          else { score += 10; }
        }
      }

      if (p->isDead()) { 
        --life;
        clean(&DeadEntity);
        clean(&Bullet);
        if (life >= 0) {
          CreatePlayer(this);
          while (getch() == ERR) {
              updateViews("Got hit! Press any key to Continue!", StatusLine::ZERO);
              updateViews("", StatusLine::ONE);
              updateViews("", StatusLine::TWO);
              displayViews();
          }
        } else {
          status = GameStatus::LOSE;
        }
      }

      if (inquireEntities({"Big Minion", "Medium Minion", "Small Minion"}).size() == 0) {
        status = GameStatus::WIN;
      }
    
      string msg1 = "Remaining Respawn: " + to_string(life);
      string msg2 = "Total Score: " + to_string(score);
      updateViews(msg1, StatusLine::ZERO);
      updateViews(msg2, StatusLine::ONE);

      clean(&TagOutside20);
      clean(&DeadEntity);
      displayViews();
      napms(40);
    }
  }
  string result;
  updateViews("Combat has finished!!", StatusLine::ZERO);
  string msg2 = "Final Score: " + to_string(score);
  updateViews(msg2, StatusLine::ONE);
  updateViews("", StatusLine::TWO);
  displayViews();

  napms(2000);
  flushinp();
  nodelay(stdscr,false);
  getch();
}