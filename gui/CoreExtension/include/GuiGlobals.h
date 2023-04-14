//
// Created by Adrien BLANCHET on 28/06/2020.
//

#ifndef SIMPLEMODMANAGER_GUIGLOBALS_H
#define SIMPLEMODMANAGER_GUIGLOBALS_H

//#include <TabModBrowser.h>
//#include <FrameModBrowser.h>

class TabModBrowser;
class FrameModBrowser;
class TabModPresets;

namespace GuiGlobals {


  void setCurrentTabModBrowserPtr(TabModBrowser* currentTabModBrowserPtr_);
  void setCurrentFrameModBrowserPtr(FrameModBrowser* currentFrameModBrowserPtr_);
  void setCurrentTabModPresetPtr(TabModPresets* currentTabModPresetPtr_);

  TabModBrowser* getCurrentTabModBrowserPtr();
  FrameModBrowser* getCurrentFrameModBrowserPtr();
  TabModPresets* getCurrentTabModPresetPtr();

};


#endif //SIMPLEMODMANAGER_GUIGLOBALS_H
