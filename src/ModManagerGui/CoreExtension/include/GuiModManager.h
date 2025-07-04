//
// Created by Adrien BLANCHET on 28/06/2020.
//

#ifndef SIMPLEMODMANAGER_GUIMODMANAGER_H
#define SIMPLEMODMANAGER_GUIMODMANAGER_H


#include "GameBrowser.h"
#include "GenericToolbox.Borealis.h"

#include <borealis.hpp>

#include <string>
#include <vector>
#include <future>


class GuiModManager {

public:
  GuiModManager() = default;

  // setters
  void setTriggerUpdateModsDisplayedStatus(bool triggerUpdateModsDisplayedStatus);

  // getters
  [[nodiscard]] bool isTriggerUpdateModsDisplayedStatus() const;
  const GameBrowser &getGameBrowser() const;
  GameBrowser &getGameBrowser();

  void startApplyModThread(const std::string& modName_);
  void startRemoveModThread(const std::string& modName_);
  void startRemoveAllModsThread();
  void startApplyModPresetThread(const std::string &modPresetName_);

  void applyMod(const std::string &modName_);
  void applyModsList(std::vector<std::string>& modsList_);
  void removeMod(const std::string &modName_);
  void removeAllMods();
  bool isActive(const std::string &modName_);

protected:
  void applyModFunction(const std::string& modName_);
  void applyModPresetFunction(const std::string& presetName_);
  void removeModFunction(const std::string& modName_);
  void removeAllModsFunction();

  void leaveModAction();



private:
  // core
  GameBrowser _gameBrowser_{};

  bool _triggeredOnCancel_{false};
  bool _triggerUpdateModsDisplayedStatus_{false};


  // monitors
  GenericToolbox::Borealis::PopupLoadingBox _loadingPopup_;
  struct ModApplyMonitor{
    double progress{0};
    std::string currentFile{};
  }; ModApplyMonitor modApplyMonitor{};

  struct ModApplyListMonitor{
    double progress{0};
    std::string currentMod{};
  }; ModApplyListMonitor modApplyListMonitor{};
  struct ModCheckMonitor{
    double progress{0};
    std::string currentFile{};
  }; ModCheckMonitor modCheckMonitor{};

  struct ModCheckAllMonitor{
    double progress{0};
    std::string currentMod{};
  }; ModCheckAllMonitor modCheckAllMonitor{};

  struct ModRemoveMonitor{
    double progress{0};
    std::string currentFile{};
  }; ModRemoveMonitor modRemoveMonitor{};

  struct ModRemoveAllMonitor{
    double progress{0};
    std::string currentMod{};
  }; ModRemoveAllMonitor modRemoveAllMonitor{};


};


#endif //SIMPLEMODMANAGER_GUIMODMANAGER_H
