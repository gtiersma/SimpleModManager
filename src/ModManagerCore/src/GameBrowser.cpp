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
  controller.setTitleId(titleId_);
  _isGameSelected_ = true;
}

uint8_t* GameBrowser::getFolderIcon(const std::string& gameFolder_){
  if( _isGameSelected_ ){ return nullptr; }
  uint8_t* icon = GenericToolbox::Switch::Utils::getIconFromTitleId(gameFolder_);
  return icon;
}

// protected
void GameBrowser::init(){
  auto folderList = GenericToolbox::lsDirs(ALCHEMIST_PATH);
  
  // Filter out any folders that are definitely no Switch Title ID:
  for (auto& folder : folderList) {
    if (MetaManager::isTitleId(folder)) {
      u64 titleId = MetaManager::getNumericTitleId(folder);
      Game game(titleId, folder);

      NsApplicationControlData gameData;
      if (R_SUCCEEDED(nsGetApplicationControlData(NsApplicationControlSource_Storage, titleId, &gameData, sizeof(gameData), nullptr))) {
        memcpy(game.icon, gameData.icon, 0x20000);
      }

      NacpLanguageEntry* nameData;
      if (R_SUCCEEDED(nsGetApplicationDesiredLanguage(&gameData.nacp, &nameData))) {
        game.name = nameData->name;
      }

      _gameList_.push_back(game);
    }
  }
}

