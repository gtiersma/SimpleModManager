//
// Created by Adrien BLANCHET on 21/06/2020.
//

#ifndef SIMPLEMODMANAGER_FRAMEMODBROWSER_H
#define SIMPLEMODMANAGER_FRAMEMODBROWSER_H

#include "borealis.hpp"


class FrameModBrowser : public brls::Activity {

public:
  CONTENT_FROM_XML_RES("FrameModBrowser/frame_mod_browser.xml");

  void initialize();
};


#endif //SIMPLEMODMANAGER_FRAMEMODBROWSER_H
