#include <dlfcn.h>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp" 
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "beatsaber-hook/shared/config/config-utils.hpp"


#define PATH "/sdcard/Android/data/com.beatgames.beatsaber/files/logdump-"
#define EXT ".txt"

static ModInfo modInfo;

std::string languageText = "Darth Maul";
bool flip = false;
bool swapHands = false;
bool alwaysDarthMaul = false;

static Configuration& getConfig() {
    static Configuration config(modInfo);
    return config;
}

static const Logger& getLogger() {
    static const Logger logger(modInfo);
    return logger;
}

void write_info(FILE* fp, std::string str) {
    getLogger().debug("%s", str.data());
    fwrite((str + "\n").data(), str.length() + 1, 1, fp);
}



bool noArrows = false;
bool leftHand = false;

Il2CppObject* leftSaberTransform = nullptr;
Il2CppObject* rightSaberTransform = nullptr;
bool hookedQosmetics = false;
MAKE_HOOK_OFFSETLESS(QosmeticsTrail_Update, void, Il2CppObject *self) {

	if (noArrows && leftSaberTransform != nullptr && rightSaberTransform != nullptr) {
		Il2CppClass* transformClass = il2cpp_utils::GetClassFromName("UnityEngine", "Transform");
        const MethodInfo* getMethod = il2cpp_functions::class_get_method_from_name(transformClass, "get_localPosition", 0);
        const MethodInfo* setMethod = il2cpp_functions::class_get_method_from_name(transformClass, "set_localPosition", 1);
        const MethodInfo* setRotate = il2cpp_functions::class_get_method_from_name(transformClass, "Rotate", 1);
        const MethodInfo* setTranslate = il2cpp_functions::class_get_method_from_name(transformClass, "Translate", 1);
        const MethodInfo* getLocalRotation = il2cpp_functions::class_get_method_from_name(transformClass, "get_localRotation", 0);
        const MethodInfo* setLocalRotation = il2cpp_functions::class_get_method_from_name(transformClass, "set_localRotation", 1);

        Vector3 rightSaberLocalPosition = CRASH_UNLESS(il2cpp_utils::RunMethod<Vector3>(rightSaberTransform, getMethod));
        Vector3 leftSaberLocalPosition = CRASH_UNLESS(il2cpp_utils::RunMethod<Vector3>(leftSaberTransform, getMethod));
        
        Quaternion rightSaberLocalRotation = CRASH_UNLESS(il2cpp_utils::RunMethod<Quaternion>(rightSaberTransform, getLocalRotation));
        Quaternion leftSaberLocalRotation = CRASH_UNLESS(il2cpp_utils::RunMethod<Quaternion>(leftSaberTransform, getLocalRotation));

        if (!leftHand) {
            if (flip) {
                CRASH_UNLESS(il2cpp_utils::RunMethod(leftSaberTransform, setMethod, rightSaberLocalPosition));
                CRASH_UNLESS(il2cpp_utils::RunMethod(leftSaberTransform, setLocalRotation, rightSaberLocalRotation));
                CRASH_UNLESS(il2cpp_utils::RunMethod(rightSaberTransform, setRotate, Vector3{ 0, 180, 0 }));

                CRASH_UNLESS(il2cpp_utils::RunMethod(leftSaberTransform, setTranslate, Vector3{ 0, 0, 0.075 }));
                CRASH_UNLESS(il2cpp_utils::RunMethod(rightSaberTransform, setTranslate, Vector3{ 0, 0, 0.26 }));
            }
            else {
                CRASH_UNLESS(il2cpp_utils::RunMethod(leftSaberTransform, setMethod, rightSaberLocalPosition));
                CRASH_UNLESS(il2cpp_utils::RunMethod(leftSaberTransform, setLocalRotation, rightSaberLocalRotation));
                CRASH_UNLESS(il2cpp_utils::RunMethod(leftSaberTransform, setRotate, Vector3{ 0, 180, 0 }));

                CRASH_UNLESS(il2cpp_utils::RunMethod(rightSaberTransform, setTranslate, Vector3{ 0, 0, 0.075 }));
                CRASH_UNLESS(il2cpp_utils::RunMethod(leftSaberTransform, setTranslate, Vector3{ 0, 0, 0.26 }));
            }
            
        }
        else {

            if (flip) {
                CRASH_UNLESS(il2cpp_utils::RunMethod(rightSaberTransform, setMethod, leftSaberLocalPosition));
                CRASH_UNLESS(il2cpp_utils::RunMethod(rightSaberTransform, setLocalRotation, leftSaberLocalRotation));
                CRASH_UNLESS(il2cpp_utils::RunMethod(leftSaberTransform, setRotate, Vector3{ 0, 180, 0 }));

                CRASH_UNLESS(il2cpp_utils::RunMethod(rightSaberTransform, setTranslate, Vector3{ 0, 0, 0.075 }));
                CRASH_UNLESS(il2cpp_utils::RunMethod(leftSaberTransform, setTranslate, Vector3{ 0, 0, 0.26 }));
            }
            else {
                CRASH_UNLESS(il2cpp_utils::RunMethod(rightSaberTransform, setMethod, leftSaberLocalPosition));
                CRASH_UNLESS(il2cpp_utils::RunMethod(rightSaberTransform, setLocalRotation, leftSaberLocalRotation));
                CRASH_UNLESS(il2cpp_utils::RunMethod(rightSaberTransform, setRotate, Vector3{ 0, 180, 0 }));

                CRASH_UNLESS(il2cpp_utils::RunMethod(leftSaberTransform, setTranslate, Vector3{ 0, 0, 0.075 }));
                CRASH_UNLESS(il2cpp_utils::RunMethod(rightSaberTransform, setTranslate, Vector3{ 0, 0, 0.26 }));
            }
        }
	}
	
	QosmeticsTrail_Update(self);
}

