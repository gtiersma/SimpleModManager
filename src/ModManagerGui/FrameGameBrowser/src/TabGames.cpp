//
// Created by Adrien BLANCHET on 21/06/2020.
//

#include "TabGames.h"
#include "FrameModBrowser.h"
#include "FrameRoot.h"

#include "GenericToolbox.Switch.h"
#include "GenericToolbox.Vector.h"

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
    _gameItems_.back().item = new brls::Label();
    _gameItems_.back().item->setText(ssTitle.str());

    _gameItems_.emplace_back();
    _gameItems_.back().item = new brls::Label();
    _gameItems_.back().item->setFontSize(15.0f);
    _gameItems_.back().item->setText(ssSubTitle.str());
  } else {
    _gameItems_.reserve(gameList.size());

    for(auto& gameEntry : gameList) {
      std::string gamePath { GenericToolbox::joinPath(ALCHEMIST_FOLDER, MetaManager::getHexTitleId(gameEntry.titleId)) };

      brls::Box* item = new brls::Box();
      item->setFocusable(true);
      item->addGestureRecognizer(new brls::TapGestureRecognizer(item));

      if (gameEntry.icon.size() > 0) {
        brls::Image* image = new brls::Image();
        image->setImageFromMem(gameEntry.icon.data(), 0x20000);
        item->addView(image);
      }

      brls::Label* label = new brls::Label();
      label->setText(gameEntry.name);
      item->addView(label);

      item->registerClickAction([&, gameEntry](View* view) {
        getGameBrowser().selectGame(gameEntry.titleId);
        FrameModBrowser* modsBrowser = new FrameModBrowser(&_owner_->getGuiModManager());
        brls::Activity modsActivity = new brls::Activity(modsBrowser);
        brls::Application::pushActivity(modsActivity, brls::TransitionAnimation::SLIDE_LEFT);
        modsBrowser->registerAction("", brls::Key::PLUS, []{return true;}, true);
        modsBrowser->updateActionHint(brls::Key::PLUS, ""); // make the change visible
      });
      item->updateActionHint(brls::Key::A, "Open");

      _gameItems_.emplace_back();
      _gameItems_.back().title = gameEntry.name;
      _gameItems_.back().item = item;
    }

    GenericToolbox::sortVector(_gameItems_, [](const GameItem& a_, const GameItem& b_){
      return GenericToolbox::toLowerCase(a_.title) < GenericToolbox::toLowerCase(b_.title);
    });
  
    // add to the view
    for (auto& game : _gameItems_) { this->addView( game.item ); }
  }
}

const GameBrowser& TabGames::getGameBrowser() const{
  return _owner_->getGuiModManager().getGameBrowser();
}

GameBrowser& TabGames::getGameBrowser(){
  return _owner_->getGuiModManager().getGameBrowser();
}
