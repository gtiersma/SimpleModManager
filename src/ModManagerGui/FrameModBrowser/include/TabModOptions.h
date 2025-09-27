//
// Created by Adrien BLANCHET on 22/06/2020.
//

#ifndef SIMPLEMODMANAGER_TABMODOPTIONS_H
#define SIMPLEMODMANAGER_TABMODOPTIONS_H

#include <ModBrowser.h>

#include "ModManager.h"

#include <borealis.hpp>
#include <note_cell.hpp>


class FrameModBrowser;

class TabModOptions : public brls::Box {

public:
  explicit TabModOptions();

  void buildRandomPicks();

  void buildDisableAllMods();

  static TabModOptions* create();

};


#endif //SIMPLEMODMANAGER_TABMODOPTIONS_H
