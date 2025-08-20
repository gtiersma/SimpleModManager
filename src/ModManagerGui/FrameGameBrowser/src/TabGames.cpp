//
// Created by Adrien BLANCHET on 21/06/2020.
//

#include "TabGames.h"
#include "FrameModBrowser.h"
#include "icon_cell.hpp"

#include "GenericToolbox.Switch.h"
#include "GenericToolbox.Vector.h"

#include <StateAlchemist/constants.h>
#include <StateAlchemist/meta_manager.h>
#include <Game.h>
#include <util.hpp>
#include <note_cell.hpp>
#include <AlchemistLogger.h>

TabGames::TabGames() {
  alchemyLogger.log("TabGames::TabGames();");
  std::vector<Game> gameList = gameBrowser.getGameList();

  if (gameList.empty()) {
    brls::NoteCell* message = new brls::NoteCell();
    message->setText("No game folders have been found.");
    message->setNote(
      "To add mods, put them on the SD card in this manner: SD:/mod-alchemy/<title-id-of-the-game>/<group>/<thing-being-replaced>/<mod-name>/<mod-files-and-folders>."
    );
    Util::padTabContent(message);
    this->addView(message);
  } else {
     brls::Box* container = new brls::Box(brls::Axis::COLUMN);
     Util::padTabContent(container);
     this->addView(container);

    _gameItems_.reserve(gameList.size());

    for(auto& gameEntry : gameList) {
      std::string gamePath { GenericToolbox::joinPath(ALCHEMIST_FOLDER, MetaManager::getHexTitleId(gameEntry.titleId)) };

      brls::IconCell* item = new brls::IconCell();

      item->setText(gameEntry.name);
      if (gameEntry.icon.size() > 0) {
        item->setIconFromMem(gameEntry.icon.data(), 0x20000);
      }

      item->registerClickAction([&, gameEntry](View* view) {
        alchemyLogger.log("TabGames::select game...");
        gameBrowser.selectGame(gameEntry.titleId);
        FrameModBrowser* modsBrowser = new FrameModBrowser();
        alchemyLogger.log("TabGames::creating activity...");
        brls::Activity* modsActivity = new brls::Activity(modsBrowser);
        alchemyLogger.log("TabGames::pushing activity...");
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
