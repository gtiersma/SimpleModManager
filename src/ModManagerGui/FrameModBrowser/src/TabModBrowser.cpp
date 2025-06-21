//
// Created by Adrien BLANCHET on 21/06/2020.
//

#include "TabModBrowser.h"

#include "FrameModBrowser.h"


#include "GenericToolbox.Macro.h"
#include "Logger.h"


#include <future>
#include <StateAlchemist/controller.h>
#include <AlchemistLogger.h>


LoggerInit([]{
  Logger::setUserHeaderStr("[TabModBrowser]");
});


TabModBrowser::TabModBrowser(FrameModBrowser* owner_) : _owner_(owner_) {
  alchemyLogger.log("TabModBrowser::TabModBrowser();");

  // Fetch the available mods
  auto modList = this->getModManager().getModList();

  if( modList.empty() ){
    LogInfo << "No mod found." << std::endl;

    _modItemList_.emplace_back();
    _modItemList_.back().item = new brls::ListItem(
        "No mods have been found in " + controller.getGamePath(),
        "There you need to put your mods such as: ./<group>/<thing-being-replaced>/<mod-name>/<file-structure-in-installed-directory>"
    );
    _modItemList_.back().item->show([](){}, false );
  } else {
    LogInfo << "Adding " << modList.size() << " mods..." << std::endl;

    std::string lastGroup;
    std::string lastSource;
    _modItemList_.reserve(modList.size());
    for( auto& mod : modList ) {
      LogScopeIndent;
      LogInfo << "Adding mod: \"" << mod.getLabel() << "\"" << std::endl;

      // If group & source are different, add the default option:
      if (mod.source != lastSource || mod.group != lastGroup) {
        ModEntry defaultEntry("");
        defaultEntry.source = mod.source;
        defaultEntry.group = mod.group;
        
        // memory allocation
        auto* item = new brls::ListItem(mod.getLabel() + " DEFAULT", "", "");

        // initialization
        item->getClickEvent()->subscribe([&, this, mod](View* view) {
          controller.group = mod.group;
          controller.source = mod.source;
          
          alchemyLogger.log("TAB MOD BROWSER: getting active mod of source: " + mod.source);
          ModEntry activeEntry(controller.getActiveMod(mod.source));
          activeEntry.source = mod.source;
          activeEntry.group = mod.group;
          
          _owner_->getGuiModManager().startRemoveModThread( activeEntry.getLabel() );
          this->updateDisplayedModsStatus();
          return true;
        });
        item->updateActionHint(brls::Key::A, "Apply");

        // create the holding struct
        _modItemList_.emplace_back();
        _modItemList_.back().modIndex = int(_modItemList_.size() ) - 1;
        _modItemList_.back().item = item;

        lastGroup = mod.group;
        lastSource = mod.source;
      }

      // memory allocation
      auto* item = new brls::ListItem(mod.getLabel(), "", "");

      // initialization
      item->getClickEvent()->subscribe([&, this, mod](View* view) {
        _owner_->getGuiModManager().startApplyModThread( mod.getLabel() );
        this->updateDisplayedModsStatus();
        return true;
      });
      item->updateActionHint(brls::Key::A, "Apply");

      item->registerAction("Disable", brls::Key::X, [&, this, mod]{
        _owner_->getGuiModManager().startRemoveModThread( mod.getLabel() );
        this->updateDisplayedModsStatus();
        return true;
      });

      // create the holding struct
      _modItemList_.emplace_back();
      _modItemList_.back().modIndex = int(_modItemList_.size() ) - 1;
      _modItemList_.back().item = item;
    }
  }

  this->updateDisplayedModsStatus();

  // add to view
  for( auto& modItem : _modItemList_ ){
    this->addView( modItem.item );
  }

}

void TabModBrowser::draw(NVGcontext *vg, int x, int y, unsigned int width, unsigned int height, brls::Style *style,
                         brls::FrameContext *ctx) {
  ScrollView::draw(vg, x, y, width, height, style, ctx);
}

void TabModBrowser::updateDisplayedModsStatus(){
  alchemyLogger.log("TabModBrowser::updateDisplayedModsStatus();");
  LogInfo << __METHOD_NAME__ << std::endl;

  auto& modEntryList = _owner_->getGameBrowser().getModManager().getModList();
  LogReturnIf( modEntryList.empty(), "No mod in this folder. Nothing to update." );

  // Build list that includes the default options:
  std::vector<ModEntry> modEntryListWithDefaults;
  std::string lastGroup;
  std::string lastSource;
  for (auto& modEntry : modEntryList) {
    if (modEntry.source != lastSource || modEntry.group != lastGroup) {
      ModEntry defaultEntry("");
      defaultEntry.source = modEntry.source;
      defaultEntry.group = modEntry.group;
      modEntryListWithDefaults.push_back(defaultEntry);
      lastGroup = modEntry.group;
      lastSource = modEntry.source;
    }
    modEntryListWithDefaults.push_back(modEntry);
  }

  // TEMP CODE
  // Ordered array of active mods - first match for first index must be the first active mod
  std::vector<std::string> activeMods;
  alchemyLogger.log("TAB MOD BROWSER: loading groups...");
  std::vector<std::string> groups = controller.loadGroups(true);
  for (auto& group : groups) {
    controller.group = group;
    alchemyLogger.log("TAB MOD BROWSER: getting sources of group: " + group);
    std::vector<std::string> sources = controller.loadSources(true);
    for (auto& source : sources) {
      alchemyLogger.log("TAB MOD BROWSER: getting active mod of source: " + source);
      activeMods.push_back(controller.getActiveMod(source));
    }
  }
  size_t activeCount = activeMods.size();
  LogInfo << "Active mods: " << activeCount << std::endl;

  size_t modCount = modEntryListWithDefaults.size();
  size_t activeI = 0;
  for (size_t iMod = 0; iMod < modCount; iMod++) {
    ModEntry& modEntry = modEntryListWithDefaults[iMod];

    NVGcolor color;
    if (activeI < activeCount && modEntry.mod == activeMods[activeI]) {
      _modItemList_[iMod].item->setValue("ACTIVE");
      // applied color (less saturated green)
      color = nvgRGB(88, 195, 169);
      activeI++;
    } else {
      _modItemList_[iMod].item->setValue("inactive", true);
      // inactive color
      color = GenericToolbox::Borealis::grayNvgColor;
    }
    _modItemList_[iMod].item->setValueActiveColor(color);
  }
}

const ModManager& TabModBrowser::getModManager() const{
  return _owner_->getGameBrowser().getModManager();
}
