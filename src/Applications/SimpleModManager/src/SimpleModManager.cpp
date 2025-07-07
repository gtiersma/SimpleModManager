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
#include <AlchemistLogger.h>
#include <StateAlchemist/meta_manager.h>


int main(int argc, char* argv[]){
  alchemyLogger.log("launching...");

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


void runGui() {
  MetaManager::tryResult(nsInitialize());
  alchemyLogger.log("runGui();");

  brls::i18n::loadTranslations("en-US");
  MetaManager::tryResult(brls::Application::init("SimpleModManager"));

  auto* mainFrame = new FrameRoot();

  alchemyLogger.log("runGui: about to push the view");
  brls::Application::pushView( mainFrame );
  alchemyLogger.log("runGui: the view was pushed");
  mainFrame->registerAction( "", brls::Key::PLUS, []{return true;}, true );
  mainFrame->updateActionHint( brls::Key::PLUS, "" ); // make the change visible

  while( brls::Application::mainLoop() ){  }

  nsExit();
}
