//
// Created by Adrien BLANCHET on 21/06/2020.
//

#include <TabGeneralSettings.h>
#include <ModMigrator.h>

#include "FrameRoot.h"

#include <dialog_util.hpp>

TabGeneralSettings::TabGeneralSettings() {
  this->rebuildLayout();
}

brls::View* TabGeneralSettings::create() { return new TabGeneralSettings(); }

void TabGeneralSettings::rebuildLayout() {
  brls::DetailCell* migrationItem = new brls::DetailCell();
  migrationItem->setText("Bring over old SimpleModManager mods");
  migrationItem->setDetailText(
    "This will take any mods on the SD card that were set up for the original SimpleModManager to work with this manager."
  );
  migrationItem->setFocusable(true);
  migrationItem->registerClickAction([this](brls::View* view) {
    buildMigrateConfirmDialog()->open();
    return true;
  });
  this->addView(migrationItem);
}

/**
 * Builds a confirmation dialog that will allow the user to migrate mods from the old SMM
 */
brls::Dialog* TabGeneralSettings::buildMigrateConfirmDialog() {
  brls::Dialog* confirmDialog = new brls::Dialog(
    "Migrate the mods from SimpleModManager?\n\n"\
    "This action cannot easily be undone.\n"\
    "Turn all the mods off in SimpleModManager first to clear them out before running this."
  );

  confirmDialog->addButton("Yes", [this, confirmDialog]() {
    brls::Dialog* loadingDialog = DialogUtil::buildLoadingDialog(
      "Moving mods from the old SimpleModManager to this app. Please wait"
    );
    loadingDialog->open();

    new std::thread([this, loadingDialog]() {
      ModMigrator().begin();
      loadingDialog->close();

      brls::Dialog* completeDialog = buildMigrateFinishedDialog();
      completeDialog->open();
    });
  });

  confirmDialog->addButton("No", []() {});

  return confirmDialog;
}

 /**
  * Builds a dialog to show for after the migration finishes
  */
brls::Dialog* TabGeneralSettings::buildMigrateFinishedDialog() {
  return new brls::Dialog(
    "Finished moving mod files & folders.\n\n"\
    "The mods have been grouped in an \"uncategorized\" folder. "\
    "It's recommended to reorganize them into group folders on your pc to make them easy to use, but you don't have to. "\
    "Any files that couldn't be moved have been left where they were."
  );
}
