//
// Created by Adrien BLANCHET on 22/06/2020.
//

#include "TabModOptions.h"
#include "FrameModBrowser.h"

#include <StateAlchemist/controller.h>
#include <util.hpp>


TabModOptions::TabModOptions() {
  Util::padTabContent(this);
  this->buildDisableAllMods();
}

void TabModOptions::buildDisableAllMods() {
  brls::NoteCell* disableAll = new brls::NoteCell();
  disableAll->setText("Disable all mods");
  disableAll->setNote(
    "Turn all mods off for this game, returning all files to under the \"" + ALCHEMIST_FOLDER + "\" folder. "\
    "This is useful if you want to delete some of them from the SD card."
  );
  disableAll->registerClickAction([](brls::View* view) {
    brls::Dialog* dialog = new brls::Dialog("Disable all mods? Are you sure?");

    dialog->addButton("Yes", []() {
      brls::Dialog* loadingDialog = Util::buildLoadingDialog("Disabling all mods. Please wait");
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
  this->addView(disableAll);
}

TabModOptions* TabModOptions::create() {
  return new TabModOptions();
}


