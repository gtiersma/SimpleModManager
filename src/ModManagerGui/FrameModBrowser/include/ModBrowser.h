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
    ModDataSource(std::function<void (brls::SelectorCell* selector, const ModSource& mod, const int& index)> selector_config_fn);

    int numberOfRows(brls::RecyclerFrame* recycler, int section) override;
    brls::RecyclerCell* cellForRow(brls::RecyclerFrame* recycler, brls::IndexPath index) override;

  private:
    // Configures a selector cell for the specified mod source that located at the specified index in the list
    std::function<void (brls::SelectorCell* selector, const ModSource& mod, const int& index)> _selector_config_fn_{nullptr};

    int getModSourceCount();
    bool hasModSources();
};

class ModBrowser : public brls::Box {

public:
  explicit ModBrowser(brls::View* parentCell);

private:
  BRLS_BIND(brls::RecyclerFrame, modList, "mod-list");

  brls::View* _parent_cell_{nullptr};

  /**
   * Called when the value of one of the select items is changed
   */
  void handleModSelect(const ModSource& mod, size_t selectedIndex);

  void configureModSelector(brls::SelectorCell* selector, const ModSource& mod, const int& index);
};


#endif //SIMPLEMODMANAGER_MODBROWSER_H
