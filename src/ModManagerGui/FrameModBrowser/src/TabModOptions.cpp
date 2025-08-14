//
// Created by Adrien BLANCHET on 22/06/2020.
//

#include "TabModOptions.h"
#include "FrameModBrowser.h"

#include <StateAlchemist/controller.h>
#include <dialog_util.hpp>
#include <AlchemistLogger.h>



TabModOptions::TabModOptions() {}

void TabModOptions::buildDisableAllMods() {
  alchemyLogger.log("TabModOptions::buildDisableAllMods();");
  _itemDisableAllMods_ = new brls::DetailCell();
  _itemDisableAllMods_->setText("Disable all mods");
  _itemDisableAllMods_->setDetailText(
    "Turn all mods off for this game, returning all files to under the \"" + ALCHEMIST_FOLDER + "\" folder. "\
    "This is useful if you want to delete some of them from the SD card."
  );
  _itemDisableAllMods_->registerClickAction([](brls::View* view) {
    brls::Dialog* dialog = new brls::Dialog("Disable all mods? Are you sure?");

    dialog->addButton("Yes", []() {
      brls::Dialog* loadingDialog = DialogUtil::buildLoadingDialog("Disabling all mods. Please wait");
      loadingDialog->open();

      new std::thread([loadingDialog]() {
        controller.deactivateAll();
        loadingDialog->close();
      });
    });
    dialog->addButton("No", []() {});

    dialog->setCancelable(true);
    dialog->open();
    return true;
  });
}

void TabModOptions::initialize() {
  alchemyLogger.log("TabModOptions::initialize();");

  this->buildDisableAllMods();
  this->addView(_itemDisableAllMods_);
}




