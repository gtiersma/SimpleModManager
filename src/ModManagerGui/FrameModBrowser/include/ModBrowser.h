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
    ModDataSource(std::function<void (const ModSource& source, int selectedModIndex)> on_selected);

    int numberOfRows(brls::RecyclerFrame* recycler, int section) override;
    brls::RecyclerCell* cellForRow(brls::RecyclerFrame* recycler, brls::IndexPath index) override;

  private:
    // The action to perform when a mod is changed
    std::function<void (const ModSource& source, int selectedModIndex)> _on_selected_{nullptr};

    int getModSourceCount();
    bool hasModSources();
};

class ModBrowser : public brls::Box {

public:
  explicit ModBrowser();

private:
  BRLS_BIND(brls::RecyclerFrame, modList, "mod-list");

  /**
   * Called when the value of one of the select items is changed
   */
  void handleModSelect(const ModSource& mod, size_t selectedIndex);
};


#endif //SIMPLEMODMANAGER_MODBROWSER_H
