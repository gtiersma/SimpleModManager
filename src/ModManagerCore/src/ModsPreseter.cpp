//
// Created by Nadrino on 13/02/2020.
//

#include <Toolbox.h>
#include <ModsPresetHandler.h>
#include <Selector.h>

#include "GenericToolbox.Vector.h"
#include "GenericToolbox.Switch.h"

#include <switch.h>

#include <iostream>
#include <iomanip>
#include <algorithm>
#include "sstream"
#include <StateAlchemist/controller.h>
#include <ModManager.h>


void ModsPresetHandler::setGameId(const u64 &gameId_) {
  controller.titleId = gameId_;
  this->readConfigFile();
}

const std::vector<PresetData> &ModsPresetHandler::getPresetList() const {
  return _presetList_;
}
std::vector<PresetData> &ModsPresetHandler::getPresetList() {
  return _presetList_;
}

Selector &ModsPresetHandler::getSelector() {
  return _selector_;
}

void ModsPresetHandler::selectModPreset() {


  auto drawSelectorPage = [&]{
    using namespace GenericToolbox::Switch::Terminal;

    consoleClear();
    printRight("SimpleModManager v" + Toolbox::getAppVersion());
    std::cout << GenericToolbox::ColorCodes::redBackground << std::setw(GenericToolbox::getTerminalWidth()) << std::left;
    std::cout << "Select mod preset" << GenericToolbox::ColorCodes::resetColor;
    std::cout << GenericToolbox::repeatString("*", GenericToolbox::getTerminalWidth());
    _selector_.printTerminal();
    std::cout << GenericToolbox::repeatString("*", GenericToolbox::getTerminalWidth());
    printLeft("  Page (" + std::to_string(_selector_.getCursorPage() + 1) + "/" + std::to_string(
        _selector_.getNbPages()) + ")");
    std::cout << GenericToolbox::repeatString("*", GenericToolbox::getTerminalWidth());
    printLeftRight(" A : Select mod preset", " X : Delete mod preset ");
    printLeftRight(" Y : Edit preset", "+ : Create a preset ");
    printLeft(" B : Go back");
    consoleUpdate(nullptr);
  };

  PadState pad;
  padInitializeAny(&pad);

  drawSelectorPage();
  while(appletMainLoop()){

    padUpdate(&pad);;
    u64 kDown = padGetButtonsDown(&pad);
    u64 kHeld = padGetButtons(&pad);
    _selector_.scanInputs(kDown, kHeld);
    if(kDown & HidNpadButton_B){
      break;
    }
    else if( kDown & HidNpadButton_A and not _presetList_.empty() ){
      return;
    }
    else if(kDown & HidNpadButton_X and not _presetList_.empty()){
      std::string answer = Selector::askQuestion(
          "Are you sure you want to remove this preset ?",
          std::vector<std::string>({"Yes", "No"})
      );
      if( answer == "Yes" ) this->deleteSelectedPreset();
    }
    else if(kDown & HidNpadButton_Plus){ createNewPreset(); }
    else if(kDown & HidNpadButton_Y){ this->editPreset( _selector_.getCursorPage() ); }

    if( kDown != 0 or kHeld != 0 ){ drawSelectorPage(); }

  }
}
void ModsPresetHandler::createNewPreset(){
  // generate a new default name
  int iPreset{1};
  std::stringstream ss;
  auto presetNameList = this->generatePresetNameList();
  do{ ss.str(""); ss << "preset-" << iPreset++; }
  while( GenericToolbox::doesElementIsInVector( ss.str(), presetNameList ) );

  // create a new entry
  size_t presetIndex{_presetList_.size()};
  _presetList_.emplace_back();
  _presetList_.back().name = ss.str();

  // start the editor
  this->editPreset( presetIndex );
}
void ModsPresetHandler::deleteSelectedPreset(){
  this->deletePreset( _selector_.getCursorPosition() );
}
void ModsPresetHandler::editPreset( size_t entryIndex_ ) {

  auto& preset = _presetList_[entryIndex_];

  // list mods
  // for now, just combining group names with mod names - will be separated in a future commit
  std::vector<std::string> modsList;
  std::vector<std::string> groups = controller.loadGroups(true);
  for (auto& group : groups) {
    controller.group = group;
    std::vector<std::string> sources = controller.loadSources(true);
    for (auto& source : sources) {
      controller.source = source;
      std::vector<std::string> mods = controller.loadMods(true);
      for (auto& mod : mods) {
        ModEntry modEntry(mod);
        modEntry.source = source;
        modEntry.group = group;
        modsList.push_back(modEntry.getLabel());
      }
    }
  }

  Selector sel;
  sel.setEntryList(modsList);

  auto reprocessSelectorTags = [&]{
    // clear tags
    sel.clearTags();

    for(size_t presetModIndex = 0 ; presetModIndex < preset.modList.size() ; presetModIndex++ ){
      for( size_t jEntry = 0 ; jEntry < modsList.size() ; jEntry++ ){

        if(preset.modList[presetModIndex] == modsList[jEntry] ){
          // add selector tag to the given mod
          std::stringstream ss;
          ss << sel.getEntryList()[jEntry].tag;
          if( not ss.str().empty() ) ss << " & ";
          ss << "#" << presetModIndex;
          sel.setTag( jEntry, ss.str() );
          break;
        }

      }
    }
  };


  auto printSelector = [&]{
    using namespace GenericToolbox::Switch::Terminal;

    consoleClear();
    printRight("SimpleModManager v" + Toolbox::getAppVersion());
    std::cout << GenericToolbox::ColorCodes::redBackground << std::setw(GenericToolbox::getTerminalWidth()) << std::left;
    std::string header_title = "Creating preset : " + preset.name + ". Select the mods you want.";
    std::cout << header_title << GenericToolbox::ColorCodes::resetColor;
    std::cout << GenericToolbox::repeatString("*", GenericToolbox::getTerminalWidth());
    sel.printTerminal();
    std::cout << GenericToolbox::repeatString("*", GenericToolbox::getTerminalWidth());
    printLeftRight(" A : Add mod", "X : Cancel mod ");
    printLeftRight(" + : SAVE", "B : Abort / Go back ");
    consoleUpdate(nullptr);
  };

  PadState pad;
  padInitializeAny(&pad);

  printSelector();
  while(appletMainLoop()){

    padUpdate(&pad);
    u64 kDown = padGetButtonsDown(&pad);
    u64 kHeld = padGetButtons(&pad);
    sel.scanInputs(kDown, kHeld);
    if(kDown & HidNpadButton_A){
      preset.modList.emplace_back(sel.getSelectedEntryTitle() );
      reprocessSelectorTags();
    }
    else if(kDown & HidNpadButton_X){
      preset.modList.pop_back();
      reprocessSelectorTags();
    }
    else if( kDown & HidNpadButton_Plus ){
      // save changes
      break;
    }
    else if( kDown & HidNpadButton_B ){
      // discard changes
      return;
    }

    if( kDown != 0 or kHeld != 0 ){ printSelector(); }
  }


  preset.name = GenericToolbox::Switch::UI::openKeyboardUi( preset.name );

  // Check for conflicts
  this->showConflictingFiles( entryIndex_ );
  this->writeConfigFile();
  this->readConfigFile();
}
void ModsPresetHandler::deletePreset( size_t entryIndex ){
  _presetList_.erase( _presetList_.begin() + long( entryIndex ) );
  this->writeConfigFile();
  this->readConfigFile();
}

