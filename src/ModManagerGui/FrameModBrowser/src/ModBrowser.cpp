//
// Created by Adrien BLANCHET on 21/06/2020.
//

#include "ModBrowser.h"
#include "FrameModBrowser.h"

#include <StateAlchemist/controller.h>
#include <StateAlchemist/meta_manager.h>
#include <AlchemistLogger.h>



ModBrowser::ModBrowser(FrameModBrowser* owner_): _owner_(owner_) {}

void ModBrowser::clearItems() { this->clear(true); }

void ModBrowser::loadFirstPage() {
  alchemyLogger.log("ModBrowser::loadFirstPage: clearing...");
  this->clearItems();
  this->_page_ = 0;

  alchemyLogger.log("ModBrowser::loadFirstPage: loading sources...");
  this->_source_names_ = controller.loadSources(true);

  if (this->_source_names_.empty()) {
    alchemyLogger.log("ModBrowser::loadFirstPage: no sources...");
    this->displayNoMods();
    return;
  }

  this->appendNextPage();
  
  this->willAppear(true); /* Scroll to the top */
}

void ModBrowser::focusTop() {
  brls::Application::giveFocus(this->getChild(0));
}

void ModBrowser::displayNoMods() {
  this->clearItems();

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
  alchemyLogger.log("ModBrowser::handleModSelect: selecting index " + std::to_string(selectedIndex));
  alchemyLogger.log("ModBrowser::handleModSelect: selecting source " + mod.source);

  // Note: selection is -1 if backed out of selecting
  if (selectedIndex == -1) return;

  controller.source = mod.source;

  if (selectedIndex == 0) {
    // If the default option was chosen, deactivate whatever mod is active:
    controller.deactivateMod();
    alchemyLogger.log("ModBrowser::handleModSelect: mod deactivated");
  } else if (mod.activeIndex != selectedIndex - 1) {
    alchemyLogger.log("ModBrowser::handleModSelect: mod to activate: " + mod.mods[selectedIndex - 1]);
    alchemyLogger.log("ModBrowser::handleModSelect: currently active: " + controller.getActiveMod(mod.source));

    // If the mod was changed, deactivate the old one and activate the new one:
    controller.deactivateMod();
    alchemyLogger.log("ModBrowser::handleModSelect: mod deactivated");
    // mod.mods doesn't have the default option at the begining, so index must be offset by -1:
    controller.activateMod(mod.mods[selectedIndex - 1]);
    alchemyLogger.log("ModBrowser::handleModSelect: mod activated");
  }
}

void ModBrowser::appendNextPage() {
  this->_page_++;
  
  ModManager& modManager = this->getModManager();
  size_t sourceCount = this->_source_names_.size();

  // Whether this page is being loaded from the load-more button or not
  bool wasLoadButtonUsed = this->_page_ != 1;

  if (wasLoadButtonUsed) {
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
    alchemyLogger.log("ModBrowser::appendNextPage: loading source... " + this->_source_names_[i]);
    ModSource source = modManager.loadSource(this->_source_names_[i]);
    std::vector<std::string> options = source.mods;

    // Add the option for choosing to use no mod
    options.insert(options.begin(), _DEFAULT_LABEL_);

    std::vector<std::string> limitedLabels = MetaManager::limitSelectLabels(options);
    for (std::string& label : limitedLabels) {
      alchemyLogger.log("ModBrowser::appendNextPage: constructing item with label... " + label);
    }

    brls::SelectListItem* item = new brls::SelectListItem(
      source.source,
      MetaManager::limitSelectLabels(options),
      source.activeIndex + 1, // Add 1 for the no-mod option added to the beginning
      ""
    );
    alchemyLogger.log("ModBrowser::appendNextPage: active " + std::to_string(source.activeIndex - 1));

    item->getValueSelectedEvent()->subscribe([this, item, source](size_t selection) {
      this->handleModSelect(source, selection);

      // Could be my imagination, but I think these lines help prevent the random DataAbort errors that infrequently occur
      item->setSelectedValue(selection);
      this->refresh();
    });

    this->addView(item);

    // When the load-more button is used, it is re-created at the end of the list
    // but we want to preserve the focus position in the list, so move the focus to the first-loaded item
    if (wasLoadButtonUsed && i == pageStart) {
      brls::Application::giveFocus(item);
    }
  }

  // If there are more items not yet loaded, show the load-more item
  if (sourceCount > nextPageStart) {
    alchemyLogger.log("ModBrowser::appendNextPage: adding load item... ");
    this->appendLoadItem();
  }

  // Not sure why, but the list scrolls to a wrong position after the items are loaded
  // AND after the first time focus changes after the items load when more items are appended.
  //
  // To fix this, both scroll positions need to be manually overridden.
  if (wasLoadButtonUsed) {
    size_t itemCount = this->getViewsCount();
    this->scrollOverride = (float)(pageStart - 3) / (float)(itemCount - this->_page_);

    // Don't ask what this calculation actually means,
    // but it seems to scroll to approximately the right position each time a new page of content is loaded.
    this->startScrolling(
      true,
      ((float)(itemCount - 12 + (this->_page_ * 2)) / (float)(itemCount + this->_page_))
    );
  }

  alchemyLogger.log("ModBrowser::appendNextPage: kohmplete... ");
}

size_t ModBrowser::getFirstIndex(size_t page) {
  if (page == 1) return 0;
  
  size_t index = _FIRST_PAGE_SIZE_;
  for (size_t i = 2; i < page; i++) {
    index += _SEQUENT_PAGE_SIZE_;
  }
  
  return index;
}

ModManager& ModBrowser::getModManager() {
  return _owner_->getGameBrowser().getModManager();
}