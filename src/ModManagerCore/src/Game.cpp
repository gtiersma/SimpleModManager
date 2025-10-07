#include <Game.h>

Game::Game(u64 titleId_, std::string titleIdLabel_): titleId(titleId_) {
    // Set the name after the constructor
    // By default, the name is just the title ID if never updated.
    name = titleIdLabel_;
}