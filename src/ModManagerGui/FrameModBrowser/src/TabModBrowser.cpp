//
// Created by Adrien BLANCHET on 21/06/2020.
//

#include "TabModBrowser.h"
#include "FrameModBrowser.h"

#include <StateAlchemist/controller.h>


bool TabModBrowser::_shouldReloadActiveMods_ = false;

TabModBrowser::TabModBrowser(FrameModBrowser* owner_, std::string group_): _owner_(owner_), _group_(group_) {
  controller.group = _group_;

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
    options.insert(options.begin(), _DEFAULT_LABEL_);

    brls::SelectListItem* item = new brls::SelectListItem(mod.source, options, modManager.getActiveIndex(mod) + 1, "");

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

    this->_items_.push_back(item);
    this->addView(item);
  }
}

void TabModBrowser::reloadActiveMods() {
  ModManager& modManager = this->getModManager();

  for (int i = 0; i < this->_items_.size(); i++) {
    _items_[i]->setSelectedValue(modManager.getActiveIndex(_mods_[i]) + 1);
  }
}

void TabModBrowser::draw(NVGcontext *vg, int x, int y, unsigned int width, unsigned int height, brls::Style *style,
                         brls::FrameContext *ctx) {

  if (_shouldReloadActiveMods_) {
    this->reloadActiveMods();
    _shouldReloadActiveMods_ = false;
  }

  ScrollView::draw(vg, x, y, width, height, style, ctx);
}

ModManager& TabModBrowser::getModManager() {
  return _owner_->getGameBrowser().getModManager();
}
