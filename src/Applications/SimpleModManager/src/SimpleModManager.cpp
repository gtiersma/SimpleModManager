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
#include <StateAlchemist/controller.h>
#include "StateAlchemist/constants.h"
#include "StateAlchemist/fs_manager.h"


int main(int argc, char* argv[])
{
    brls::Application::init();
    brls::Application::createWindow("Simple Mod Alchemist");
    brls::Application::getPlatform()->setThemeVariant(brls::ThemeVariant::DARK);

    // Have the application register an action on every activity that will quit when you press BUTTON_START
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
    brls::Application::registerXMLView("TabGames", TabGames::create);
    brls::Application::registerXMLView("TabGeneralSettings", TabGeneralSettings::create);
    brls::Application::registerXMLView("TabAbout", TabAbout::create);

    // Create the app's folder in the SD Root if not yet created:
    FsManager::createFolderIfNeeded(ALCHEMIST_PATH);

    gameBrowser.init();

    brls::Activity* mainActivity = new FrameRoot();

    brls::AppletFrame* appFrame = (brls::AppletFrame*)mainActivity->getContentView();
    appFrame->setTitle("Simple Mod Alchemist (v" + APP_VERSION + ")");

    brls::Application::pushActivity(new FrameRoot());

    // Run the app
    while (brls::Application::mainLoop())
        ;

    nsExit();

    // Exit
    return EXIT_SUCCESS;
}

#ifdef __WINRT__
#include <borealis/core/main.hpp>
#endif
