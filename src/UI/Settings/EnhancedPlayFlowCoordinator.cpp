#include "UI/Settings/EnhancedPlayFlowCoordinator.hpp"
#include "UI/Settings/EnhancedPlaySettingsViewController.hpp"

#include "questui/shared/BeatSaberUI.hpp"
//#include "Utils/UIUtils.hpp"

#include "HMUI/TitleViewController.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/ViewController_AnimationType.hpp"

DEFINE_TYPE(EnhancedPlay::UI, EnhancedPlayFlowCoordinator);

using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace HMUI;
using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;

namespace EnhancedPlay::UI
{
	void EnhancedPlayFlowCoordinator::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
	{
		if (firstActivation)
		{
			enhancedPlaySettingsViewController = reinterpret_cast<HMUI::ViewController*>(CreateViewController<EnhancedPlaySettingsViewController*>());
			
			SetTitle(il2cpp_utils::newcsstr("Enhanced Play Settings"), ViewController::AnimationType::Out);
			showBackButton = true;

			ProvideInitialViewControllers(enhancedPlaySettingsViewController, nullptr, nullptr, nullptr, nullptr);
		}
	}

	void EnhancedPlayFlowCoordinator::BackButtonWasPressed(HMUI::ViewController* topViewController)
	{
		parentFlowCoordinator->DismissFlowCoordinator(this, ViewController::AnimationDirection::Horizontal, nullptr, false);
	}
}