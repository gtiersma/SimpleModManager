//
// Derived from a file created by Adrien BLANCHET on 21/06/2020.
//

#ifndef SIMPLEMODMANAGER_TABGROUPBROWSER_H
#define SIMPLEMODMANAGER_TABGROUPBROWSER_H


#include <GuiModManager.h>
#include "ModManager.h"

#include <borealis.hpp>

#include "map"
#include "string"


class FrameModBrowser;

class TabGroupBrowser : public brls::TabFrame {

public:
  explicit TabGroupBrowser(FrameModBrowser* owner_);
  bool onCancel() override;

private:
  FrameModBrowser* _owner_{nullptr};
};

#endif //SIMPLEMODMANAGER_TABGROUPBROWSER_H