MAKE_HOOK_OFFSETLESS(StandardLevelScenesTransitionSetupDataSO, void, Il2CppObject* self, Il2CppString* gameMode, Il2CppObject* difficultyBeatmap, Il2CppObject* overrideEnvironmentSettings, Il2CppObject* overrideColorScheme, Il2CppObject* gameplayModifiers, Il2CppObject* playerSpecificSettings, Il2CppObject* practiceSettings, Il2CppObject* backButtonText, bool useTestNoteCutSoundEffects) {
	if (!alwaysDarthMaul) {
		noArrows = CRASH_UNLESS(il2cpp_utils::GetPropertyValue<bool>(gameplayModifiers, "noArrows"));
	}
	else {
		noArrows = true;
	}
	leftHand = CRASH_UNLESS(il2cpp_utils::GetPropertyValue<bool>(playerSpecificSettings, "leftHanded"));
	if (swapHands) leftHand = !leftHand;
	StandardLevelScenesTransitionSetupDataSO(self, gameMode, difficultyBeatmap, overrideEnvironmentSettings, overrideColorScheme, gameplayModifiers, playerSpecificSettings, practiceSettings, backButtonText, useTestNoteCutSoundEffects);

}

MAKE_HOOK_OFFSETLESS(MultiplayerLevelScenesTransitionSetupDataSO_Init, void, Il2CppObject* self, Il2CppString* gameMode, Il2CppObject*previewBeatmapLevel, Il2CppObject* beatmapDifficulty, Il2CppObject* beatmapCharacteristic, Il2CppObject* difficultyBeatmap, Il2CppObject* overrideColorScheme, Il2CppObject* gameplayModifiers, Il2CppObject* playerSpecificSettings, Il2CppObject* practiceSettings, bool useTestNoteCutSoundEffects) {
	if (!alwaysDarthMaul) {
		noArrows = CRASH_UNLESS(il2cpp_utils::GetPropertyValue<bool>(gameplayModifiers, "noArrows"));
	}
	else {
		noArrows = true;
	}
	leftHand = CRASH_UNLESS(il2cpp_utils::GetPropertyValue<bool>(playerSpecificSettings, "leftHanded"));
	if (swapHands) leftHand = !leftHand;
	MultiplayerLevelScenesTransitionSetupDataSO_Init(self, gameMode, previewBeatmapLevel, beatmapDifficulty, beatmapCharacteristic, difficultyBeatmap, overrideColorScheme, gameplayModifiers, playerSpecificSettings, practiceSettings, useTestNoteCutSoundEffects);
}

