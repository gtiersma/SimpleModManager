//
// Created by Adrien BLANCHET on 28/06/2020.
//

#include "GuiModManager.h"

#include "GenericToolbox.Switch.h"
#include "GenericToolbox.Vector.h"
#include "GenericToolbox.Misc.h"
#include "Logger.h"

#include <string>
#include <vector>
#include <future>
#include <sstream>
#include <StateAlchemist/controller.h>


LoggerInit([]{
  Logger::setUserHeaderStr("[GuiModManager]");
});


void GuiModManager::setTriggerUpdateModsDisplayedStatus(bool triggerUpdateModsDisplayedStatus) {
  _triggerUpdateModsDisplayedStatus_ = triggerUpdateModsDisplayedStatus;
}

bool GuiModManager::isTriggerUpdateModsDisplayedStatus() const {
  return _triggerUpdateModsDisplayedStatus_;
}
const GameBrowser &GuiModManager::getGameBrowser() const { return _gameBrowser_; }
GameBrowser &GuiModManager::getGameBrowser(){ return _gameBrowser_; }

void GuiModManager::applyMod(const std::string &modName_) {
  LogWarning << __METHOD_NAME__ << ": " << modName_ << std::endl;
  modApplyMonitor = ModApplyMonitor();

  // Get the group, source and mod name from the string into a vector --- TEMP CODE
  std::istringstream iss(modName_);
  std::vector<std::string> modGrouping;
  std::string individualModGrouping;
  while (std::getline(iss, individualModGrouping, '/')) {
    modGrouping.push_back(individualModGrouping);
  }

  controller.group = modGrouping[0];
  controller.source = modGrouping[1];
  controller.activateMod(modGrouping[2]);
}

bool GuiModManager::isActive(const std::string &modName_) {
  LogWarning << __METHOD_NAME__ << ": " << modName_ << std::endl;
  
  // Get the group, source and mod name from the string into a vector --- TEMP CODE
  std::istringstream iss(modName_);
  std::vector<std::string> modGrouping;
  std::string individualModGrouping;
  while (std::getline(iss, individualModGrouping, '/')) {
    modGrouping.push_back(individualModGrouping);
  }

  controller.group = modGrouping[0];
  return controller.getActiveMod(modGrouping[1]) == modGrouping[2];
}

void GuiModManager::removeMod(const std::string &modName_){
  if (!this->isActive(modName_)) return;

  LogWarning << __METHOD_NAME__ << ": " << modName_ << std::endl;
  
  // Get the group, source and mod name from the string into a vector --- TEMP CODE
  std::istringstream iss(modName_);
  std::vector<std::string> modGrouping;
  std::string individualModGrouping;
  while (std::getline(iss, individualModGrouping, '/')) {
    modGrouping.push_back(individualModGrouping);
  }

  controller.source = modGrouping[1];
  controller.deactivateMod();
}

void GuiModManager::removeAllMods() {
  controller.deactivateAll();
}

void GuiModManager::applyModsList(std::vector<std::string>& modsList_){
  for (const auto& modName : modsList_) {
    this->applyMod(modName);
  }
}

std::vector<std::string> GuiModManager::getActiveMods() {
  std::vector<std::string> activeMods;
  
  std::vector<std::string> groups = controller.loadGroups(false);

  for (const std::string& group : groups) {
    controller.group = group;
    std::vector<std::string> sources = controller.loadSources(false);
    for (const std::string& source : sources) {
      ModEntry modEntry(controller.getActiveMod(source));
      modEntry.source = source;
      modEntry.group = group;
      activeMods.push_back(modEntry.getLabel());
    }
  }

  return activeMods;
}

void GuiModManager::startApplyModThread(const std::string& modName_) {
  LogReturnIf(modName_.empty(), "No mod name provided. Can't apply mod.");

  this->_triggeredOnCancel_ = false;

  // start the parallel thread
  std::async(&GuiModManager::applyModFunction, this, modName_);
}
void GuiModManager::startRemoveModThread(const std::string& modName_){
  LogReturnIf(modName_.empty(), "No mod name provided. Can't remove mod.");

  this->_triggeredOnCancel_ = false;

  // start the parallel thread
  std::async(&GuiModManager::removeModFunction, this, modName_);
}
void GuiModManager::startRemoveAllModsThread(){
  this->_triggeredOnCancel_ = false;

  // start the parallel thread
  std::async(&GuiModManager::removeAllModsFunction, this);
}
void GuiModManager::startApplyModPresetThread(const std::string &modPresetName_){
  this->_triggeredOnCancel_ = false;

  // start the parallel thread
  std::async(&GuiModManager::applyModPresetFunction, this, modPresetName_);
}

