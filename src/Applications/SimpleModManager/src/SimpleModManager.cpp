//
// Created by Adrien Blanchet on 14/04/2023.
//


#include "SimpleModManager.h"

#include <FrameRoot.h>

#include "ConfigHandler.h"

#include <borealis.hpp>

#include <string>
#include <cstdlib>
#include "iostream"

#include "switch.h"
#include <StateAlchemist/controller.h>
#include "StateAlchemist/constants.h"
#include "StateAlchemist/fs_manager.h"


int main(int argc, char* argv[]) {

  // https://github.com/jbeder/yaml-cpp/wiki/Tutorial
//  YAML::Node config = YAML::LoadFile("config.yaml");
//  if (config["lastLogin"]) {
//    std::cout << "Last logged in: " << config["lastLogin"].as<std::string>() << "\n";
//  }
//  const auto username = config["username"].as<std::string>();
//  const auto password = config["password"].as<std::string>();

  runGui();

  // Exit
  return EXIT_SUCCESS;
}


void runGui(){
  brls::Application::init();
  brls::Application::createWindow("Simple Mod Alchemist");

  // Create the app's folder in the SD Root if not yet created:
  FsManager::createFolderIfNeeded(ALCHEMIST_PATH);

  FrameRoot* mainFrame = new FrameRoot();
  brls::Activity* mainActivity = new brls::Activity(mainFrame);
  brls::Application::pushActivity(mainActivity);

  while (brls::Application::mainLoop()) {};

  nsExit();
}
