#include "concreteModel3.h"

unique_ptr<Entity> createBoard() {
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

bool DeadEntity(Entity *e) {
  return e->isDead();
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

ConcreteModel_HEIGHT::ConcreteModel_HEIGHT() {
  addController(CreateControl());
  addView(std::move(std::make_unique<Curses>(0, 0, true)));
  addView(std::move(std::make_unique<Curses>(23, 0, false)));

  map<pair<string, string>, pair<vector<CollideType>, vector<CollideType>>> t{};
  t[make_pair("WALL", "*")] = make_pair<vector<CollideType>, vector<CollideType>> ({}, {CollideType::BOUNCE});
  t[make_pair("Player", "Win")] = make_pair<vector<CollideType>, vector<CollideType>> ({CollideType::WIN}, {CollideType::DESTROY}); 
  t[make_pair("Player", "Lose")] = make_pair<vector<CollideType>, vector<CollideType>> ({CollideType::LOSE}, {CollideType::DESTROY}); 
  t[make_pair("WALL", "P:Bullet")] = make_pair<vector<CollideType>, vector<CollideType>> ({}, {CollideType::DESTROY}); 
  addCollision(std::move(make_unique<ConcreteCollision>(t)));


  unique_ptr<Entity> player = make_unique<WallEntity>("Player", Coordinate {60,16}, Shape({{Coordinate(0,0), '@'}}, Color::CYAN), 10);
  player->addMove(make_unique<PlayerControl>());
  player->addMove(make_unique<Gravatate>(Direction::DOWN, 6));
  player->addMove(make_unique<StraightLine>(Direction::UP, 3));
  player->addMove(make_unique<ForwardShot>(Direction::PLAYER, Action::SHOOT, 1, "P:Bullet", Shape({{Coordinate(0,0), 'O'}}, Color::CYAN), 1));

  deque<Shape> newlook {Shape({{Coordinate(0,0), '^'}, {Coordinate(0,-1), '^'}}, Color::RED), Shape({{Coordinate(0,0), '+'}, {Coordinate(-1,-1), '+'}, {Coordinate(1,-1), '+'}}, Color::GREEN)};
  player->addMove(make_unique<Transform>(newlook, 100));


  unique_ptr<Entity> win = make_unique<WallEntity>("Win", Coordinate{3, 2}, Shape({{Coordinate(0,0), '*'}}, Color::YELLOW), 5);
  unique_ptr<Entity> lose = make_unique<WallEntity>("Lose", Coordinate{76, 19}, Shape({{Coordinate(0,0), '!'}}, Color::RED), 5);

  unique_ptr<Entity> plate1 = make_unique<WallEntity>("Plate1", Coordinate(10,10), 
  Shape({{Coordinate(0,0), '#'}, {Coordinate(1,0), '#'}, {Coordinate(2,0), '#'},  {Coordinate(0,1), '#'}, {Coordinate(1,1), '#'}, {Coordinate(2,1), '#'}, 
  {Coordinate(0,2), '#'}, {Coordinate(1,2), '#'},  {Coordinate(2,2), '#'}},  Color::MAGENTA), 6);
  plate1->addMove(make_unique<StraightLine>(Direction::LEFT, 5));

  
  unique_ptr<Entity> plate2 = make_unique<WallEntity>("Plate2", Coordinate(30,10), 
  Shape({{Coordinate(0,0), '='}, {Coordinate(1,0), '='}, {Coordinate(2,0), '='},  {Coordinate(0,1), '='}, {Coordinate(1,1), '='}, {Coordinate(2,1), '='}, 
  {Coordinate(0,2), '='}, {Coordinate(1,2), '='},  {Coordinate(2,2), '='}},  Color::BLUE), 7);
  plate2->addMove(make_unique<StraightLine>(Direction::RIGHT, 5));

  unique_ptr<Entity> triangle =  make_unique<WallEntity>("Triangle", Coordinate(20,15), 
  Shape({{Coordinate(2,0), '^'}, {Coordinate(1,1), '/'}, {Coordinate(3,1), '\\'},  {Coordinate(0,2), '/'}, {Coordinate(4,2), '\\'}},  Color::WHITE), 8);
  triangle->addMove(make_unique<StraightLine>(Direction::DOWN, 5));
  
  unique_ptr<Entity> ball1 = make_unique<WallEntity>("Ball", Coordinate(50,10), Shape({{{0,0}, 'O'}}), 20);
  ball1->addMove(make_unique<StraightLine>(Direction::UP, 1));
  unique_ptr<Entity> ball2 = make_unique<WallEntity>("Ball", Coordinate(52,10), Shape({{{0,0}, 'O'}}), 20);
  ball2->addMove(make_unique<StraightLine>(Direction::UP, 2));
  unique_ptr<Entity> ball3 = make_unique<WallEntity>("Ball", Coordinate(54,10), Shape({{{0,0}, 'O'}}), 20);
  ball3->addMove(make_unique<StraightLine>(Direction::UP, 3));
  unique_ptr<Entity> ball4 = make_unique<WallEntity>("Ball", Coordinate(56,10), Shape({{{0,0}, 'O'}}), 20);
  ball4->addMove(make_unique<StraightLine>(Direction::UP, 4));


  addEntity(createBoard());
  addEntity(std::move(player));
  addEntity(std::move(win));
  addEntity(std::move(lose));
  addEntity(std::move(plate1));
  addEntity(std::move(plate2));
  addEntity(std::move(triangle));
  addEntity(std::move(ball1));
  addEntity(std::move(ball2));
  addEntity(std::move(ball3));
  addEntity(std::move(ball4));
}

void ConcreteModel_HEIGHT::go() {
  GameStatus status = GameStatus::CONTINUE;
  while (status == GameStatus::CONTINUE) {

    Action cmd = getAction();
    status = moveEntities(cmd);
    clean(&DeadEntity);
    displayViews();
    napms(40);
  }
  string result;
  if (status == GameStatus::WIN) result = "You win!";
  else {result = "You lose"; }
  updateViews(result, StatusLine::ZERO);
  displayViews();

  nodelay(stdscr,false);
  getch();
}