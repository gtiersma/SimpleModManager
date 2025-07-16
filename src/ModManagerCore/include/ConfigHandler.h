//
// Created by Nadrino on 16/10/2019.
//

#ifndef SIMPLEMODMANAGER_CONFIGHANDLER_H
#define SIMPLEMODMANAGER_CONFIGHANDLER_H


#include "GenericToolbox.String.h"
#include "GenericToolbox.Macro.h"

#include "StateAlchemist/constants.h"

#include <string>
#include <sstream>



struct ConfigHolder{
  std::string lastSmmVersion{};

  std::string configFilePath{ALCHEMIST_PATH +"/parameters.ini"};

  [[nodiscard]] std::string getSummary() const {
    std::stringstream ss;
    ss << GET_VAR_NAME_VALUE(lastSmmVersion) << std::endl;
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

private:
  ConfigHolder _config_{};

};


#endif //SIMPLEMODMANAGER_CONFIGHANDLER_H
