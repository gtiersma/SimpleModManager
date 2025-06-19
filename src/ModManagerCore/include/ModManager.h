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

struct ModEntry{
  ModEntry() = default;
  explicit ModEntry(std::string mod_): mod(std::move(mod_)) {}

  std::string mod;
  std::string source;
  std::string group;

  const std::string getLabel() const {
    return group + "/" + source + "/" + mod;
  }
};

ENUM_EXPANDER(
    ResultModAction, 0,
    Success,
    Fail,
    Abort
);


class GameBrowser;

class ModManager {

public:
  explicit ModManager(GameBrowser* owner_);

  // setters
  void setGameId(const u64 &gameId);

  // getters
  const Selector &getSelector() const;
  const std::vector<ModEntry> &getModList() const;

  std::vector<ModEntry> &getModList();

  // shortcuts
  const ConfigHolder& getConfig() const;
  ConfigHolder& getConfig();

  // selector related
  void updateModList();
  void resetAllMods();

  // mod management
  ResultModAction applyMod(int modIndex_);
  ResultModAction applyMod(const std::string& modName_);
  ResultModAction applyModList(const std::vector<std::string> &modNamesList_);

  void removeMod(int modIndex_);
  void removeMod(const std::string &modName_);

  // utils
  int getModIndex(const std::string& modName_);

  // preset
  void reloadCustomPreset();
  void setCustomPreset(const std::string &presetName_);
  const PresetConfig& fetchCurrentPreset() const;

  const std::string &getCurrentPresetName() const;

private:
  GameBrowser* _owner_{nullptr};

  std::string _gameName_{};
  std::vector<std::string> _ignoredFileList_{};

  Selector _selector_;
  std::vector<ModEntry> _modList_{};

  std::string _currentPresetName_{};
};


#endif //MODAPPLIER_MOD_MANAGER_H
