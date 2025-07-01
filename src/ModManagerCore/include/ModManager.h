//
// Created by Nadrino on 06/09/2019.
//

#ifndef MODAPPLIER_MOD_MANAGER_H
#define MODAPPLIER_MOD_MANAGER_H

#include <ConfigHandler.h>
#include "Selector.h"

#include "GenericToolbox.Map.h"

#include <map>
#include <vector>
#include <string>
#include <utility>

struct ApplyCache{
  std::string statusStr{"UNCHECKED"};
  double applyFraction{0};
};

/**
 * Object containing data related to a "source" (something moddable in a game)
 */
struct ModSource{
  ModSource() = default;

  /**
   * @param source_ String label of the name of the moddable thing
   * @param mods_ List of available mods for the moddable thing
   */
  explicit ModSource(std::string source_, std::vector<std::string> mods_): source(std::move(source_)), mods(std::move(mods_)) {}

  std::string source;
  std::vector<std::string> mods;
};


class GameBrowser;

/**
 * Class for interacting with mods
 * 
 * Originally part of vanilla SimpleModManager. Now uses StateAlchemist as a backend.
 * 
 * Set the group in controller.group; all interactions in this class are scoped to that group.
 */
class ModManager {

public:
  explicit ModManager(GameBrowser* owner_);

  /**
   * Gets a list of all ModSources that belong to the current group
   */
  const std::vector<ModSource> &getGroupedModList() const;

  // shortcuts
  const ConfigHolder& getConfig() const;
  ConfigHolder& getConfig();

  // selector related
  void updateModList();

  // mod management
  int getActiveIndex(const ModSource& source_);

  void applyMod(const std::string& source_, const std::string& modName_);

  void removeMod(const std::string& source_);

  // preset
  void reloadCustomPreset();
  void setCustomPreset(const std::string &presetName_);
  const PresetConfig& fetchCurrentPreset() const;

  const std::string &getCurrentPresetName() const;

private:
  GameBrowser* _owner_{nullptr};

  std::string _gameName_{};
  std::vector<std::string> _ignoredFileList_{};

  /**
   * List of mods only belonging to group that is current when constructed.
   * Can always be reconstructed by calling updateModList if the group changes.
   */
  std::vector<ModSource> _groupedModList_{};

  std::string _currentPresetName_{};
};


#endif //MODAPPLIER_MOD_MANAGER_H
