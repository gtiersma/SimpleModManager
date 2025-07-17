//
// Created by Nadrino on 06/09/2019.
//

#include "ModManager.h"
#include <Toolbox.h>
#include <GameBrowser.h>

#include "GenericToolbox.Switch.h"
#include "GenericToolbox.Vector.h"

#include <switch.h>

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
int ModManager::getActiveIndex(const ModSource& source_) {
  alchemyLogger.log("MOD MANAGER: getting active index of source: " + source_.source);
  unsigned activeIndex = -1;
  std::string activeMod = controller.getActiveMod(source_.source);

  if (activeMod.empty()) { return activeIndex; }

  for (auto& mod : source_.mods) {
    activeIndex++;
    if (mod == activeMod) { break; }
  }
  alchemyLogger.log("MOD MANAGER: active index: " + std::to_string(activeIndex));

  return activeIndex;
}






