//
// Created by Adrien BLANCHET on 21/06/2020.
//

#include "ModBrowser.h"
#include "FrameModBrowser.h"

#include <StateAlchemist/controller.h>
#include <AlchemistLogger.h>



ModBrowser::ModBrowser(FrameModBrowser* owner_): _owner_(owner_) {}

void ModBrowser::loadMods(std::string group) {
  this->clear();

  controller.group = group;

  alchemyLogger.log("TabModBrowser::TabModBrowser();");

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
    return;
  }

  for (auto& mod : mods) {
    std::vector<std::string> options = mod.mods;
    options.insert(options.begin(), _DEFAULT_LABEL_);

    brls::SelectListItem* item = new brls::SelectListItem(mod.source, options, modManager.getActiveIndex(mod) + 1, "");

    item->getValueSelectedEvent()->subscribe([mod, mod](size_t selection) {
      alchemyLogger.log("TAB MOD BROWSER: getValueSelectedEvent: " + std::to_string(selection));
      controller.source = mod.source;

      // Note: selection is -1 if backed out of selecting
      if (selection == -1) { return; }

      if (selection == 0) {
        // If the default option was chosen, deactivate whatever mod is active:
        alchemyLogger.log("TAB MOD BROWSER: Applying mod " + controller.source + " deactivation");
        controller.deactivateMod();
      } else {

        // mod.mods doesn't have the default option at the begining, so index must be offset by -1:
        std::string modToActivate(mod.mods[selection - 1]);
        std::string currentlyActiveMod(controller.getActiveMod(mod.source));

        alchemyLogger.log("TAB MOD BROWSER: Selected activation: " + modToActivate);
        alchemyLogger.log("TAB MOD BROWSER: Currently activated: " + currentlyActiveMod);

        if (currentlyActiveMod != modToActivate) {
          alchemyLogger.log("TAB MOD BROWSER: Applying mod " + mod.source + " deactivation");
          controller.deactivateMod();
          alchemyLogger.log("TAB MOD BROWSER: Applying mod " + modToActivate + " activation");
          controller.activateMod(modToActivate);
        }
      }
    });

    this->addView(item);
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
