//
// Derived from a file created by Adrien BLANCHET on 21/06/2020.
//

#ifndef SIMPLEMODMANAGER_GROUPBROWSER_H
#define SIMPLEMODMANAGER_GROUPBROWSER_H


#include "borealis.hpp"

#include "ModBrowser.h"


class GroupBrowser : public brls::Box {
  public:
    explicit GroupBrowser();

    static GroupBrowser* create();

  private:
    BRLS_BIND(brls::Sidebar, groupList, "group-list");

    ModBrowser* _current_mod_browser_{nullptr};
};

#endif //SIMPLEMODMANAGER_GROUPBROWSER_H
