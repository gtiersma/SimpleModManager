//
// Created by Adrien BLANCHET on 21/06/2020.
//

#ifndef SIMPLEMODMANAGER_TABGAMES_H
#define SIMPLEMODMANAGER_TABGAMES_H


#include "icon_cell.hpp"
#include "GameBrowser.h"

#include "borealis.hpp"

#include <vector>


struct GameItem;

class TabGames : public brls::ScrollingFrame {

public:
  explicit TabGames();

  brls::IconCell* buildGameCell(const Game& game);

  static brls::View* create();

private:
  std::vector<GameItem> _gameItems_;

};

struct GameItem{
  std::string title{};
  brls::Box* item{nullptr};
};


#endif //SIMPLEMODMANAGER_TABGAMES_H
