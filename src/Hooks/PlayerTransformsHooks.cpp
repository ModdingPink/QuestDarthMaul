#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "hooks.hpp"
#include "config.hpp"
#include "levelInfo.hpp"

#include "GlobalNamespace/SaberManager.hpp"
#include "GlobalNamespace/PlayerTransforms.hpp"
#include "GlobalNamespace/Saber.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Quaternion.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Vector3.hpp"

MAKE_AUTO_HOOK_MATCH(PlayerTransforms_Update, &GlobalNamespace::PlayerTransforms::Update, void, GlobalNamespace::PlayerTransforms* self)
{
	if (self->leftHandTransform == nullptr || self->rightHandTransform == nullptr) {
		PlayerTransforms_Update(self);
		return;
	}

	if (config.darthMaulOneHand) {
		//OneHandDarthMaul
		UnityEngine::Transform* mainSaber;
		UnityEngine::Transform* saberToMove;

		if (config.mainHand == 0) {
			//right handed
			mainSaber = self->rightHandTransform;
			saberToMove = self->leftHandTransform;
		}
		else {
			//left handed
			mainSaber = self->leftHandTransform;
			saberToMove = self->rightHandTransform;
		}

		UnityEngine::Quaternion mainSaberRot = mainSaber->get_rotation();
		UnityEngine::Vector3 mainSaberPos = mainSaber->get_position();
		saberToMove->set_rotation(mainSaberRot);
		saberToMove->Rotate({ 0,180,0 });
		saberToMove->set_position(mainSaberPos);
		saberToMove->Translate({ 0, 0, 0.26 });

		PlayerTransforms_Update(self);
		return;
	}
	else if (config.darthMaulBothHands) {
		//TwoHandDarthMaul

		UnityEngine::Vector3 rightSaberPos = self->leftHandTransform->get_position();
		self->rightHandTransform->set_position({ ((self->leftHandTransform->get_position().x + self->rightHandTransform->get_position().x) / 2), ((self->leftHandTransform->get_position().y + self->rightHandTransform->get_position().y) / 2), ((self->leftHandTransform->get_position().z + self->rightHandTransform->get_position().z) / 2) });
		self->rightHandTransform->LookAt(rightSaberPos);
		self->rightHandTransform->Translate({ 0, 0, -0.13 });

		self->leftHandTransform->set_position(self->rightHandTransform->get_position());
		self->leftHandTransform->set_rotation(self->rightHandTransform->get_rotation());
		self->rightHandTransform->Rotate({ 0,180,0 });
		self->leftHandTransform->Translate({ 0, 0, 0.26 });

		PlayerTransforms_Update(self);
		return;
	}
	else if (config.unicornMode) {
		//UnicornMode

		self->rightHandTransform->set_rotation(self->headWorldRot);
		self->rightHandTransform->set_position(self->headWorldPos);
		self->leftHandTransform->set_position({0,-1000,0});
		PlayerTransforms_Update(self);
		return;
	}
	
	else if (config.oneSaber) {
		//OneSaber

		if(!leftHanded){
			//normal mode
			if (!config.swapControllers) {
				//right handed
				self->leftHandTransform->set_position({0,-1000,0});
			}
			else {
				//left handed
				self->rightHandTransform->set_position(self->leftHandTransform->get_position());
				self->rightHandTransform->set_rotation(self->leftHandTransform->get_rotation());
				self->leftHandTransform->set_position({0,-1000,0});

			}


		}else{
			//left handed mode
			if (!config.swapControllers) {
				//right handed
				self->rightHandTransform->set_position({0,-1000,0});
			}
			else {
				//left handed
				self->leftHandTransform->set_position(self->rightHandTransform->get_position());
				self->leftHandTransform->set_rotation(self->rightHandTransform->get_rotation());
				self->rightHandTransform->set_position({0,-1000,0});
			}
		}

	

		PlayerTransforms_Update(self);
		return;
	}
	else if (config.swapControllers) {
		//Swap Controllers
		UnityEngine::Quaternion leftSaberRot = self->leftHandTransform->get_rotation();
		UnityEngine::Vector3 leftSaberPos = self->leftHandTransform->get_position();

		self->leftHandTransform->set_position(self->rightHandTransform->get_position());
		self->leftHandTransform->set_rotation(self->rightHandTransform->get_rotation());

		self->rightHandTransform->set_position(leftSaberPos);
		self->rightHandTransform->set_rotation(leftSaberRot);



		PlayerTransforms_Update(self);
		return;
	}
	PlayerTransforms_Update(self);
}

