//
// Created by Adrien BLANCHET on 21/06/2020.
//

#include "ModBrowser.h"
#include "FrameModBrowser.h"

#include <StateAlchemist/controller.h>
#include <StateAlchemist/meta_manager.h>



ModBrowser::ModBrowser(FrameModBrowser* owner_): _owner_(owner_) {}

void ModBrowser::loadMods(std::string group) {
  this->clear();

  controller.group = group;

  ModManager& modManager = this->getModManager();

  // Fetch the available mods
  modManager.updateModList();
  std::vector<ModSource> mods = modManager.getGroupedModList();

  if (mods.empty()) {
    brls::ListItem* item = new brls::ListItem(
      "No mods have been found in " + controller.getGroupPath(),
      "Put mods within that folder like this: ./<thing-being-replaced>/<mod-name>/<file-structure-in-installed-directory>"
    );

    item->show([](){}, false);

    this->addView(item);
    this->willAppear(true);
    return;
  }

  for (auto& mod : mods) {
    std::vector<std::string> options = mod.mods;
    options.insert(options.begin(), _DEFAULT_LABEL_);

    brls::SelectListItem* item = new brls::SelectListItem(
      mod.source,
      MetaManager::limitSelectLabels(options),
      modManager.getActiveIndex(mod) + 1,
      ""
    );

    item->getValueSelectedEvent()->subscribe([mod](size_t selection) {
      controller.source = mod.source;

      // Note: selection is -1 if backed out of selecting
      if (selection == -1) { return; }

      controller.source = mod.source;

      if (selection == 0) {
        // If the default option was chosen, deactivate whatever mod is active:
        controller.deactivateMod();
      } else {

        // mod.mods doesn't have the default option at the begining, so index must be offset by -1:
        std::string modToActivate(mod.mods[selection - 1]);

        // If the mod was changed, deactivate the old one and activate the new one:
        if (controller.getActiveMod(mod.source) != modToActivate) {
          controller.deactivateMod();
          controller.activateMod(modToActivate);
        }
      }
    });

    this->addView(item);
    this->willAppear(true); /* Scroll to the top */
  }
}

void ModBrowser::draw(
  NVGcontext *vg,
  int x,
  int y,
  unsigned int width,
  unsigned int height,
  brls::Style *style,
  brls::FrameContext *ctx
) {
  ScrollView::draw(vg, x, y, width, height, style, ctx);
}

ModManager& ModBrowser::getModManager() {
  return _owner_->getGameBrowser().getModManager();
}
