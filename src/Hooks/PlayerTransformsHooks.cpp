#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "hooks.hpp"
#include "config.hpp"
#include "levelInfo.hpp"

#include "GlobalNamespace/PlayerTransforms.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Quaternion.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Vector3.hpp"

MAKE_AUTO_HOOK_MATCH(PlayerTransforms_Update, &GlobalNamespace::PlayerTransforms::Update, void, GlobalNamespace::PlayerTransforms* self)
{
    auto leftHandTransform =  self->dyn__leftHandTransform();
    auto rightHandTransform =  self->dyn__rightHandTransform();
	if (leftHandTransform == nullptr || rightHandTransform == nullptr) {
		PlayerTransforms_Update(self);
		return;
	}

	if (config.darthMaulOneHand) {
		//OneHandDarthMaul
		UnityEngine::Transform* mainSaber;
		UnityEngine::Transform* saberToMove;

		if (config.mainHand == 0) {
			//r Right-handed
			mainSaber = rightHandTransform;
            saberToMove = leftHandTransform;
		} else {
			// Left-handed
			mainSaber = leftHandTransform;
            saberToMove = rightHandTransform;
		}

		UnityEngine::Quaternion mainSaberRot = mainSaber->get_rotation();
		UnityEngine::Vector3 mainSaberPos = mainSaber->get_position();
		saberToMove->set_rotation(mainSaberRot);
		saberToMove->Rotate({ 0,180,0 });
		saberToMove->set_position(mainSaberPos);
		saberToMove->Translate({ 0, 0, 0.26 });

		PlayerTransforms_Update(self);
		return;
	} else if (config.darthMaulBothHands) {
		// Two-Hand Darth Maul

		UnityEngine::Vector3 rightSaberPos = leftHandTransform->get_position();
        rightHandTransform->set_position({ ((leftHandTransform->get_position().x + rightHandTransform->get_position().x) / 2), ((leftHandTransform->get_position().y + rightHandTransform->get_position().y) / 2), ((leftHandTransform->get_position().z + rightHandTransform->get_position().z) / 2) });
        rightHandTransform->LookAt(rightSaberPos);
        rightHandTransform->Translate({ 0, 0, -0.13 });

        leftHandTransform->set_position(rightHandTransform->get_position());
        leftHandTransform->set_rotation(rightHandTransform->get_rotation());
        rightHandTransform->Rotate({ 0,180,0 });
        leftHandTransform->Translate({ 0, 0, 0.26 });

		PlayerTransforms_Update(self);
		return;
	} else if (config.unicornMode) {
		//UnicornMode

        rightHandTransform->set_rotation(self->dyn__headWorldRot());
        rightHandTransform->set_position(self->dyn__headWorldPos());
        leftHandTransform->set_position({0,-1000,0});
		PlayerTransforms_Update(self);
		return;
	} else if (config.oneSaber) {
		// OneSaber

		if (!leftHanded) {
			//normal mode
			if (!config.swapControllers) {
                // Right-handed
                leftHandTransform->set_position({0,-1000,0});
			} else {
                // Left-handed
                rightHandTransform->set_position(leftHandTransform->get_position());
                rightHandTransform->set_rotation(leftHandTransform->get_rotation());
                leftHandTransform->set_position({0,-1000,0});
			}
		} else {
			// Left handed mode
			if (!config.swapControllers) {
                // Right-handed
                rightHandTransform->set_position({0,-1000,0});
			} else {
                // Left-handed
                leftHandTransform->set_position(rightHandTransform->get_position());
                leftHandTransform->set_rotation(rightHandTransform->get_rotation());
                rightHandTransform->set_position({0,-1000,0});
			}
		}

		PlayerTransforms_Update(self);
		return;
	} else if (config.swapControllers) {
		//Swap Controllers
		UnityEngine::Quaternion leftSaberRot = leftHandTransform->get_rotation();
		UnityEngine::Vector3 leftSaberPos = leftHandTransform->get_position();

        leftHandTransform->set_position(rightHandTransform->get_position());
        leftHandTransform->set_rotation(rightHandTransform->get_rotation());

        rightHandTransform->set_position(leftSaberPos);
        rightHandTransform->set_rotation(leftSaberRot);

		PlayerTransforms_Update(self);
		return;
	}

	PlayerTransforms_Update(self);
}