#include <Game.h>

Game::Game(u64 titleId_, std::string titleIdLabel_): titleId(titleId_) {
    name = titleIdLabel_;
}