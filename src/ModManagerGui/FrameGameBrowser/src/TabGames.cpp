//
// Created by Adrien BLANCHET on 21/06/2020.
//

#include "TabGames.h"
#include "FrameModBrowser.h"
#include "FrameRoot.h"

#include "GenericToolbox.Switch.h"
#include "GenericToolbox.Vector.h"
#include "Logger.h"

#include <sstream>
#include <StateAlchemist/constants.h>
#include <StateAlchemist/meta_manager.h>
#include <Game.h>

TabGames::TabGames(FrameRoot* owner_) : _owner_(owner_) {
  std::vector<Game> gameList = this->getGameBrowser().getGameList();

  if (gameList.empty()) {
    std::stringstream ssTitle;
    std::stringstream ssSubTitle;

    ssTitle << "No game folders have been found.";

    ssSubTitle << "- To add mods, put them on the SD card in this manner: ";
    ssSubTitle << "SD:/mod-alchemy/<title-id-of-the-game>/<group>/<thing-is-replacing>/<mod-name>/<mods-files-and-folders>." << std::endl;

    _gameItems_.emplace_back();
    _gameItems_.back().item = new brls::ListItem( ssTitle.str(), ssSubTitle.str() );
    _gameItems_.back().item->show([](){}, false);
  } else {
    _gameItems_.reserve(gameList.size());

    for(auto& gameEntry : gameList) {
      std::string gamePath { GenericToolbox::joinPath(ALCHEMIST_FOLDER, MetaManager::getHexTitleId(gameEntry.titleId)) };

      // memory allocation
      auto* item = new brls::ListItem(gameEntry.name, "", "");

      if (gameEntry.icon.size() > 0) {
        item->setThumbnail(gameEntry.icon.data(), 0x20000);
      }

      item->getClickEvent()->subscribe([&, gameEntry](View* view) {
        getGameBrowser().selectGame(gameEntry.titleId);
        auto* modsBrowser = new FrameModBrowser( &_owner_->getGuiModManager() );
        brls::Application::pushView(modsBrowser, brls::ViewAnimation::SLIDE_LEFT);
        modsBrowser->registerAction("", brls::Key::PLUS, []{return true;}, true);
        modsBrowser->updateActionHint(brls::Key::PLUS, ""); // make the change visible
      });
      item->updateActionHint(brls::Key::A, "Open");


      _gameItems_.emplace_back();
      _gameItems_.back().title = gameEntry.name;
      _gameItems_.back().item = item;
    }
  }

  GenericToolbox::sortVector(_gameItems_, [](const GameItem& a_, const GameItem& b_){
    return GenericToolbox::toLowerCase(a_.title) < GenericToolbox::toLowerCase(b_.title);
  });

  // add to the view
  for (auto& game : _gameItems_) { this->addView( game.item ); }
}

const GameBrowser& TabGames::getGameBrowser() const{
  return _owner_->getGuiModManager().getGameBrowser();
}

GameBrowser& TabGames::getGameBrowser(){
  return _owner_->getGuiModManager().getGameBrowser();
}
