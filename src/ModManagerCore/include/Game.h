#ifndef SIMPLEMODMANAGER_GAME_H
#define SIMPLEMODMANAGER_GAME_H

#include <switch.h>
#include <string>

class Game {
  public:
    Game(u64 titleId_, std::string titleIdLabel_);
    ~Game();

    u64 titleId;
    std::string name;

    u8* getIcon();
    void setIcon(u8* icon, size_t size);

  private:
    u8* icon;
};

#endif //SIMPLEMODMANAGER_GAME_H