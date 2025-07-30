//
// Created by Adrien BLANCHET on 21/06/2020.
//

#include "ModBrowser.h"
#include "FrameModBrowser.h"

#include <StateAlchemist/controller.h>
#include <StateAlchemist/meta_manager.h>
#include <AlchemistLogger.h>



ModBrowser::ModBrowser(FrameModBrowser* owner_): _owner_(owner_) {}

void ModBrowser::loadFirstPage() {
  this->clear(true);
  this->_page_ = 0;

  this->_source_names_ = controller.loadSources(true);

  if (this->_source_names_.empty()) {
    this->displayNoMods();
    return;
  }

  this->appendNextPage();
  
  this->willAppear(true); /* Scroll to the top */
}

void ModBrowser::displayNoMods() {
  this->clear(true);

  brls::ListItem* item = new brls::ListItem(
    "No mods have been found in " + controller.getGroupPath(),
    "Put mods within that folder like this: ./<thing-being-replaced>/<mod-name>/<file-structure-in-installed-directory>"
  );

  item->show([](){}, false);

  this->addView(item);
  this->willAppear(true);
}

void ModBrowser::appendLoadItem() {
  auto* item = new brls::ListItem("Show more...");

  item->getClickEvent()->subscribe([this](brls::View* view) {
    this->appendNextPage();
  });

  this->addView(item);
}

void ModBrowser::handleModSelect(const ModSource& mod, size_t selectedIndex) {

  // Note: selection is -1 if backed out of selecting
  if (selectedIndex == -1) return;

  controller.source = mod.source;

  if (selectedIndex == 0) {
    // If the default option was chosen, deactivate whatever mod is active:
    controller.deactivateMod();
  } else {

    // mod.mods doesn't have the default option at the begining, so index must be offset by -1:
    std::string modToActivate(mod.mods[selectedIndex - 1]);

    // If the mod was changed, deactivate the old one and activate the new one:
    if (controller.getActiveMod(mod.source) != modToActivate) {
      controller.deactivateMod();
      controller.activateMod(modToActivate);
    }
  }
}

void ModBrowser::appendNextPage() {
  this->_page_++;
  
  ModManager& modManager = this->getModManager();
  size_t sourceCount = this->_source_names_.size();

  if (this->_page_ != 1) {
    // Would typically call "remove",
    // but it was crashing the app (probably because its being initiated within a lambda that belongs to itself).
    //
    // So the next best thing is to just collapse it.
    //
    // It will still remain in memory, unused, but there shouldn't be many of these.
    //
    // The last child should be the load item.
    this->getChild(this->getViewsCount() - 1)->collapse(false);
  }

  size_t pageStart = this->getFirstIndex(this->_page_);
  size_t nextPageStart = this->getFirstIndex(this->_page_ + 1);
  for (
    size_t i = pageStart;
    i < nextPageStart && i < sourceCount;
    i++
  ) {
    ModSource source = modManager.loadSource(this->_source_names_[i]);
    std::vector<std::string> options = source.mods;

    // Add the option for choosing to use no mod
    options.insert(options.begin(), _DEFAULT_LABEL_);

    brls::SelectListItem* item = new brls::SelectListItem(
      source.source,
      MetaManager::limitSelectLabels(options),
      source.activeIndex + 1, // Add 1 for the no-mod option added to the beginning
      ""
    );

    item->getValueSelectedEvent()->subscribe([this, &source](size_t selection) {
      this->handleModSelect(source, selection);
    });

    this->addView(item);
  }

  // If there are more items not yet loaded, show the load-more item
  if (sourceCount > nextPageStart) {
    this->appendLoadItem();
  }

  // Update scrollable areas in response to the added items above
  this->setUpdateScrollingOnNextFrame(true);
}

size_t ModBrowser::getFirstIndex(size_t page) {
  if (page == 1) return 0;
  
  size_t index = _FIRST_PAGE_SIZE_;
  for (size_t i = 2; i < page; i++) {
    index += _SEQUENT_PAGE_SIZE_;
  }
  
  return index;
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
