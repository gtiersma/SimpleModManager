//
// Created by Adrien BLANCHET on 21/06/2020.
//

#include <TabGeneralSettings.h>
#include <ModMigrator.h>

#include "FrameRoot.h"
#include <AlchemistLogger.h>

#include "Logger.h"

LoggerInit([]{
  Logger::setUserHeaderStr("[TabGeneralSettings]");
});

TabGeneralSettings::TabGeneralSettings(FrameRoot* owner_) : _owner_(owner_) {
  alchemyLogger.log("TabGeneralSettings::TabGeneralSettings();");
  LogWarning << "Building general settings tab..." << std::endl;
  this->rebuildLayout();
  LogInfo << "General settings tab build." << std::endl;
}

void TabGeneralSettings::rebuildLayout() {
  alchemyLogger.log("TabGeneralSettings::rebuildLayout();");

  auto* migrationItem = new brls::ListItem(
    "Bring over old SimpleModManager mods",
    "This will take any mods on the SD card that were set up for the original SimpleModManager to work with this manager."
  );

  migrationItem->getClickEvent()->subscribe([this](View* view) {
    buildMigrateConfirmDialog()->open();
  });

  this->addView(migrationItem);

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

/**
 * Builds a confirmation dialog that will allow the user to migrate mods from the old SMM
 */
brls::Dialog* TabGeneralSettings::buildMigrateConfirmDialog() {
  auto* label = new brls::Label(
    brls::LabelStyle::DIALOG,
    "Migrate the mods from SimpleModManager?\n\nThis action cannot easily be undone.",
    true
  );
  auto* confirmDialog = new brls::Dialog(label);

  confirmDialog->addButton("Yes", [this, confirmDialog](brls::View* view) {
    auto* loadingDialog = buildMigrateLoadingDialog();
    loadingDialog->open();

    // Must run remainder code in new thread so the loading dialog visually appears as the work is being done.
    new std::thread([this, confirmDialog, loadingDialog]() {
      ModMigrator().begin();
  
      auto* completeDialog = buildMigrateFinishedDialog();

      // Add an OK button to the complete dialog to close all the popups
      completeDialog->addButton("OK", [confirmDialog, loadingDialog, completeDialog](brls::View* view) {
        completeDialog->close([confirmDialog, loadingDialog]() {
          loadingDialog->close([confirmDialog]() {
            confirmDialog->close();
          });
        });
      });
  
      completeDialog->open();

      // Buttons were not doing anything at this point, so we need to unblock them
      brls::Application::unblockInputs();
    });
  });

  confirmDialog->addButton("No", [confirmDialog](brls::View* view) {
    confirmDialog->close();
  });

  return confirmDialog;
}

/**
 * Builds a dialog to show for while the migration is occurring
 */
brls::Dialog* TabGeneralSettings::buildMigrateLoadingDialog() {
  auto* layout = new brls::BoxLayout(brls::BoxLayoutOrientation::HORIZONTAL);
  auto* label = new brls::Label(brls::LabelStyle::DIALOG, "Migrating mods...");
  auto* progress = new brls::ProgressSpinner();
  progress->willAppear(); // TODO: no progress spinner :(
  layout->addView(label);
  layout->addView(progress);

  auto* dialog = new brls::Dialog(layout);
  dialog->setCancelable(false);

  return dialog;
}

 /**
  * Builds a dialog to show for after the migration finishes
  */
brls::Dialog* TabGeneralSettings::buildMigrateFinishedDialog() {
  auto* label = new brls::Label(
    brls::LabelStyle::DIALOG,
    "Finished moving mod files & folders.\n\n"\
    "The mods have been grouped in an \"uncategorized\" folder. "\
    "It's recommended to reorganize them into group folders on your pc to make them easy to use, but you don't have to. "\
    "Any files that couldn't be moved have been left where they were.",
    true
  );

  auto* dialog = new brls::Dialog(label);
  dialog->setCancelable(false);

  return dialog;
}

const ConfigHolder& TabGeneralSettings::getConfig() const{
  return _owner_->getGuiModManager().getGameBrowser().getConfigHandler().getConfig();
}
ConfigHolder& TabGeneralSettings::getConfig(){
  return _owner_->getGuiModManager().getGameBrowser().getConfigHandler().getConfig();
}
