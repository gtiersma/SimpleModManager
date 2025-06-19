//
// Created by Nadrino on 16/10/2019.
//

#ifndef SIMPLEMODMANAGER_CONFIGHANDLER_H
#define SIMPLEMODMANAGER_CONFIGHANDLER_H


#include "GenericToolbox.String.h"
#include "GenericToolbox.Macro.h"

#include <map>
#include <string>
#include <vector>
#include <sstream>



struct PresetConfig{
  std::string name{};
  std::string installBaseFolder{};
};

struct ConfigHolder{

#define ENUM_NAME SortGameList
#define ENUM_FIELDS \
  ENUM_FIELD( NbMods, 0 ) \
  ENUM_FIELD( Alphabetical ) \
  ENUM_FIELD( NoSort )
#include "GenericToolbox.MakeEnum.h"

  bool useGui{true};
  SortGameList sortGameList{SortGameList::NbMods};
  int selectedPresetIndex{0};
  std::vector<PresetConfig> presetList{
      {{"default"}, {"/atmosphere"}},
      {{"reinx"}, {"/reinx"}},
      {{"sxos"}, {"/sxos"}},
      {{"root"}, {"/"}},
  };
  std::string lastSmmVersion{};

  std::string configFilePath{"/config/SimpleModManager/parameters.ini"};

  void setSelectedPresetIndex(int selectedPresetIndex_);
  void setSelectedPreset(const std::string& preset_);
  [[nodiscard]] std::string getCurrentPresetName() const;
  [[nodiscard]] const PresetConfig& getCurrentPreset() const { return presetList[selectedPresetIndex]; }

  [[nodiscard]] std::string getSummary() const {
    std::stringstream ss;
    ss << GET_VAR_NAME_VALUE(useGui) << std::endl;
    ss << GET_VAR_NAME_VALUE(sortGameList.toString()) << std::endl;
    ss << GET_VAR_NAME_VALUE(selectedPresetIndex) << std::endl;
    ss << GET_VAR_NAME_VALUE(lastSmmVersion) << std::endl;
    ss << GET_VAR_NAME_VALUE(configFilePath) << std::endl;
    ss << GenericToolbox::toString(presetList, [](const PresetConfig& p){ return p.name + " -> " + p.installBaseFolder; });
    return ss.str();
  }
};

class ConfigHandler {

public:
  ConfigHandler(){ this->loadConfig(); }

  // getters
  [[nodiscard]] const ConfigHolder &getConfig() const{ return _config_; }
  ConfigHolder &getConfig(){ return _config_; }

  // io
  void loadConfig(const std::string& configFilePath_ = "");
  void dumpConfigToFile() const;

  // change preset
  void setCurrentConfigPresetId(int selectedPresetId_);
  void selectPresetWithName(const std::string &presetName_);
  void selectNextPreset();
  void selectPreviousPreset();

private:
  ConfigHolder _config_{};

};


#endif //SIMPLEMODMANAGER_CONFIGHANDLER_H
