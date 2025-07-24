//
// Created by Adrien BLANCHET on 22/06/2020.
//

#include "TabModOptions.h"
#include "FrameModBrowser.h"

#include "Logger.h"
#include <StateAlchemist/controller.h>
#include <dialog_util.hpp>
#include <AlchemistLogger.h>

LoggerInit([]{
  Logger::setUserHeaderStr("[TabModOptions]");
});


TabModOptions::TabModOptions(FrameModBrowser* owner_) : _owner_(owner_) {  }

void TabModOptions::buildDisableAllMods() {
  alchemyLogger.log("TabModOptions::buildDisableAllMods();");

  _itemDisableAllMods_ = new brls::ListItem(
    "Disable all mods",
    "Turn all mods off for this game, returning all files to under the \"" + ALCHEMIST_FOLDER + "\" folder. "\
    "This is useful if you want to delete some of them from the SD card.",
    ""
  );

  _itemDisableAllMods_->getClickEvent()->subscribe([this](View* view){

    auto* dialog = new brls::Dialog("Disable all mods? Are you sure?");

    dialog->addButton("Yes", [dialog](brls::View* view) {
      auto* loadingDialog = DialogUtil::buildLoadingDialog("Disabling all mods. Please wait");
      loadingDialog->open();

      new std::thread([dialog, loadingDialog]() {
        controller.deactivateAll();
        loadingDialog->close([dialog]() { dialog->close(); });
        brls::Application::unblockInputs();
      });
    });
    dialog->addButton("No", [dialog](brls::View* view) {
      dialog->close();
    });

    dialog->setCancelable(true);
    dialog->open();

  });

}

void TabModOptions::initialize() {
  alchemyLogger.log("TabModOptions::initialize();");

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




