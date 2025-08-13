//
// Created by Adrien BLANCHET on 22/06/2020.
//

#ifndef SIMPLEMODMANAGER_FRAMEROOT_H
#define SIMPLEMODMANAGER_FRAMEROOT_H

#include "GuiModManager.h"

#include "borealis.hpp"


class FrameRoot : public brls::Activity {

public:
  CONTENT_FROM_XML_RES("FrameGameBrowser/FrameRoot.xml");
};


#endif //SIMPLEMODMANAGER_FRAMEROOT_H
