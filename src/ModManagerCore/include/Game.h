#ifndef SIMPLEMODMANAGER_GAME_H
#define SIMPLEMODMANAGER_GAME_H

#include <switch.h>
#include <string>
#include <vector>

class Game {
  public:
    Game(u64 titleId_, std::string titleIdLabel_);

    u64 titleId;
    std::string name;
    std::vector<u8> icon;
};

#endif //SIMPLEMODMANAGER_GAME_H