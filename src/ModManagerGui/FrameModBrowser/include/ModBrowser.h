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

class ModDataSource : public brls::RecyclerDataSource
{
  public:
    ModDataSource(
      const std::vector<std::string>& mod_source_names,
      std::function<void (const ModSource& source, int selectedModIndex)> on_selected
    );

    int numberOfRows(brls::RecyclerFrame* recycler, int section) override;
    brls::RecyclerCell* cellForRow(brls::RecyclerFrame* recycler, brls::IndexPath index) override;
    void didSelectRowAt(brls::RecyclerFrame* recycler, brls::IndexPath indexPath) override;

  private:
    // The last page # of mod sources that was loaded
    size_t _page_{0};

    // Vector of all moddable things (aka sources) available under the current group
    std::vector<std::string> _mod_source_names_;

    // Data related to each mod source (parallel array to mod_source_names).
    // The data is loaded in chunks, so it may not always have an entry for each mod source.
    std::vector<ModSource> _loaded_mod_sources_;

    // The action to perform when a mod is changed
    std::function<void (const ModSource& source, int selectedModIndex)> _on_selected_{nullptr};

    /**
     * Loads the next page of mod source data
     */
    void loadNextPage();

    int getModSourceCount();
    int getLoadedModSourceCount();
    bool hasModSources();
    bool hasLoadedAllModSources();

    /**
     * Gets what the first item index should be in the page specified
     */
    size_t getFirstIndex(size_t page);
    size_t getFirstIndexOfCurrentPage();
    size_t getFirstIndexOfNextPage();

    // The number of items that the first page should have
    // (small because it loads automatically when navigating over groups).
    const size_t _FIRST_PAGE_SIZE_{10};
  
    // The number of items all pages after the first should have
    const size_t _SEQUENT_PAGE_SIZE_{30};
};

class ModBrowser : public brls::Box {

public:
  explicit ModBrowser();

private:
  ModDataSource* _recycled_data_{nullptr};
  brls::RecyclerFrame* _container_{nullptr};

  /**
   * Called when the value of one of the select items is changed
   */
  void handleModSelect(const ModSource& mod, size_t selectedIndex);
};


#endif //SIMPLEMODMANAGER_MODBROWSER_H
