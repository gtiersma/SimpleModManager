//
// Created by Nadrino on 16/10/2019.
//

#include "ParametersHandler.h"
#include "Toolbox.h"

#include "GenericToolbox.Switch.h"

#include <iostream>


ParametersHandler::ParametersHandler() { reset(); }
ParametersHandler::~ParametersHandler() { reset(); }

void ParametersHandler::initialize() {

  if(not GenericToolbox::doesPathIsFile(_parameters_file_path_)){
    recreate_parameters_file(); // create file with default parameters
  }

  read_parameters();
  recreate_parameters_file(); // update version + cleanup

  set_current_config_preset_name(_data_handler_["last-preset-used"]);

}
void ParametersHandler::reset() {

  _parameters_file_path_ = "/config/SimpleModManager/parameters.ini";
  _data_handler_.clear();
  _presets_list_.clear();
  set_default_parameters();

}

void ParametersHandler::setParametersFilePath(const std::string &parameters_file_path_) {
  _parameters_file_path_ = parameters_file_path_;
}
void ParametersHandler::setCurrentConfigPresetId(int selected_preset_id_){
  if(selected_preset_id_ < 0 or selected_preset_id_ >= int(_presets_list_.size())) _current_config_preset_id_ = 0; // reset;
  else _current_config_preset_id_ = selected_preset_id_;
  fill_current_preset_parameters();
  recreate_parameters_file(); // save last-preset-used
}
void ParametersHandler::set_current_config_preset_name(const std::string &preset_name_){
  int preset_id = -1;
  for(int i_preset = 0 ; i_preset < int(_presets_list_.size()) ; i_preset++){
    if(preset_name_ == _presets_list_[i_preset]){
      preset_id = i_preset;
      break;
    }
  }
  setCurrentConfigPresetId(preset_id);
}
void ParametersHandler::set_parameter(const std::string &parameter_name_, const std::string &value_){
  _data_handler_[parameter_name_] = value_;
  recreate_parameters_file();
}

int ParametersHandler::get_current_config_preset_id() const{
  return _current_config_preset_id_;
}
std::string ParametersHandler::get_parameter(const std::string& parameter_name_) {
  return _data_handler_[parameter_name_];
}
std::string ParametersHandler::get_parameters_file_path(){
  return _parameters_file_path_;
}
std::string ParametersHandler::get_current_config_preset_name(){
  return _presets_list_[_current_config_preset_id_];
}

std::vector<std::string> & ParametersHandler::get_presets_list(){
  return _presets_list_;
}

void ParametersHandler::increment_selected_preset_id(){
  int next_preset_id = _current_config_preset_id_ + 1;
  if(next_preset_id >= int(_presets_list_.size())) setCurrentConfigPresetId(0);
  else setCurrentConfigPresetId(next_preset_id);
}

void ParametersHandler::set_default_parameters() {

  _data_handler_["stored-mods-base-folder"] = "/mods/";
  _data_handler_["use-gui"] = "1";
  _data_handler_["last-preset-used"] = "default";

  // while adding a new preset dependant parameter
  // don't forget to propagate the changes to "fill_current_preset_parameters()"
  // and "recreate_parameters_file()"
  _presets_list_.emplace_back("default");
  _data_handler_[_presets_list_.back() + "-install-mods-base-folder"] = "/atmosphere/";

  _presets_list_.emplace_back("reinx");
  _data_handler_[_presets_list_.back() + "-install-mods-base-folder"] = "/reinx/";

  _presets_list_.emplace_back("sxos");
  _data_handler_[_presets_list_.back() + "-install-mods-base-folder"] = "/sxos/";

  _presets_list_.emplace_back("root");
  _data_handler_[_presets_list_.back() + "-install-mods-base-folder"] = "/";

}
void ParametersHandler::recreate_parameters_file() {

  GenericToolbox::mkdirPath(GenericToolbox::getFolderPathFromFilePath(_parameters_file_path_));

  std::stringstream parameter_file;
  parameter_file << "# This is a config file" << std::endl;
  parameter_file << std::endl;
  parameter_file << "# folder where mods are stored" << std::endl;
  parameter_file << "stored-mods-base-folder = " << _data_handler_["stored-mods-base-folder"] << std::endl;
  parameter_file << "use-gui = " << _data_handler_["use-gui"] << std::endl;
  parameter_file << "last-preset-used = " << _data_handler_["last-preset-used"] << std::endl;
  parameter_file << std::endl;
  parameter_file << std::endl;
  for(auto const &preset : _presets_list_){
    parameter_file << "########################################" << std::endl;
    parameter_file << "# preset that can be changed in the app" << std::endl;
    parameter_file << "preset = " << preset << std::endl;
    parameter_file << std::endl;
    parameter_file << "# base folder where mods are installed" << std::endl;
    parameter_file << "install-mods-base-folder = " << _data_handler_[preset + "-install-mods-base-folder"] << std::endl;
    parameter_file << "########################################" << std::endl;
    parameter_file << std::endl;
    parameter_file << std::endl;
  }
  parameter_file << "# DO NOT TOUCH THIS : used to recognise the last version of the program config" << std::endl;
  parameter_file << "last-program-version = " << Toolbox::get_app_version() << std::endl;
  parameter_file << std::endl;

  std::string data = parameter_file.str();
  GenericToolbox::dumpStringInFile(_parameters_file_path_, data);

}
void ParametersHandler::read_parameters() {

  auto lines = GenericToolbox::dumpFileAsVectorString(_parameters_file_path_);

  // specifying first preset to keep compatibility with older versions
  // where presets were not implemented yet (1.2.0)
  std::string current_preset;

  for(auto &line : lines){

    if(line[0] == '#') continue;

    auto line_elements = GenericToolbox::splitString(line, "=");
    if(line_elements.size() != 2) continue;

    // clean up for extra spaces characters
    for(auto &element : line_elements){
      while(element[0] == ' '){
        element.erase(element.begin());
      }
      while(element[element.size()-1] == ' '){
        element.erase(element.end()-1);
      }
    }

    //
    if(line_elements[0] == "preset"){
      current_preset = line_elements[1];
      if(not GenericToolbox::doesElementIsInVector(line_elements[1], _presets_list_) ){
        _presets_list_.emplace_back(current_preset);
      }
    } else {
      if(current_preset.empty()){
        _data_handler_[line_elements[0]] = line_elements[1];
      }
      else{
        _data_handler_[current_preset + "-" + line_elements[0]] = line_elements[1];
      }
    }

  }

  // getting last-program-version:
  _data_handler_["last-program-version"] = _data_handler_[_presets_list_.back() + "-last-program-version"];

}

void ParametersHandler::fill_current_preset_parameters(){

  _data_handler_["last-preset-used"] = _presets_list_[_current_config_preset_id_];
  _data_handler_["install-mods-base-folder"] = _data_handler_[_presets_list_[_current_config_preset_id_] + "-install-mods-base-folder"];

}