void ModsPresetHandler::deletePreset( const std::string& presetName_ ){
  int index = GenericToolbox::findElementIndex( presetName_, _selector_.getEntryList(), []( const SelectorEntry& e ){ return e.title; } );
  if( index == -1 ){ return ; }
  this->deletePreset( index );
}

std::vector<std::string> ModsPresetHandler::generatePresetNameList() const{
  std::vector<std::string> out;
  out.reserve(_presetList_.size());
  for( auto& preset : _presetList_ ){
    out.emplace_back( preset.name );
  }
  return out;
}

std::string ModsPresetHandler::getSelectedModPresetName() const {
  if( _presetList_.empty() ) return {};
  return _presetList_[_selector_.getCursorPosition()].name;
}
const std::vector<std::string>& ModsPresetHandler::getSelectedPresetModList() const{
  return _presetList_[_selector_.getCursorPosition()].modList;
}

void ModsPresetHandler::readConfigFile() {
  _presetList_.clear();

  // check if file exist
  auto lines = GenericToolbox::dumpFileAsVectorString(controller.getGamePath() + "/mod_presets.conf", true);

  for( auto &line : lines ){
    if(line[0] == '#') continue;

    auto lineElements = GenericToolbox::splitString(line, "=", true);
    if( lineElements.size() != 2 ) continue;

    // clean up for extra spaces characters
    for(auto &element : lineElements){ GenericToolbox::trimInputString(element, " "); }

    if( lineElements[0] == "preset" ){
      _presetList_.emplace_back();
      _presetList_.back().name = lineElements[1];
    }
    else {
      // cleaning possible garbage
      if( _presetList_.empty() ) continue;
      // element 0 is "mod7" for example. Irrelevant here
      _presetList_.back().modList.emplace_back( lineElements[1] );
    }
  }

  this->fillSelector();
}
void ModsPresetHandler::writeConfigFile() {

  std::stringstream ss;

  ss << "# This is a config file" << std::endl;
  ss << std::endl;
  ss << std::endl;

  for(auto const &preset : _presetList_ ){
    ss << "########################################" << std::endl;
    ss << "# mods preset name" << std::endl;
    ss << "preset = " << preset.name << std::endl;
    ss << std::endl;
    ss << "# mods list" << std::endl;
    int iMod{0};
    for( auto& mod : preset.modList ){
      ss << "mod" << iMod++ << " = " << mod << std::endl;
    }
    ss << "########################################" << std::endl;
    ss << std::endl;
  }

  std::string data = ss.str();
  GenericToolbox::dumpStringInFile(controller.getGamePath() + "/mod_presets.conf", data);

}
void ModsPresetHandler::fillSelector(){
  _selector_ = Selector();

  if( _presetList_.empty() ){
    _selector_.setEntryList({{"NO MODS PRESETS"}});
    return;
  }

  auto presetNameList = this->generatePresetNameList();
  std::vector<std::vector<std::string>> descriptionsList;
  descriptionsList.reserve( presetNameList.size() );
  for( auto& preset: _presetList_ ){
    descriptionsList.emplace_back();
    descriptionsList.back().reserve( preset.modList.size() );
    for( auto& mod : preset.modList ){ descriptionsList.back().emplace_back("  | " + mod ); }
  }

  _selector_.setEntryList(presetNameList);
  _selector_.setDescriptionList(descriptionsList);
}





