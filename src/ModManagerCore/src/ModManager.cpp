//
// Created by Nadrino on 06/09/2019.
//

#include "ModManager.h"
#include <Toolbox.h>
#include <GameBrowser.h>

#include "GenericToolbox.Switch.h"
#include "GenericToolbox.Vector.h"

#include <switch.h>

#include <iostream>
#include <sstream>
#include <StateAlchemist/controller.h>


ModManager::ModManager(GameBrowser* owner_) : _owner_(owner_) {}

// setters
void ModManager::setGameId(const u64 &gameId) {
  controller.titleId = gameId;
}

// getters
const Selector &ModManager::getSelector() const {
  return _selector_;
}
const std::vector<ModEntry> &ModManager::getModList() const {
  return _modList_;
}

std::vector<ModEntry> &ModManager::getModList() {
  return _modList_;
}

const ConfigHolder& ModManager::getConfig() const{
  return _owner_->getConfigHandler().getConfig();
}
ConfigHolder& ModManager::getConfig(){
  return _owner_->getConfigHandler().getConfig();
}

void ModManager::updateModList() {
  // list groups
  std::vector<std::string> groups = controller.loadGroups(true);

  // list mods
  // for now, just combining group names with mod names - will be separated in a future commit
  std::vector<ModEntry> options;
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
        options.push_back(modEntry);
      }
    }
  }

  _modList_.clear();
  _modList_.reserve(options.size());
  for (auto& option : options) {
    _modList_.emplace_back(option);
  }

  // reset the selector
  _selector_ = Selector();
  for( auto& mod : _modList_ ){
    _selector_.getEntryList().emplace_back();
    _selector_.getEntryList().back().title = mod.getLabel();
  }

  _selector_.clearMenu();
}

void ModManager::resetAllMods() {
  controller.deactivateAll();
  this->updateModList();
}

// mod management
ResultModAction ModManager::applyMod(int modIndex_){
  if( modIndex_ < 0 or modIndex_ >= int( _modList_.size() ) ) return Fail;
  auto* modPtr = &_modList_[modIndex_];
  if( modPtr == nullptr ) return Fail;

  controller.group = modPtr->group;
  controller.source = modPtr->source;
  controller.activateMod(modPtr->mod);

  return Success;
}
ResultModAction ModManager::applyMod(const std::string& modName_) {
  return this->applyMod( this->getModIndex(modName_));
}
ResultModAction ModManager::applyModList(const std::vector<std::string> &modNamesList_){
  
  for (auto& modName : modNamesList_) {
    this->applyMod(modName);
  }

  return Success;
}

void ModManager::removeMod(int modIndex_) {
  if( modIndex_ < 0 or modIndex_ >= int( _modList_.size() ) ) return;
  auto* modPtr = &_modList_[modIndex_];

  controller.group = modPtr->group;

  // Case: do nothing if this mod is not active
  std::string activeMod = controller.getActiveMod(modPtr->source);
  if (activeMod != modPtr->mod) return;

  controller.source = modPtr->source;
  controller.deactivateMod();
}

void ModManager::removeMod(const std::string &modName_) {
  this->removeMod( this->getModIndex(modName_) );
}

// utils
int ModManager::getModIndex(const std::string& modName_){
  return GenericToolbox::findElementIndex(
    modName_,
    _modList_,
    [](const ModEntry& mod_){ return mod_.getLabel(); }
  );
}


void ModManager::reloadCustomPreset(){
  std::string configFilePath = controller.getGamePath() + "/this_folder_config.txt";
  if( GenericToolbox::isFile(configFilePath) ){
    _currentPresetName_ = GenericToolbox::dumpFileAsString(configFilePath);
  }
  else{
    _currentPresetName_ = "";
  }
}
void ModManager::setCustomPreset(const std::string &presetName_) {
  std::string configFilePath = controller.getGamePath() + "/this_folder_config.txt";
  GenericToolbox::rm( configFilePath );
  if( not presetName_.empty() ){
    GenericToolbox::dumpStringInFile( configFilePath, presetName_ );
  }
  this->reloadCustomPreset();
}

const PresetConfig &ModManager::fetchCurrentPreset() const {
  int idx = GenericToolbox::findElementIndex(_currentPresetName_, getConfig().presetList, [](const PresetConfig& p){ return p.name; });
  if( idx != -1 ){ return getConfig().presetList[idx]; }
  return getConfig().getCurrentPreset();
}

const std::string &ModManager::getCurrentPresetName() const {
  return _currentPresetName_;
}






