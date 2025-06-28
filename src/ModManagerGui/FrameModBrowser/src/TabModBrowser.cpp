//
// Created by Adrien BLANCHET on 21/06/2020.
//

#include "TabModBrowser.h"
#include "FrameModBrowser.h"
#include "GenericToolbox.Macro.h"

#include <future>
#include <StateAlchemist/controller.h>
#include <AlchemistLogger.h>


TabModBrowser::TabModBrowser(FrameModBrowser* owner_, std::string group_) : _owner_(owner_) {
  controller.group = group_;

  alchemyLogger.log("TabModBrowser::TabModBrowser();");

  ModManager& modManager = this->getModManager();

  // Fetch the available mods
  modManager.updateModList();
  _mods_ = modManager.getGroupedModList();

  if (_mods_.empty()) {
    brls::ListItem* item = new brls::ListItem(
      "No mods have been found in " + controller.getGroupPath(),
      "Put mods within that folder like this: ./<thing-being-replaced>/<mod-name>/<file-structure-in-installed-directory>"
    );

    item->show([](){}, false);

    this->addView(item);
    return;
  }

  for (auto& mod : _mods_) {
    std::vector<std::string> options = mod.mods;
    options.push_back(_DEFAULT_LABEL_);

    mod.item = new brls::SelectListItem(mod.source, options, modManager.getActiveIndex(mod) + 1, "");
    this->addView(mod.item);
  }
}

// Apply any changes made once the class is destroyed
TabModBrowser::~TabModBrowser() {
  alchemyLogger.log("TAB MOD BROWSER: destroying!!!");
  for (auto& mod : _mods_) {
    controller.source = mod.source;

    // getSelectedValue counts the default mod at index 0 which isn't a mod, so subtract 1 from it to get the real mod index
    unsigned activeIndex = mod.item->getSelectedValue() - 1;

    // Because of the subtraction of 1, it will be -1 if there is no active mod, so remove the active mod if that's the case
    if (activeIndex == -1) {
      controller.deactivateMod();
      alchemyLogger.log("TAB MOD BROWSER: deactivated in destruction");
    } else if (controller.getActiveMod(mod.source) != mod.mods[activeIndex]) {
      alchemyLogger.log("TAB MOD BROWSER: activated " + mod.mods[activeIndex] + " in destruction");
      // If the active mod was changed in the UI, update it in the filesystem:
      controller.activateMod(mod.mods[activeIndex]);
    }
  }
}

void TabModBrowser::draw(NVGcontext *vg, int x, int y, unsigned int width, unsigned int height, brls::Style *style,
                         brls::FrameContext *ctx) {
  ScrollView::draw(vg, x, y, width, height, style, ctx);
}

ModManager& TabModBrowser::getModManager() {
  return _owner_->getGameBrowser().getModManager();
}
