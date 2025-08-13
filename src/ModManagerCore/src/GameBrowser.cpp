//
// Created by Nadrino on 03/09/2019.
//

#include <GameBrowser.h>

#include <switch.h>

#include <StateAlchemist/controller.h>
#include <StateAlchemist/meta_manager.h>
#include <StateAlchemist/constants.h>
#include <Game.h>
#include <GenericToolbox.Fs.h>

GameBrowser gameBrowser;

GameBrowser::GameBrowser(){ this->init(); }

// getters
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

std::optional<Game> GameBrowser::getGame(const u64 &titleId_) {
  for (auto& game : _gameList_) {
    if (game.titleId == titleId_) {
      return game;
    }
  }
  
  return std::nullopt;
}

// Browse
void GameBrowser::selectGame(const u64 &titleId_) {
  controller.setTitleId(titleId_);
}

// protected
void GameBrowser::init(){
  auto folderList = GenericToolbox::lsDirs(ALCHEMIST_PATH);
  
  // Filter out any folders that are definitely no Switch Title ID:
  for (auto& folder : folderList) {
    if (MetaManager::isTitleId(folder)) {
      u64 titleId = MetaManager::getNumericTitleId(folder);
      auto game = std::make_unique<Game>(titleId, folder);

      // Load the icon for the game:
      u64 gameDataSize {};
      auto gameData = std::make_unique<NsApplicationControlData>();
      if (
        R_SUCCEEDED(
          nsGetApplicationControlData(
            NsApplicationControlSource_Storage,
            titleId,
            gameData.get(),
            sizeof(NsApplicationControlData),
            &gameDataSize
          )
        )
      ) {
        const auto iconSize = gameDataSize - sizeof(NacpStruct);
        game->icon.resize(iconSize);
        std::memcpy(game->icon.data(), gameData->icon, game->icon.size());
      }

      // Load the title of the game:
      NacpLanguageEntry* nameData;
      if (R_SUCCEEDED(nsGetApplicationDesiredLanguage(&gameData->nacp, &nameData))) {
        game->name = nameData->name;
      }

      _gameList_.push_back(std::move(*game));
    }
  }
}

