//
// Created by Adrien BLANCHET on 21/06/2020.
//

#include "TabModBrowser.h"
#include "FrameModBrowser.h"
#include "GenericToolbox.Macro.h"

#include <future>
#include <StateAlchemist/controller.h>


TabModBrowser::TabModBrowser(FrameModBrowser* owner_, std::string group_) : _owner_(owner_) {
  controller.group = group_;

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

    brls::SelectListItem* item = new brls::SelectListItem(mod.source, options, modManager.getActiveIndex(mod) + 1, "");

    item->getValueSelectedEvent()->subscribe([&, mod](size_t selection) {
      controller.source = mod.source;

      // Index 0 is the default option
      if (selection == 0) {
        controller.deactivateMod();
      } else {
        // mod.mods doesn't have the default option at the begin, so index must be offset by -1:
        std::string activeMod(mod.mods[selection - 1]);
        
        if (controller.getActiveMod(mod.source) != activeMod) {
          controller.activateMod(activeMod);
        }
      }
    });

    this->addView(item);
  }
}

void TabModBrowser::draw(NVGcontext *vg, int x, int y, unsigned int width, unsigned int height, brls::Style *style,
                         brls::FrameContext *ctx) {
  ScrollView::draw(vg, x, y, width, height, style, ctx);
}

ModManager& TabModBrowser::getModManager() {
  return _owner_->getGameBrowser().getModManager();
}