MAKE_HOOK_OFFSETLESS(GameplayModifierToggle_Start, void, Il2CppObject* self) {
	GameplayModifierToggle_Start(self);
	if (alwaysDarthMaul) {
		return;
	}
	Il2CppObject* gameplayModifier = CRASH_UNLESS(il2cpp_utils::GetFieldValue(self, "_gameplayModifier"));
	Il2CppString* gameplayModifierNameLocalizationKey = CRASH_UNLESS(il2cpp_utils::GetPropertyValue< Il2CppString*>(gameplayModifier, "modifierNameLocalizationKey"));
	std::string gameplayModifierNameLocalizationKeyString = to_utf8(csstrtostr(gameplayModifierNameLocalizationKey));
	if (gameplayModifierNameLocalizationKeyString == "MODIFIER_NO_ARROWS") {
		std::string modifName = languageText + "\n<color=#ffffff88><size=80%>-0.30</size></color>";
		Il2CppObject* nameText = CRASH_UNLESS(il2cpp_utils::GetFieldValue(self, "_nameText"));
		Il2CppString *changeStr = il2cpp_utils::createcsstr(modifName.c_str());
		CRASH_UNLESS(il2cpp_utils::RunMethod(nameText, "SetText", changeStr));
	}
}

MAKE_HOOK_OFFSETLESS(SaberManager_Update, void, Il2CppObject* self) {
	

	if (noArrows) {
        
        if(il2cpp_utils::FindMethodUnsafe("Qosmetics", "QosmeticsTrail", "Update", 0) && !hookedQosmetics) {
            log("Installing Qosmetics hook");
            INSTALL_HOOK_OFFSETLESS(QosmeticsTrail_Update, il2cpp_utils::FindMethodUnsafe("Qosmetics", "QosmeticsTrail", "Update", 0));
            hookedQosmetics = true;
        }

		Il2CppObject* leftSaber = CRASH_UNLESS(il2cpp_utils::GetFieldValue<Il2CppObject*>(self, "_leftSaber"));
		Il2CppObject* rightSaber = CRASH_UNLESS(il2cpp_utils::GetFieldValue<Il2CppObject*>(self, "_rightSaber"));

		if (leftSaber != nullptr && rightSaber != nullptr) {

			Il2CppClass* componentsClass = il2cpp_utils::GetClassFromName("", "Saber");
			leftSaberTransform = CRASH_UNLESS(il2cpp_utils::RunMethod(leftSaber, il2cpp_functions::class_get_method_from_name(componentsClass, "get_transform", 0)));
			rightSaberTransform = CRASH_UNLESS(il2cpp_utils::RunMethod(rightSaber, il2cpp_functions::class_get_method_from_name(componentsClass, "get_transform", 0)));

			Il2CppClass* transformClass = il2cpp_utils::GetClassFromName("UnityEngine", "Transform");

			if (leftSaberTransform != nullptr && rightSaberTransform != nullptr && !hookedQosmetics) {
				const MethodInfo* getMethod = il2cpp_functions::class_get_method_from_name(transformClass, "get_localPosition", 0);
				const MethodInfo* setMethod = il2cpp_functions::class_get_method_from_name(transformClass, "set_localPosition", 1);
				const MethodInfo* setRotate = il2cpp_functions::class_get_method_from_name(transformClass, "Rotate", 1);
				const MethodInfo* setTranslate = il2cpp_functions::class_get_method_from_name(transformClass, "Translate", 1);
				const MethodInfo* getLocalRotation = il2cpp_functions::class_get_method_from_name(transformClass, "get_localRotation", 0);
				const MethodInfo* setLocalRotation = il2cpp_functions::class_get_method_from_name(transformClass, "set_localRotation", 1);

				Vector3 rightSaberLocalPosition = CRASH_UNLESS(il2cpp_utils::RunMethod<Vector3>(rightSaberTransform, getMethod));
				Vector3 leftSaberLocalPosition = CRASH_UNLESS(il2cpp_utils::RunMethod<Vector3>(leftSaberTransform, getMethod));
				
				Quaternion rightSaberLocalRotation = CRASH_UNLESS(il2cpp_utils::RunMethod<Quaternion>(rightSaberTransform, getLocalRotation));
				Quaternion leftSaberLocalRotation = CRASH_UNLESS(il2cpp_utils::RunMethod<Quaternion>(leftSaberTransform, getLocalRotation));

				if (!leftHand) {
					if (flip) {
						CRASH_UNLESS(il2cpp_utils::RunMethod(leftSaberTransform, setMethod, rightSaberLocalPosition));
						CRASH_UNLESS(il2cpp_utils::RunMethod(leftSaberTransform, setLocalRotation, rightSaberLocalRotation));
						CRASH_UNLESS(il2cpp_utils::RunMethod(rightSaberTransform, setRotate, Vector3{ 0, 180, 0 }));

						CRASH_UNLESS(il2cpp_utils::RunMethod(leftSaberTransform, setTranslate, Vector3{ 0, 0, 0.075 }));
						CRASH_UNLESS(il2cpp_utils::RunMethod(rightSaberTransform, setTranslate, Vector3{ 0, 0, 0.26 }));
					}
					else {
						CRASH_UNLESS(il2cpp_utils::RunMethod(leftSaberTransform, setMethod, rightSaberLocalPosition));
						CRASH_UNLESS(il2cpp_utils::RunMethod(leftSaberTransform, setLocalRotation, rightSaberLocalRotation));
						CRASH_UNLESS(il2cpp_utils::RunMethod(leftSaberTransform, setRotate, Vector3{ 0, 180, 0 }));

						CRASH_UNLESS(il2cpp_utils::RunMethod(rightSaberTransform, setTranslate, Vector3{ 0, 0, 0.075 }));
						CRASH_UNLESS(il2cpp_utils::RunMethod(leftSaberTransform, setTranslate, Vector3{ 0, 0, 0.26 }));
					}
					
				}
				else {

					if (flip) {
						CRASH_UNLESS(il2cpp_utils::RunMethod(rightSaberTransform, setMethod, leftSaberLocalPosition));
						CRASH_UNLESS(il2cpp_utils::RunMethod(rightSaberTransform, setLocalRotation, leftSaberLocalRotation));
						CRASH_UNLESS(il2cpp_utils::RunMethod(leftSaberTransform, setRotate, Vector3{ 0, 180, 0 }));

						CRASH_UNLESS(il2cpp_utils::RunMethod(rightSaberTransform, setTranslate, Vector3{ 0, 0, 0.075 }));
						CRASH_UNLESS(il2cpp_utils::RunMethod(leftSaberTransform, setTranslate, Vector3{ 0, 0, 0.26 }));
					}
					else {
						CRASH_UNLESS(il2cpp_utils::RunMethod(rightSaberTransform, setMethod, leftSaberLocalPosition));
						CRASH_UNLESS(il2cpp_utils::RunMethod(rightSaberTransform, setLocalRotation, leftSaberLocalRotation));
						CRASH_UNLESS(il2cpp_utils::RunMethod(rightSaberTransform, setRotate, Vector3{ 0, 180, 0 }));

						CRASH_UNLESS(il2cpp_utils::RunMethod(leftSaberTransform, setTranslate, Vector3{ 0, 0, 0.075 }));
						CRASH_UNLESS(il2cpp_utils::RunMethod(rightSaberTransform, setTranslate, Vector3{ 0, 0, 0.26 }));
					}
				}
			}

		}

	}
	SaberManager_Update(self);

}



