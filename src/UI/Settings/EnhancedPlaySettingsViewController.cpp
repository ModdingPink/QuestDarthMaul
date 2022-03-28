#include "UI/Settings/EnhancedPlaySettingsViewController.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "config.hpp"
#include "logging.hpp"
#include "ScoreSubmission.hpp"


DEFINE_TYPE(EnhancedPlay::UI, EnhancedPlaySettingsViewController);

using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace HMUI;
using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;

// simple toggle macro because typing the same thing every time is dumb
#define TOGGLE(name, displayName) \
	CreateToggle(container->get_transform(), displayName, config.name, [](bool value) -> void { \
		config.name = value; \
		SaveConfig(); \
	});

namespace EnhancedPlay::UI
{
	void EnhancedPlaySettingsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
	{
		if (firstActivation)
		{
			GameObject* container = CreateScrollableSettingsContainer(get_transform());
			
            /*
            std::vector<StringW> playStyleStringWVector = { 
                "Default",
                "Darth Maul One Hand",
                "Darth Maul Two Hands",
                "Unicorn Mode",
                "Swap Controllers",
                "One Saber"
                //"Any Colour"
            };
            StringW playStyleStringW = playStyleStringWVector[config.playStyleMode];
            
            QuestUI::BeatSaberUI::CreateDropdown(container->get_transform(), "Play Style", playStyleStringW, playStyleStringWVector,
                [playStyleStringWVector](std::string value) {
                    if (value == playStyleStringWVector[0]) {
                        config.playStyleMode = 0;
                        enableScoreSubmission();
                    }
                    else if (value == playStyleStringWVector[1]) {
                        config.playStyleMode = 1;
                        enableScoreSubmission();
                    }
                    else if (value == playStyleStringWVector[2]) {
                        config.playStyleMode = 2;
                        enableScoreSubmission();
                    }
                    else if (value == playStyleStringWVector[3]) {
                        config.playStyleMode = 3;
                        disableScoreSubmission();
                    }
                    //else{
                    else if (value == playStyleStringWVector[4]) {
                        config.playStyleMode = 4;
                        enableScoreSubmission();
                    }else{
                    //else if (value == playStyleStringWVector[4]) {
                        config.playStyleMode = 5;
                        disableScoreSubmission();
                    }
                    //else {
                    //    config.playStyleMode = 5;
                    //}
                    SaveConfig();
                }
            );
            */
            std::vector<StringW> mainHandStringWVector = {
                "Right",
                "Left"
            };
            StringW mainHandStringW = mainHandStringWVector[config.mainHand];

            QuestUI::BeatSaberUI::CreateDropdown(container->get_transform(), "Dominant Hand", mainHandStringW, mainHandStringWVector,
                [mainHandStringWVector](std::string value) {
                    if (value == mainHandStringWVector[0]) {
                        config.mainHand = 0;
                    }
                    else{
                        config.mainHand = 1;
                    }
                    SaveConfig();
                }
            );

			TOGGLE(swapControllers, "Swap Controllers");

			TOGGLE(disableRumble, "Disable Rumble");
		}
	}
}