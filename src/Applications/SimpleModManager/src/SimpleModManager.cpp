//
// Created by Adrien Blanchet on 14/04/2023.
//


#include "SimpleModManager.h"

#include <FrameRoot.h>
#include <TabGames.h>
#include <TabGeneralSettings.h>
#include <TabAbout.h>

#include "ConfigHandler.h"

#include <borealis.hpp>

#include "switch.h"
#include <AlchemistLogger.h>
#include "StateAlchemist/constants.h"
#include "StateAlchemist/fs_manager.h"


int main(int argc, char* argv[])
{
    alchemyLogger.log("main:::init");
    brls::Application::init();
    alchemyLogger.log("main:::createWindow");
    brls::Application::createWindow("Simple Mod Alchemist");
    alchemyLogger.log("main:::setThemeVariant");
    brls::Application::getPlatform()->setThemeVariant(brls::ThemeVariant::DARK);

    // Have the application register an action on every activity that will quit when you press BUTTON_START
    alchemyLogger.log("main:::setGlobalQuit");
    brls::Application::setGlobalQuit(false);

    // Register custom views (including tabs, which are views)
    /*brls::Application::registerXMLView("CaptionedImage", CaptionedImage::create);
    brls::Application::registerXMLView("RecyclingListTab", RecyclingListTab::create);
    brls::Application::registerXMLView("ComponentsTab", ComponentsTab::create);
    brls::Application::registerXMLView("TransformTab", TransformTab::create);
    brls::Application::registerXMLView("TransformBox", TransformBox::create);
    brls::Application::registerXMLView("PokemonView", PokemonView::create);
    brls::Application::registerXMLView("SettingsTab", SettingsTab::create);
    brls::Application::registerXMLView("TextTestTab", TextTestTab::create);*/
    alchemyLogger.log("main:::registerXMLView");
    brls::Application::registerXMLView("TabGames", TabGames::create);
    brls::Application::registerXMLView("TabGeneralSettings", TabGeneralSettings::create);
    brls::Application::registerXMLView("TabAbout", TabAbout::create);

    // Create the app's folder in the SD Root if not yet created:
    FsManager::createFolderIfNeeded(ALCHEMIST_PATH);

    gameBrowser.init();

    alchemyLogger.log("main:::new FrameRoot()");
    brls::Activity* mainActivity = new FrameRoot();

    alchemyLogger.log("main:::getContentView");
    brls::AppletFrame* appFrame = (brls::AppletFrame*)mainActivity->getContentView();
    alchemyLogger.log("main:::setTitle");
    appFrame->setTitle("Simple Mod Alchemist (v" + APP_VERSION + ")");

    alchemyLogger.log("main:::setTitle");
    brls::Application::pushActivity(mainActivity);

    // Run the app
    alchemyLogger.log("main:::mainLoop");
    while (brls::Application::mainLoop())
        ;

    nsExit();

    // Exit
    return EXIT_SUCCESS;
}

#ifdef __WINRT__
#include <borealis/core/main.hpp>
#endif
