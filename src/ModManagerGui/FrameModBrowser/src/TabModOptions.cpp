//
// Created by Adrien BLANCHET on 22/06/2020.
//

#include "TabModOptions.h"
#include "FrameModBrowser.h"




#include "Logger.h"
#include <StateAlchemist/controller.h>

LoggerInit([]{
  Logger::setUserHeaderStr("[TabModOptions]");
});


TabModOptions::TabModOptions(FrameModBrowser* owner_) : _owner_(owner_) {  }

void TabModOptions::buildDisableAllMods() {

  _itemDisableAllMods_ = new brls::ListItem(
    "\uE872 Disable all installed mods",
    "This option will disable all installed mods files (useful if you want to delete some from the SD card).",
    ""
  );

  _itemDisableAllMods_->getClickEvent()->subscribe([this](View* view){

    auto* dialog = new brls::Dialog("Do you want to disable all installed mods ?");

    dialog->addButton("Yes", [&, dialog](brls::View* view) {
      // first, close the dialog box before the async routine starts
      dialog->close();

      // starts the async routine
      _owner_->getGuiModManager().startRemoveAllModsThread();
    });
    dialog->addButton("No", [dialog](brls::View* view) {
      dialog->close();
    });

    dialog->setCancelable(true);
    dialog->open();

  });

}

void TabModOptions::initialize() {

  this->buildDisableAllMods();

  // finally add to view
  this->addView(_itemDisableAllMods_);

}

void TabModOptions::draw(NVGcontext *vg, int x, int y, unsigned int width, unsigned int height, brls::Style *style,
                         brls::FrameContext *ctx) {
  ScrollView::draw(vg, x, y, width, height, style, ctx);
}

const ModManager &TabModOptions::getModManager() const {
  return _owner_->getGameBrowser().getModManager();
}
ModManager &TabModOptions::getModManager() {
  return _owner_->getGameBrowser().getModManager();
}




