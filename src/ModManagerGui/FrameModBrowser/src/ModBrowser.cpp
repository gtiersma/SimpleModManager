//
// Created by Adrien BLANCHET on 21/06/2020.
//

#include "ModBrowser.h"
#include "FrameModBrowser.h"

#include <StateAlchemist/controller.h>
#include <StateAlchemist/meta_manager.h>



ModDataSource::ModDataSource(
  const std::vector<std::string>& mod_source_names,
  std::function<void (const ModSource& source, int selectedModIndex)> on_selected
) {
  this->_mod_source_names_ = mod_source_names;
  this->_on_selected_ = on_selected;

  if (mod_source_names.size() > 0) {
    this->loadNextPage();
  }
}

void ModDataSource::loadNextPage() {
  size_t pageStart = this->getFirstIndexOfCurrentPage();
  size_t nextPageStart = this->getFirstIndexOfNextPage();
  int sourceCount = this->getModSourceCount();

  for (
    size_t i = pageStart;
    i < nextPageStart && i < sourceCount;
    i++
  ) {
    this->_loaded_mod_sources_.push_back(
      gameBrowser.getModManager().loadSource(this->_mod_source_names_[i])
    );
  }

  this->_page_++;
}

int ModDataSource::getModSourceCount() {
  return this->_mod_source_names_.size();
}

int ModDataSource::getLoadedModSourceCount() {
  return this->_loaded_mod_sources_.size();
}

bool ModDataSource::hasModSources() {
  return this->getModSourceCount() > 0;
}

bool ModDataSource::hasLoadedAllModSources() {
  return this->getFirstIndexOfNextPage() >= this->getModSourceCount();
}

size_t ModDataSource::getFirstIndexOfCurrentPage() {
  return this->getFirstIndex(this->_page_);
}

size_t ModDataSource::getFirstIndexOfNextPage() {
  return this->getFirstIndex(this->_page_ + 1);
}

int ModDataSource::numberOfRows(brls::RecyclerFrame* recycler, int section) {
  if (this->hasLoadedAllModSources()) {
    return this->getModSourceCount();
  } else {
    return this->getFirstIndexOfNextPage() + 1;
  }
}

brls::RecyclerCell* ModDataSource::cellForRow(brls::RecyclerFrame* recycler, brls::IndexPath indexPath)
{
  if (!this->hasModSources()) {

    // CASE: No mod sources in this group
    brls::DetailCell* item = (brls::DetailCell*)recycler->dequeueReusableCell("Detail");
    item->setText("No mods have been found in " + controller.getGroupPath());
    item->setDetailText(
      "Within that folder, organize the mods in this manner: ./<thing-being-replaced>/<mod-name>/<file-structure-in-installed-directory>"
    );
    return item;

  } else if (indexPath.row == this->getLoadedModSourceCount()) {

    // CASE: Not all mods loaded into 1 page, and this is the last index, so make it a button to show more
    brls::DetailCell* item = (brls::DetailCell*)recycler->dequeueReusableCell("Detail");
    item->setText("Show more...");
    return item;

  } else {

    // CASE: Selector for mods
    brls::SelectorCell* item = (brls::SelectorCell*)recycler->dequeueReusableCell("Selector");
    ModSource& source = this->_loaded_mod_sources_[indexPath.row];
    item->init(
      source.source,
      source.options,
      source.activeIndex + 1, // Add 1 for the no-mod option added to the beginning
      [](int selected) {},
      [](int selected) {}
    );
    return item;

  }
}

void ModDataSource::didSelectRowAt(brls::RecyclerFrame* recycler, brls::IndexPath indexPath) {
  if (!this->hasModSources()) { return; }

  if (indexPath.row == this->getLoadedModSourceCount()) {
    this->loadNextPage(); // Show-more item being selected
  } else {
    this->_on_selected_(
      this->_loaded_mod_sources_[indexPath.row],
      indexPath.row
    );
  }
}

size_t ModDataSource::getFirstIndex(size_t page) {
  if (page == 1) return 0;
  
  size_t index = this->_FIRST_PAGE_SIZE_;
  for (size_t i = 2; i < page; i++) {
    index += this->_SEQUENT_PAGE_SIZE_;
  }
  
  return index;
}

ModBrowser::ModBrowser(): brls::Box(brls::Axis::COLUMN) {
  this->_container_ = new brls::RecyclerFrame();
  this->_container_->estimatedRowHeight = 70;

  this->_container_->registerCell("Selector", []() { return new brls::SelectorCell(); });
  this->_container_->registerCell("Detail", []() { return new brls::DetailCell(); });

  this->_recycled_data_ = new ModDataSource(
    controller.loadSources(true),
    [this](const ModSource& source, int selectedModIndex) {
      this->handleModSelect(source, selectedModIndex);
    }
  );

  this->_container_->setDataSource(this->_recycled_data_);
  this->addView(this->_container_);
}

void ModBrowser::handleModSelect(const ModSource& mod, size_t selectedIndex) {

  // Note: selection is -1 if backed out of selecting
  if (selectedIndex == -1) return;

  controller.source = mod.source;

  if (selectedIndex == 0) {
    // If the default option was chosen, deactivate whatever mod is active:
    controller.deactivateMod();
  } else if (mod.activeIndex != selectedIndex - 1) {
    // If the mod was changed, deactivate the old one and activate the new one:
    controller.deactivateMod();
    // mod.mods doesn't have the default option at the begining, so index must be offset by -1:
    controller.activateMod(mod.mods[selectedIndex - 1]);
  }
}