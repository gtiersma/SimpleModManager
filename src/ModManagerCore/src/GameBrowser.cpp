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
const Selector &GameBrowser::getSelector() const{
  return _selector_;
}
Selector &GameBrowser::getSelector(){
  return _selector_;
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

// Browse
void GameBrowser::selectGame(const u64 &titleId_) {
  controller.titleId = titleId_;
  _isGameSelected_ = true;
}

void GameBrowser::rebuildSelectorMenu(){
  _selector_.clearMenu();

  _selector_.getHeader() >> "SimpleModManager v" >> Toolbox::getAppVersion() << std::endl;
  _selector_.getHeader() << GenericToolbox::ColorCodes::redBackground << "Current Folder : ";
  _selector_.getHeader() << GenericToolbox::repeatString("*", GenericToolbox::getTerminalWidth()) << std::endl;

  _selector_.getFooter() << GenericToolbox::repeatString("*", GenericToolbox::getTerminalWidth()) << std::endl;
  _selector_.getFooter() << "  Page (" << _selector_.getCursorPage() + 1 << "/" << _selector_.getNbPages() << ")" << std::endl;
  _selector_.getFooter() << GenericToolbox::repeatString("*", GenericToolbox::getTerminalWidth()) << std::endl;
  _selector_.getFooter() << "Configuration preset : " << GenericToolbox::ColorCodes::greenBackground;
  _selector_.getFooter() << _configHandler_.getConfig().getCurrentPresetName() << GenericToolbox::ColorCodes::resetColor << std::endl;
  _selector_.getFooter() << "install-mods-base-folder = " + _configHandler_.getConfig().getCurrentPreset().installBaseFolder << std::endl;
  _selector_.getFooter() << GenericToolbox::repeatString("*", GenericToolbox::getTerminalWidth()) << std::endl;
  _selector_.getFooter() << " A : Select folder" >> "Y : Change config preset " << std::endl;
  _selector_.getFooter() << " B : Quit" << std::endl;
  _selector_.getFooter() << std::endl;
  _selector_.getFooter() << std::endl;
  _selector_.getFooter() << std::endl;

  _selector_.invalidatePageCache();
  _selector_.refillPageEntryCache();
}

uint8_t* GameBrowser::getFolderIcon(const std::string& gameFolder_){
  if( _isGameSelected_ ){ return nullptr; }
  uint8_t* icon = GenericToolbox::Switch::Utils::getIconFromTitleId(gameFolder_);
  return icon;
}

// protected
void GameBrowser::init(){
  auto gameList = GenericToolbox::lsDirs(ALCHEMIST_PATH);

  std::vector<size_t> nGameMod;
  nGameMod.reserve( gameList.size() );
  for( auto& game : gameList ){
    nGameMod.emplace_back(
        GenericToolbox::lsDirs(controller.getGamePath()).size()
        );
  }

  auto ordering = GenericToolbox::getSortPermutation(nGameMod, [](size_t a_, size_t b_){ return a_ > b_; });
  GenericToolbox::applyPermutation(gameList, ordering);
  GenericToolbox::applyPermutation(nGameMod, ordering);

  _selector_.getEntryList().reserve( gameList.size() );
  for( size_t iGame = 0 ; iGame < gameList.size() ; iGame++ ){
    _selector_.getEntryList().emplace_back();
    _selector_.getEntryList().back().title = gameList[iGame];
    _selector_.getEntryList().back().tag = "(" + std::to_string(nGameMod[iGame]) + " mods)";
  }
}