void GuiModManager::applyModFunction(const std::string& modName_){
  // push the progress bar to the view
  _loadingPopup_.pushView();

  LogWarning << "Applying: " << modName_ << std::endl;
  _loadingPopup_.getMonitorView()->setHeaderTitle("Applying mod...");
  _loadingPopup_.getMonitorView()->setProgressColor(GenericToolbox::Borealis::greenNvgColor);
  _loadingPopup_.getMonitorView()->resetMonitorAddresses();
  _loadingPopup_.getMonitorView()->setTitlePtr( &modName_ );
  _loadingPopup_.getMonitorView()->setSubTitlePtr( &modApplyMonitor.currentFile );
  this->applyMod( modName_ );

  this->leaveModAction();
}

void GuiModManager::applyModPresetFunction(const std::string& presetName_){
  // push the progress bar to the view
  _loadingPopup_.pushView();

  LogInfo << "Removing all installed mods..." << std::endl;
  _loadingPopup_.getMonitorView()->setProgressColor(GenericToolbox::Borealis::redNvgColor);
  _loadingPopup_.getMonitorView()->setHeaderTitle("Removing all installed mods...");
  _loadingPopup_.getMonitorView()->resetMonitorAddresses();
  _loadingPopup_.getMonitorView()->setTitlePtr( &modRemoveAllMonitor.currentMod );
  _loadingPopup_.getMonitorView()->setSubTitlePtr( &modRemoveMonitor.currentFile );
  this->removeAllMods();

  LogInfo("Applying Mod Preset");
  _loadingPopup_.getMonitorView()->setHeaderTitle("Applying mod preset...");
  _loadingPopup_.getMonitorView()->setProgressColor(GenericToolbox::Borealis::greenNvgColor);
  _loadingPopup_.getMonitorView()->resetMonitorAddresses();
  _loadingPopup_.getMonitorView()->setTitlePtr( &modApplyListMonitor.currentMod );
  _loadingPopup_.getMonitorView()->setSubTitlePtr( &modApplyMonitor.currentFile );

  std::vector<std::string> modsList;
  for( auto& preset : _gameBrowser_.getModPresetHandler().getPresetList() ){
    if( preset.name == presetName_ ){ modsList = preset.modList; break; }
  }
  this->applyModsList(modsList);

  this->leaveModAction();
}
void GuiModManager::removeModFunction(const std::string& modName_){
  // push the progress bar to the view
  _loadingPopup_.pushView();

  LogWarning << "Removing: " << modName_ << std::endl;
  _loadingPopup_.getMonitorView()->setHeaderTitle("Removing mod...");
  _loadingPopup_.getMonitorView()->setProgressColor(GenericToolbox::Borealis::redNvgColor);
  _loadingPopup_.getMonitorView()->resetMonitorAddresses();
  _loadingPopup_.getMonitorView()->setTitlePtr(&modName_);
  this->removeMod( modName_ );

  this->leaveModAction();
}
void GuiModManager::removeAllModsFunction(){
  // push the progress bar to the view
  _loadingPopup_.pushView();

  LogInfo("Removing all installed mods...");
  _loadingPopup_.getMonitorView()->setProgressColor(GenericToolbox::Borealis::redNvgColor);
  _loadingPopup_.getMonitorView()->setHeaderTitle("Removing all installed mods...");
  _loadingPopup_.getMonitorView()->resetMonitorAddresses();
  _loadingPopup_.getMonitorView()->setTitlePtr( &modRemoveAllMonitor.currentMod );
  _loadingPopup_.getMonitorView()->setSubTitlePtr( &modRemoveMonitor.currentFile );
  this->removeAllMods();

  this->leaveModAction();
}


void GuiModManager::leaveModAction(){
  _triggerUpdateModsDisplayedStatus_ = true;
  _loadingPopup_.popView();
  brls::Application::unblockInputs();
}