MAKE_HOOK_OFFSETLESS(SaberTypeExtensions_Node, int, Il2CppObject* saberType) {

	if (noArrows) {
		if (!leftHand) {
			return 5;
		}
		else {
			return 4;
		}
	}
	return SaberTypeExtensions_Node(saberType);
	

}


MAKE_HOOK_OFFSETLESS(HealthWarningSceneStart_Start, void, Il2CppObject *self) {

	if (il2cpp_utils::FindMethodUnsafe("Qosmetics", "QosmeticsTrail", "Update", 0) && !hookedQosmetics) {
		INSTALL_HOOK_OFFSETLESS(QosmeticsTrail_Update, il2cpp_utils::FindMethodUnsafe("Qosmetics", "QosmeticsTrail", "Update", 0));
		hookedQosmetics = true;
	}
	HealthWarningSceneStart_Start(self);
}

void resetconfig() {

	getConfig().config.RemoveAllMembers();
	getConfig().config.SetObject();
	auto& allocator = getConfig().config.GetAllocator();

	getConfig().config.AddMember("languageText", languageText, allocator);
	getConfig().config.AddMember("flipTopSaberColour", flip, allocator);
	getConfig().config.AddMember("swapHands", swapHands, allocator);
	getConfig().config.AddMember("alwaysDarthMaul", alwaysDarthMaul, allocator);

	getConfig().Write();
}


