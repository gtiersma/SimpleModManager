//
// Created by Adrien BLANCHET on 21/06/2020.
//

#include "TabGames.h"
#include "FrameModBrowser.h"

#include "GenericToolbox.Switch.h"
#include "GenericToolbox.Vector.h"

#include <StateAlchemist/constants.h>
#include <StateAlchemist/meta_manager.h>
#include <Game.h>

TabGames::TabGames() {

  gameBrowser.init();
  std::vector<Game> gameList = gameBrowser.getGameList();

  if (gameList.empty()) {
    brls::DetailCell* message = new brls::DetailCell();
    message->setText("No game folders have been found.");
    message->setDetailText(
      "To add mods, put them on the SD card in this manner: SD:/mod-alchemy/<title-id-of-the-game>/<group>/<thing-being-replaced>/<mod-name>/<mod-files-and-folders>."
    );
    this->addView(message);
  } else {
     brls::Box* container = new brls::Box();
     this->addView(container);

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
        gameBrowser.selectGame(gameEntry.titleId);
        FrameModBrowser* modsBrowser = new FrameModBrowser();
        brls::Activity* modsActivity = new brls::Activity(modsBrowser);
        brls::Application::pushActivity(modsActivity, brls::TransitionAnimation::SLIDE_LEFT);
        return true;
      });
      item->updateActionHint(brls::BUTTON_A, "Open");

      _gameItems_.emplace_back();
      _gameItems_.back().title = gameEntry.name;
      _gameItems_.back().item = item;
    }

    GenericToolbox::sortVector(_gameItems_, [](const GameItem& a_, const GameItem& b_){
      return GenericToolbox::toLowerCase(a_.title) < GenericToolbox::toLowerCase(b_.title);
    });
  
    // add to the view
    for (auto& game : _gameItems_) { container->addView( game.item ); }
  }
}

brls::View* TabGames::create() { return new TabGames(); }
