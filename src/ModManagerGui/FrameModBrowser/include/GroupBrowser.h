//
// Derived from a file created by Adrien BLANCHET on 21/06/2020.
//

#ifndef SIMPLEMODMANAGER_GROUPBROWSER_H
#define SIMPLEMODMANAGER_GROUPBROWSER_H


#include "borealis.hpp"

#include "ModBrowser.h"


class GroupBrowser : public brls::List {

  public:

    // Short width to provide more room for the mod list
    static const int WIDTH = 300;

    explicit GroupBrowser(ModBrowser* mod_browser_);

  private:
    ModBrowser* _modBrowser_{};

    void willDisappear(bool resetState = false) override;
};

#endif //SIMPLEMODMANAGER_GROUPBROWSER_H
