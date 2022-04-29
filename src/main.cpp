#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "custom-types/shared/register.hpp"
#include "questui/shared/QuestUI.hpp"

#include "UI/Settings/EnhancedPlayFlowCoordinator.hpp"
//#include "CustomTypes/RequirementElement.hpp"

#include "HMUI/CurvedTextMeshPro.hpp"
#include "HMUI/ImageView.hpp"

#include "hooks.hpp"
#include "logging.hpp"
#include "static-defines.h"

#include "config.hpp"
#include "ScoreSubmission.hpp"

#include "bs-utils/shared/utils.hpp"


ModInfo modInfo;

void disableScoreSubmission() {
    bs_utils::Submission::disable(modInfo);
}

void enableScoreSubmission() {
    bs_utils::Submission::disable(modInfo);
}

extern "C" void setup(ModInfo& info)
{
	info.id = ID;
	info.version = VERSION;

	modInfo = info;
}

extern "C" void load()
{
	il2cpp_functions::Class_Init(classof(HMUI::ImageView*));
    il2cpp_functions::Class_Init(classof(HMUI::CurvedTextMeshPro*));
	Logger& logger = EnhancedPlay::Logging::getLogger();
	logger.info("Loading enhancedplay!");
	QuestUI::Init();
	
	 
	// if config load fails, save the config so it will work next time
	if (!LoadConfig())
		SaveConfig();
  
	Hooks::InstallHooks(logger);

	custom_types::Register::AutoRegister();

	QuestUI::Register::RegisterModSettingsFlowCoordinator<EnhancedPlay::UI::EnhancedPlayFlowCoordinator*>({"Enhanced Play", VERSION});
}