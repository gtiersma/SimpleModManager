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
#include <AlchemistLogger.h>


ModManager::ModManager(GameBrowser* owner_) : _owner_(owner_) {}

/**
 * Gets a list of all ModSources that belong to the current group
 */
const std::vector<ModSource> &ModManager::getGroupedModList() const {
  return _groupedModList_;
}

const ConfigHolder& ModManager::getConfig() const{
  return _owner_->getConfigHandler().getConfig();
}
ConfigHolder& ModManager::getConfig(){
  return _owner_->getConfigHandler().getConfig();
}

/**
 * Loads/reloads all sources/mods within the current group
 */
void ModManager::updateModList() {
  alchemyLogger.log("MOD MANAGER: loading groups... ");
  _groupedModList_.clear();

  std::vector<std::string> sources = controller.loadSources(true);
  _groupedModList_.reserve(sources.size());
  for (auto& source : sources) {
    controller.source = source;
    alchemyLogger.log("MOD MANAGER: loading mods from source: " + source);
    _groupedModList_.emplace_back(
      ModSource(source, controller.loadMods(true))
    );
  }
}

/**
 * Gets the index of the currently-active mod listed in the source_'s "mods" vector
 * 
 * Returns -1 if no mod is active
 */
unsigned ModManager::getActiveIndex(const ModSource& source_) {
  alchemyLogger.log("MOD MANAGER: getting active index of source: " + source_.source);
  unsigned activeIndex = -1;
  std::string activeMod = controller.getActiveMod(source_.source);

  for (auto& mod : source_.mods) {
    if (mod == activeMod) { break; }
    activeIndex++;
  }
  alchemyLogger.log("MOD MANAGER: active index: " + std::to_string(activeIndex));

  return activeIndex;
}

void ModManager::applyMod(const std::string& source_, const std::string& modName_) {
  controller.source = source_;
  controller.activateMod(modName_);
}

void ModManager::removeMod(const std::string &source_) {
  alchemyLogger.log("ModManager::removeMod();");
  controller.source = source_;
  controller.deactivateMod();
}


void ModManager::reloadCustomPreset(){
  alchemyLogger.log("ModManager::reloadCustomPreset();");
  std::string configFilePath = controller.getGamePath() + "/this_folder_config.txt";
  if( GenericToolbox::isFile(configFilePath) ){
    _currentPresetName_ = GenericToolbox::dumpFileAsString(configFilePath);
  }
  else{
    _currentPresetName_ = "";
  }
}
void ModManager::setCustomPreset(const std::string &presetName_) {
  alchemyLogger.log("ModManager::setCustomPreset();");
  std::string configFilePath = controller.getGamePath() + "/this_folder_config.txt";
  GenericToolbox::rm( configFilePath );
  if( not presetName_.empty() ){
    GenericToolbox::dumpStringInFile( configFilePath, presetName_ );
  }
  this->reloadCustomPreset();
}

const PresetConfig &ModManager::fetchCurrentPreset() const {
  alchemyLogger.log("ModManager::fetchCurrentPreset();");
  int idx = GenericToolbox::findElementIndex(_currentPresetName_, getConfig().presetList, [](const PresetConfig& p){ return p.name; });
  if( idx != -1 ){ return getConfig().presetList[idx]; }
  return getConfig().getCurrentPreset();
}

const std::string &ModManager::getCurrentPresetName() const {
  return _currentPresetName_;
}






