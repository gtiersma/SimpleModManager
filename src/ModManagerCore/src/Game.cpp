#include <Game.h>
#include <cstring>

Game::Game(u64 titleId_, std::string titleIdLabel_): titleId(titleId_) {
    name = titleIdLabel_;
}

Game::~Game() { delete[] icon; }
u8* Game::getIcon() { return icon; }

void Game::setIcon(u8* newIcon, size_t size) {
    if (icon != nullptr) delete[] icon;

    icon = new u8[size];
    memcpy(icon, newIcon, size);
}