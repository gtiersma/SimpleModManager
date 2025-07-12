//
// Created by Nadrino on 03/09/2019.
//

#ifndef SWITCHTEMPLATE_BROWSER_H
#define SWITCHTEMPLATE_BROWSER_H

#include <ModManager.h>
#include <ConfigHandler.h>
#include <ModsPresetHandler.h>
#include <Game.h>

#include <switch.h>

#include <vector>
#include <string>
#include <optional>


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

  std::optional<Game> getGame(const u64 &titleId_);

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

  ModManager _modManager_{this};
  ConfigHandler _configHandler_;
  ModsPresetHandler _modPresetHandler_;

  std::vector<Game> _gameList_;

};

#endif //SWITCHTEMPLATE_BROWSER_H
