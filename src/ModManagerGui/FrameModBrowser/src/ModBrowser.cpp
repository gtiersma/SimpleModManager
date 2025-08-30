//
// Created by Adrien BLANCHET on 21/06/2020.
//

#include "ModBrowser.h"
#include "FrameModBrowser.h"

#include <StateAlchemist/controller.h>
#include <StateAlchemist/meta_manager.h>
#include <note_cell.hpp>



ModDataSource::ModDataSource(std::function<void (const ModSource& source, int selectedModIndex)> on_selected) {
  this->_on_selected_ = on_selected;
}

int ModDataSource::getModSourceCount() {
  return gameBrowser.getModManager().getSourceCount();
}

bool ModDataSource::hasModSources() {
  return this->getModSourceCount() > 0;
}

int ModDataSource::numberOfRows(brls::RecyclerFrame* recycler, int section) {
  if (this->hasModSources()) {
    return this->getModSourceCount();
  }

  // For the no-mods message:
  return 1;
}

brls::RecyclerCell* ModDataSource::cellForRow(brls::RecyclerFrame* recycler, brls::IndexPath indexPath)
{
  ModManager& modManager = gameBrowser.getModManager();

  if (!this->hasModSources()) {

    // CASE: No mod sources in this group
    brls::NoteCell* item = (brls::NoteCell*)recycler->dequeueReusableCell("Note");
    item->setText("No mods have been found in " + controller.getGroupPath());
    item->setNote(
      "Within that folder, organize the mods in this manner: ./<thing-being-replaced>/<mod-name>/<file-structure-in-installed-directory>"
    );
    return item;

  } else if (modManager.isSourceLoaded(indexPath.row)) {
    
    // CASE: Selector for mods
    brls::SelectorCell* item = (brls::SelectorCell*)recycler->dequeueReusableCell("Selector");
    ModSource& source = modManager.getSource(indexPath.row);
    item->init(
      source.source,
      source.options,
      source.activeIndex + 1, // Add 1 for the no-mod option added to the beginning
      [](int selected) {},
      [this, source](int selected) {
        this->_on_selected_(source, selected);
      }
    );
    item->getFocusEvent()->subscribe([indexPath](brls::View* view) {
      gameBrowser.getModManager().loadSourcesIfNeeded(indexPath.row);
    });
    return item;

  }

  // CASE: Selectors for mods that aren't yet loaded.
  // These should technically never be seen in the UI, but they're here for safety in case something is off.
  brls::SelectorCell* item = (brls::SelectorCell*)recycler->dequeueReusableCell("Selector");
  item->init(modManager.getSourceName(indexPath.row), { "UNKNOWN" }, 0, [](int selected) {}, [](int selected) {});
  return item;
}

ModBrowser::ModBrowser() {
  this->inflateFromXMLRes("xml/FrameModBrowser/mod_browser.xml");

  modList->estimatedRowHeight = 70;

  modList->registerCell("Selector", []() { return new brls::SelectorCell(); });
  modList->registerCell("Note", []() { return new brls::NoteCell(); });

  modList->setDataSource(
    new ModDataSource(
      [this](const ModSource& source, int selectedModIndex) {
        this->handleModSelect(source, selectedModIndex);
      }
    )
  );
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