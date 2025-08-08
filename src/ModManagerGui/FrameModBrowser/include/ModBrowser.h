//
// Created by Adrien BLANCHET on 21/06/2020.
//

#ifndef SIMPLEMODMANAGER_MODBROWSER_H
#define SIMPLEMODMANAGER_MODBROWSER_H


#include <GuiModManager.h>
#include "ModManager.h"

#include <borealis.hpp>

#include <vector>
#include <string>


class FrameModBrowser;

class ModBrowser : public brls::ScrollingFrame {

public:
  explicit ModBrowser(FrameModBrowser* owner_);

  ModManager& getModManager();

private:
  brls::Box* _container_{nullptr};
  brls::Button* _load_button_{nullptr};
  FrameModBrowser* _owner_{nullptr};

  // The last page # of mod sources that was loaded
  size_t _page_{0};

  // Vector of all moddable things (aka sources) available under the current group
  std::vector<std::string> _source_names_;

  /**
   * Clears the list, adding a message that there are no mods
   */
  void displayNoMods();

  /**
   * Appends a button to the end of the list for loading the next page of sources.
   * The button is available as "_load_button_".
   */
  void appendLoadButton();

  /**
   * Called when the value of one of the select items is changed
   */
  void handleModSelect(const ModSource& mod, size_t selectedIndex);

  /**
   * Loads another page of mod sources
   */
  void appendNextPage();

  /**
   * Gets what the first item index should be in the page specified
   */
  size_t getFirstIndex(size_t page);

  // Label used for the setting to turn a mod off
  const std::string _DEFAULT_LABEL_{"UNMODIFIED"}; 

  // The number of items that the first page should have
  // (small because it loads automatically when navigating over groups).
  const size_t _FIRST_PAGE_SIZE_{10};

  // The number of items all pages after the first should have
  const size_t _SEQUENT_PAGE_SIZE_{30};
};


#endif //SIMPLEMODMANAGER_MODBROWSER_H
