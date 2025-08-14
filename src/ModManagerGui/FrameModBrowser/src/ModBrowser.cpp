//
// Created by Adrien BLANCHET on 21/06/2020.
//

#include "ModBrowser.h"
#include "FrameModBrowser.h"

#include <StateAlchemist/controller.h>
#include <StateAlchemist/meta_manager.h>
#include <AlchemistLogger.h>



ModBrowser::ModBrowser() {
  this->_container_ = new brls::Box();
  this->addView(this->_container_);

  alchemyLogger.log("ModBrowser::loadFirstPage: loading sources...");
  this->_source_names_ = controller.loadSources(true);

  if (this->_source_names_.empty()) {
    alchemyLogger.log("ModBrowser::loadFirstPage: no sources...");
    this->displayNoMods();
    return;
  }

  this->appendNextPage();
}

void ModBrowser::displayNoMods() {
  brls::DetailCell* message = new brls::DetailCell();
  message->setText("No mods have been found in " + controller.getGroupPath());
  message->setDetailText(
    "Within that folder, organize the mods in this manner: ./<thing-being-replaced>/<mod-name>/<file-structure-in-installed-directory>"
  );
  this->_container_->addView(message);
}

void ModBrowser::appendLoadButton() {
  brls::Button* button = new brls::Button();
  button->setText("Show more...");
  button->registerClickAction([this](brls::View* view) {
    this->appendNextPage();
    return true;
  });
  this->_container_->addView(button);
  this->_load_button_ = button;
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
  
  size_t sourceCount = this->_source_names_.size();

  // Whether this page is being loaded from the load-more button or not
  bool wasLoadButtonUsed = this->_page_ != 1;

  if (wasLoadButtonUsed) {
    this->removeView(this->_load_button_);
  }

  size_t pageStart = this->getFirstIndex(this->_page_);
  size_t nextPageStart = this->getFirstIndex(this->_page_ + 1);
  for (
    size_t i = pageStart;
    i < nextPageStart && i < sourceCount;
    i++
  ) {
    alchemyLogger.log("ModBrowser::appendNextPage: loading source... " + this->_source_names_[i]);
    ModSource source = gameBrowser.getModManager().loadSource(this->_source_names_[i]);
    std::vector<std::string> options = source.mods;

    // Add the option for choosing to use no mod
    options.insert(options.begin(), _DEFAULT_LABEL_);

    std::vector<std::string> limitedLabels = MetaManager::limitSelectLabels(options);
    for (std::string& label : limitedLabels) {
      alchemyLogger.log("ModBrowser::appendNextPage: constructing item with label... " + label);
    }

    std::vector<std::string> limitedLabels = MetaManager::limitSelectLabels(options);
    for (std::string& label : limitedLabels) {
      alchemyLogger.log("ModBrowser::appendNextPage: constructing item with label... " + label);
    }

    brls::SelectorCell* selector = new brls::SelectorCell();
    selector->init(
      source.source,
      MetaManager::limitSelectLabels(options),
      source.activeIndex + 1, // Add 1 for the no-mod option added to the beginning
      [](int selected) {},
      [this, source](int selected) { this->handleModSelect(source, selected); }
    );
    this->_container_->addView(selector);
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
      brls::Application::giveFocus(selector);
    }
  }

  // If there are more items not yet loaded, show the load-more button
  if (sourceCount > nextPageStart) {
    alchemyLogger.log("ModBrowser::appendNextPage: adding load item... ");
    this->appendLoadButton();
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