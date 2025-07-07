//
// Created by Nadrino on 03/09/2019.
//

#ifndef SWITCHTEMPLATE_BROWSER_H
#define SWITCHTEMPLATE_BROWSER_H

#include <Selector.h>
#include <ModManager.h>
#include <ConfigHandler.h>
#include <ModsPresetHandler.h>

#include <switch.h>

#include <vector>
#include <string>

struct Game {
  Game() = default;

  /**
   * titleId_ & titleIdLabel_ are the same thing, just in different formats
   * 
   * By default, name is initialized to just the title ID and icon is null
   */
  explicit Game(u64 titleId_, std::string titleIdLabel_) {
    titleId = titleId_;
    icon = new uint8_t[0x20000];
    name = titleIdLabel_;
  }

  ~Game() {
    delete[] icon;
  }

  u64 titleId;
  uint8_t* icon;
  std::string name;
};


class GameBrowser{

public:
  GameBrowser();

  void setIsGameSelected(bool isGameSelected);

  // getters
  bool isGameSelected() const;
  const ConfigHandler &getConfigHandler() const;
  ModManager &getModManager();
  ModsPresetHandler &getModPresetHandler();
  ConfigHandler &getConfigHandler();
  std::vector<Game> &getGameList();

  // browse
  void selectGame(const u64 &gameName_);

  // IO
  void scanInputs(u64 kDown, u64 kHeld);

  // utils -> move to gui lib??
  uint8_t* getFolderIcon(const std::string& gameFolder_);

protected:
  void init();

private:
  bool _isGameSelected_{false};

  Selector _selector_;
  ModManager _modManager_{this};
  ConfigHandler _configHandler_;
  ModsPresetHandler _modPresetHandler_;

  std::vector<Game> _gameList_;

};

#endif //SWITCHTEMPLATE_BROWSER_H
