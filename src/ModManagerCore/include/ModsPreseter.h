//
// Created by Nadrino on 13/02/2020.
//

#ifndef SIMPLEMODMANAGER_MODSPRESETER_H
#define SIMPLEMODMANAGER_MODSPRESETER_H

#include "Selector.h"

#include <string>
#include <map>

class ModsPreseter {

public:

  ModsPreseter();
  virtual ~ModsPreseter();

  void initialize();
  void reset();

  void setSelector(Selector selector);

  int get_selected_mod_preset_index();
  std::string get_selected_mod_preset();
  std::vector<std::string> get_mods_list(std::string preset_); // Leave the output as a copy
  std::map<std::string, std::vector<std::string>> &get_data_handler();
  std::vector<std::string>& get_presets_list();

  void read_parameter_file(std::string mod_folder_ = "");
  void recreate_preset_file();
  void select_mod_preset();
  void create_new_preset();
  void delete_mod_preset(std::string preset_name_);
  void edit_preset(std::string preset_name_, std::vector<std::string> selected_mods_list_);
  void show_conflicted_files(std::string &preset_name_);

  std::map<std::string, std::vector<std::string>> get_conflicts_with_other_mods(const std::string& mod_name_);


  void select_previous_mod_preset();
  void select_next_mod_preset();

  void fill_selector();

private:

  int _selected_mod_preset_index_{-1};
  std::string _preset_file_path_;
  std::string _mod_folder_;
  std::map<std::string, std::vector<std::string>> _data_handler_;
  std::vector<std::string> _presets_list_;
  Selector _selector_;

};


#endif //SIMPLEMODMANAGER_MODSPRESETER_H
