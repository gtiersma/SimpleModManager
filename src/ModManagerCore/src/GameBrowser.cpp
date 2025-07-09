//
// Created by Nadrino on 03/09/2019.
//

#include <GameBrowser.h>
#include <Toolbox.h>


#include "GenericToolbox.Switch.h"
#include "GenericToolbox.Vector.h"

#include <switch.h>

#include <iostream>
#include <algorithm>
#include <utility>
#include <cstring>
#include <StateAlchemist/controller.h>
#include <StateAlchemist/meta_manager.h>
#include <StateAlchemist/constants.h>
#include <AlchemistLogger.h>
#include <Game.h>


GameBrowser::GameBrowser(){ this->init(); }

void GameBrowser::setIsGameSelected(bool isGameSelected) {
  _isGameSelected_ = isGameSelected;
}

// getters
bool GameBrowser::isGameSelected() const {
  return _isGameSelected_;
}
const ConfigHandler &GameBrowser::getConfigHandler() const {
  return _configHandler_;
}
ModManager &GameBrowser::getModManager(){
  return _modManager_;
}
ConfigHandler &GameBrowser::getConfigHandler(){
  return _configHandler_;
}
ModsPresetHandler &GameBrowser::getModPresetHandler(){
  return _modPresetHandler_;
}
std::vector<Game> &GameBrowser::getGameList(){
  return _gameList_;
}

// Browse
void GameBrowser::selectGame(const u64 &titleId_) {
  alchemyLogger.log("GameBrowser::selectGame();");
  controller.setTitleId(titleId_);
  alchemyLogger.log("GAME BROWSER: set title ID: " + controller.getHexTitleId());
  _isGameSelected_ = true;
}

uint8_t* GameBrowser::getFolderIcon(const std::string& gameFolder_){
  alchemyLogger.log("GameBrowser::getFolderIcon();");
  if( _isGameSelected_ ){ return nullptr; }
  uint8_t* icon = GenericToolbox::Switch::Utils::getIconFromTitleId(gameFolder_);
  return icon;
}

// protected
void GameBrowser::init(){
  alchemyLogger.log("GameBrowser::init();");

  auto folderList = GenericToolbox::lsDirs(ALCHEMIST_PATH);
  
  // Filter out any folders that are definitely no Switch Title ID:
  for (auto& folder : folderList) {
    if (MetaManager::isTitleId(folder)) {
      u64 titleId = MetaManager::getNumericTitleId(folder);
      Game game(titleId, folder);

      size_t iconSize = 0;
      NsApplicationControlData gameData;
      MetaManager::tryResult(
        nsGetApplicationControlData(NsApplicationControlSource_Storage, titleId, &gameData, sizeof(gameData), &iconSize)
      );
      game.setIcon(gameData.icon, iconSize - sizeof(gameData.nacp));

      NacpLanguageEntry* nameData;
      MetaManager::tryResult(nsGetApplicationDesiredLanguage(&gameData.nacp, &nameData));
      game.name = nameData->name;
      delete[] nameData;

      _gameList_.push_back(game);
    }
  }
}

