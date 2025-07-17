//
// Created by Adrien BLANCHET on 21/06/2020.
//

#include <TabGeneralSettings.h>
#include <ModMigrator.h>

#include "FrameRoot.h"


#include "Logger.h"
#include <dialog_util.hpp>

LoggerInit([]{
  Logger::setUserHeaderStr("[TabGeneralSettings]");
});

TabGeneralSettings::TabGeneralSettings(FrameRoot* owner_) : _owner_(owner_) {
  LogWarning << "Building general settings tab..." << std::endl;
  this->rebuildLayout();
  LogInfo << "General settings tab build." << std::endl;
}

void TabGeneralSettings::rebuildLayout() {

  auto* migrationItem = new brls::ListItem(
    "Bring over old SimpleModManager mods",
    "This will take any mods on the SD card that were set up for the original SimpleModManager to work with this manager."
  );

  migrationItem->getClickEvent()->subscribe([this](View* view) {
    buildMigrateConfirmDialog()->open();
  });

  this->addView(migrationItem);
}

/**
 * Builds a confirmation dialog that will allow the user to migrate mods from the old SMM
 */
brls::Dialog* TabGeneralSettings::buildMigrateConfirmDialog() {
  auto* label = new brls::Label(
    brls::LabelStyle::DIALOG,
    "Migrate the mods from SimpleModManager?\n\n"\
    "This action cannot easily be undone.\n"\
    "Turn all the mods off in SimpleModManager first to clear them out before running this.",
    true
  );
  auto* confirmDialog = new brls::Dialog(label);

  confirmDialog->addButton("Yes", [this, confirmDialog](brls::View* view) {
    auto* loadingDialog = DialogUtil::buildLoadingDialog("Moving mods from the old SimpleModManager to this app. Please wait");
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
