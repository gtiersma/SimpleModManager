//
// Created by Adrien BLANCHET on 21/06/2020.
//

#include <TabGeneralSettings.h>

#include "FrameRoot.h"


#include "Logger.h"

LoggerInit([]{
  Logger::setUserHeaderStr("[TabGeneralSettings]");
});

TabGeneralSettings::TabGeneralSettings(FrameRoot* owner_) : _owner_(owner_) {
  LogWarning << "Building general settings tab..." << std::endl;
  this->rebuildLayout();
  LogInfo << "General settings tab build." << std::endl;
}

void TabGeneralSettings::rebuildLayout() {

  // TODO: support this
  /*itemInstallLocationPreset = new brls::ListItem(
    "\uE255 Install location:",
    "Specify from which base folder mods will be installed.\n"\
    "- If you are using Atmosphere, mods have to be installed in /atmosphere/. "\
    "This corresponds to the \"default\" preset. You need to take this path into account in your mod tree structure.\n"\
    "- If you want to set a specific install folder for a given game, please refer to its Option tab and go to \"Attribute a config preset\".",
    ""
  );
  itemInstallLocationPreset->setValue(this->getConfig().getCurrentPresetName() );
  itemInstallLocationPreset->getClickEvent()->subscribe([this](View* view) {
    brls::ValueSelectedEvent::Callback valueCallback = [this](int result) {
      if (result == -1) {
        return;
      }

      this->getConfig().setSelectedPresetIndex( result );
      _owner_->getGuiModManager().getGameBrowser().getConfigHandler().dumpConfigToFile();
      LogInfo << "Selected: " << this->getConfig().getCurrentPreset().name << " -> " << this->getConfig().getCurrentPreset().installBaseFolder << std::endl;
      this->itemInstallLocationPreset->setValue(this->getConfig().getCurrentPresetName() );
    };

    std::vector<std::string> presetNameList;
    presetNameList.reserve( this->getConfig().presetList.size() );
    for( auto& preset : this->getConfig().presetList ){
      presetNameList.emplace_back( preset.name + " \uE090 \"" + preset.installBaseFolder + "\"" );
    }

    brls::Dropdown::open(
      "Current install preset:",
      presetNameList,
      valueCallback,
      this->getConfig().selectedPresetIndex
    );
  });
  this->addView(itemInstallLocationPreset);*/

}

const ConfigHolder& TabGeneralSettings::getConfig() const{
  return _owner_->getGuiModManager().getGameBrowser().getConfigHandler().getConfig();
}
ConfigHolder& TabGeneralSettings::getConfig(){
  return _owner_->getGuiModManager().getGameBrowser().getConfigHandler().getConfig();
}
