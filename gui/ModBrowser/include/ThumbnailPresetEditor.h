//
// Created by Adrien BLANCHET on 30/06/2020.
//

#ifndef SIMPLEMODMANAGER_THUMBNAILPRESETEDITOR_H
#define SIMPLEMODMANAGER_THUMBNAILPRESETEDITOR_H

#include <borealis.hpp>

class ThumbnailPresetEditor : public brls::ThumbnailFrame {

public:
  void initialize();
  void process_tags();
  void save();
  void autoAssignPresetName();

  void cleanup();

  void draw(NVGcontext* vg, int x, int y, unsigned width, unsigned height, brls::Style* style, brls::FrameContext* ctx) override;

  void setPresetName(const std::string &presetName);
  std::vector<std::string> & getSelectedModsList();

private:
  std::string _presetName_;
  std::vector<std::string> _selectedModsList_;
  std::vector<brls::ListItem*> itemsList;


};


#endif //SIMPLEMODMANAGER_THUMBNAILPRESETEDITOR_H
