#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "hooks.hpp"
#include "config.hpp"

#include "GlobalNamespace/StandardLevelGameplayManager.hpp"
#include "GlobalNamespace/HapticFeedbackController.hpp"
#include "GlobalNamespace/SaberClashEffect.hpp"

#include "System/Action.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Quaternion.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/XR/XRNode.hpp"
#include "Libraries/HM/HMLib/VR/HapticPresetSO.hpp"



bool currentlyInLevel = false;




MAKE_AUTO_HOOK_MATCH(SaberClashEffect_LateUpdate, &GlobalNamespace::SaberClashEffect::LateUpdate, void, GlobalNamespace::SaberClashEffect* self)
{
	if (config.playStyleMode == 5) {
		//OneHandDarthMaul
		return;
	}
	SaberClashEffect_LateUpdate(self);

}


MAKE_AUTO_HOOK_MATCH(StandardLevelGameplayManager_Update, &GlobalNamespace::StandardLevelGameplayManager::Update, void, GlobalNamespace::StandardLevelGameplayManager* self)
{
	currentlyInLevel = self->gameState == 1;
	StandardLevelGameplayManager_Update(self);

}



MAKE_AUTO_HOOK_MATCH(HapticFeedbackController_PlayHapticFeedback, &GlobalNamespace::HapticFeedbackController::PlayHapticFeedback, void, GlobalNamespace::HapticFeedbackController* self, ::UnityEngine::XR::XRNode node, Libraries::HM::HMLib::VR::HapticPresetSO* hapticPreset)
{
	if (!currentlyInLevel) {
		HapticFeedbackController_PlayHapticFeedback(self, node, hapticPreset);
		return;
	}

	if (config.disableRumble) {
		return;
	}else if (config.playStyleMode == 1) {
		//OneHandDarthMaul
		if (config.mainHand == 0) {
			//right handed
			if (node == 4) {
				//node is an enum, 4 being left hand 5 being right hand
				node = 5;
			}
		}
		else {
			//left handed
			if (node == 5) {
				node = 4;
			}
		}
		HapticFeedbackController_PlayHapticFeedback(self, node, hapticPreset);

		return;
	}
	else if (config.playStyleMode == 2) {
		//TwoHandDarthMaul
		HapticFeedbackController_PlayHapticFeedback(self, 4, hapticPreset);
		HapticFeedbackController_PlayHapticFeedback(self, 5, hapticPreset);
		//play in both hands
		return;
	}
	else if (config.playStyleMode == 3) {
		//UnicornMode
		//disable
		return;
	}
	else if (config.playStyleMode == 4) {
		//Swap Controllers
		if (node == 4) {
			HapticFeedbackController_PlayHapticFeedback(self, 5, hapticPreset);
		}else if (node == 5) {
			HapticFeedbackController_PlayHapticFeedback(self, 4, hapticPreset);
		}
		return;
	}else if (config.playStyleMode == 5) {
		//OneHandDarthMaul
		if (config.mainHand == 0) {
			//right handed
			if (node == 4) {
				//node is an enum, 4 being left hand 5 being right hand
				node = 5;
			}
		}
		else {
			//left handed
			if (node == 5) {
				node = 4;
			}
		}
		HapticFeedbackController_PlayHapticFeedback(self, node, hapticPreset);

		return;
	}
	HapticFeedbackController_PlayHapticFeedback(self, node, hapticPreset);

}

