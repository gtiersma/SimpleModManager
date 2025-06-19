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

LoggerInit([]{
  Logger::setUserHeaderStr("[TabGames]");
});

TabGames::TabGames(FrameRoot* owner_) : _owner_(owner_) {
  LogWarning << "Building game tab..." << std::endl;

  auto gameList = this->getGameBrowser().getSelector().getEntryList();

  if( gameList.empty() ){
    LogInfo << "No game found." << std::endl;

    std::stringstream ssTitle;
    std::stringstream ssSubTitle;

    ssTitle << "No game folders have been found.";

    ssSubTitle << "- To add mods, you need to copy them such as: ";
    ssSubTitle << "SD:/mod-alchemy/<title-id-of-the-game>/<group>/<thing-being-replaced>/<name-of-the-mod>/<mods-files-and-folders>." << std::endl;

    _gameList_.emplace_back();
    _gameList_.back().item = new brls::ListItem( ssTitle.str(), ssSubTitle.str() );
    _gameList_.back().item->show([](){}, false);
  }
  else{
    LogInfo << "Adding " << gameList.size() << " game folders..." << std::endl;

    _gameList_.reserve( gameList.size() );
    for( auto& gameEntry : gameList ){
      LogScopeIndent;
      LogInfo << "Adding game folder: \"" << gameEntry.title << "\"" << std::endl;

      std::string gamePath{GenericToolbox::joinPath(ALCHEMIST_FOLDER, gameEntry.title)};

      // memory allocation
      // TODO: Game ID being used instead of name
      auto* item = new brls::ListItem(gameEntry.title, "", "");

      auto* icon = GenericToolbox::Switch::Utils::getIconFromTitleId(gameEntry.title);
      if(icon != nullptr){ item->setThumbnail(icon, 0x20000); }
      item->getClickEvent()->subscribe([&, gameEntry](View* view) {
        LogWarning << "Opening \"" << gameEntry.title << "\"" << std::endl;
        getGameBrowser().selectGame(std::stoi(gameEntry.title));
        auto* modsBrowser = new FrameModBrowser( &_owner_->getGuiModManager() );
        brls::Application::pushView(modsBrowser, brls::ViewAnimation::SLIDE_LEFT);
        modsBrowser->registerAction("", brls::Key::PLUS, []{return true;}, true);
        modsBrowser->updateActionHint(brls::Key::PLUS, ""); // make the change visible
      });
      item->updateActionHint(brls::Key::A, "Open");


      _gameList_.emplace_back();
      _gameList_.back().title = gameEntry.title;
      _gameList_.back().item = item;

    }
  }

  LogInfo << "Sorting games wrt nb of mods..." << std::endl;
  GenericToolbox::sortVector(_gameList_, [](const GameItem& a_, const GameItem& b_){
    return GenericToolbox::toLowerCase(a_.title) < GenericToolbox::toLowerCase(b_.title); // if true, then a_ goes first
  });
  LogDebug << "Sort done." << std::endl;

  // add to the view
  for( auto& game : _gameList_ ){ this->addView( game.item ); }

  LogInfo << "Game tab build." << std::endl;
}

const GameBrowser& TabGames::getGameBrowser() const{
  return _owner_->getGuiModManager().getGameBrowser();
}
const ConfigHolder& TabGames::getConfig() const{
  return getGameBrowser().getConfigHandler().getConfig();
}

GameBrowser& TabGames::getGameBrowser(){
  return _owner_->getGuiModManager().getGameBrowser();
}
ConfigHolder& TabGames::getConfig(){
  return getGameBrowser().getConfigHandler().getConfig();
}
