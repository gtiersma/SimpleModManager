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


ModManager::ModManager(GameBrowser* owner_) : _owner_(owner_) {}

const ConfigHolder& ModManager::getConfig() const{
  return _owner_->getConfigHandler().getConfig();
}
ConfigHolder& ModManager::getConfig(){
  return _owner_->getConfigHandler().getConfig();
}

/**
 * Loads data related to a moddable thing in the game (aka source)
 * 
 * @warning Expects controller.group to be set to the group the source belongs to
 */
ModSource ModManager::loadSource(const std::string& sourceName) {
  controller.source = sourceName;
  std::vector<std::string> mods = controller.loadMods(true);
  return ModSource(sourceName, mods, this->getActiveIndex(sourceName, mods));
}

/**
 * Gets the index of the currently-active mod listed in the source_'s "mods" vector
 * 
 * Returns -1 if no mod is active
 * 
 * @param mods Ordered vector of mod names that belong to the source.
 */
int ModManager::getActiveIndex(const std::string& source, const std::vector<std::string>& mods) {
  unsigned activeIndex = -1;
  std::string activeMod = controller.getActiveMod(source);

  if (activeMod.empty()) { return activeIndex; }

  for (auto& mod : mods) {
    activeIndex++;
    if (mod == activeMod) { break; }
  }

  return activeIndex;
}