extern "C" void setup(ModInfo& info) {
    info.id = "DarthMaul";
    info.version = "1.0.0";
	modInfo = info;


    getLogger().info("Completed setup!");
    getLogger().info("Modloader name: %s", Modloader::getInfo().name.c_str());
    getConfig().Load();
	const auto& conf = getConfig().config;

	bool resetConfig = false;

	auto languageTextitr = conf.FindMember("languageText");
	if (languageTextitr != conf.MemberEnd()) {
		languageText = languageTextitr->value.GetString();
	}else { resetConfig = true; }
	auto flipitr = conf.FindMember("flipTopSaberColour");
	if (flipitr != conf.MemberEnd()) {
		flip = flipitr->value.GetBool();
	}else { resetConfig = true; }
	auto swapHandsitr = conf.FindMember("swapHands");
	if (swapHandsitr != conf.MemberEnd()) {
		swapHands = swapHandsitr->value.GetBool();
	}else { resetConfig = true; }
	auto alwaysDarthMaulitr = conf.FindMember("alwaysDarthMaul");
	if (alwaysDarthMaulitr != conf.MemberEnd()) {
		alwaysDarthMaul = alwaysDarthMaulitr->value.GetBool();
	}else { resetConfig = true; }

	if (resetConfig) {
		resetconfig();
	}

}

extern "C" void load() {
    getLogger().debug("Installing DarthMaul!");
		

	INSTALL_HOOK_OFFSETLESS(SaberTypeExtensions_Node, il2cpp_utils::FindMethodUnsafe("", "SaberTypeExtensions", "Node", 1)); 
	INSTALL_HOOK_OFFSETLESS(HealthWarningSceneStart_Start, il2cpp_utils::FindMethodUnsafe("", "HealthWarningSceneStart", "Start", 0));

	INSTALL_HOOK_OFFSETLESS(SaberManager_Update, il2cpp_utils::FindMethodUnsafe("", "SaberManager", "Update", 0));
	INSTALL_HOOK_OFFSETLESS(StandardLevelScenesTransitionSetupDataSO, il2cpp_utils::FindMethodUnsafe("", "StandardLevelScenesTransitionSetupDataSO", "Init", 9));
	INSTALL_HOOK_OFFSETLESS(MultiplayerLevelScenesTransitionSetupDataSO_Init, il2cpp_utils::FindMethodUnsafe("", "MultiplayerLevelScenesTransitionSetupDataSO", "Init", 10));


	INSTALL_HOOK_OFFSETLESS(GameplayModifierToggle_Start, il2cpp_utils::FindMethodUnsafe("", "GameplayModifierToggle", "Start", 0));

	getLogger().debug("Installed DarthMaul!");
    getLogger().info("initialized: %s", il2cpp_functions::initialized ? "true" : "false");
}
