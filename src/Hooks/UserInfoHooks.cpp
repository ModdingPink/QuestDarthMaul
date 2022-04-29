#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "hooks.hpp"
#include "config.hpp"
#include "levelInfo.hpp"

#include "GlobalNamespace/StandardLevelGameplayManager.hpp"
#include "GlobalNamespace/StandardLevelScenesTransitionSetupDataSO.hpp"
#include "GlobalNamespace/PlayerSpecificSettings.hpp"
#include "GlobalNamespace/ColorScheme.hpp"
#include "GlobalNamespace/ColorManager.hpp"
#include "GlobalNamespace/SaberType.hpp"
#include "GlobalNamespace/NoteBasicCutInfoHelper.hpp"
#include "GlobalNamespace/NoteCutDirection.hpp"
#include "GlobalNamespace/ColorType.hpp"

#include "System/Action.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Color.hpp"

bool currentlyInLevel = false;
bool leftHanded = false;

MAKE_AUTO_HOOK_MATCH(StandardLevelGameplayManager_Update, &GlobalNamespace::StandardLevelGameplayManager::Update, void, GlobalNamespace::StandardLevelGameplayManager* self)
{
	currentlyInLevel = self->dyn__gameState() == 1;
	StandardLevelGameplayManager_Update(self);
}

MAKE_AUTO_HOOK_MATCH(StandardLevelScenesTransitionSetupDataSO_Init, &GlobalNamespace::StandardLevelScenesTransitionSetupDataSO::Init, void, GlobalNamespace::StandardLevelScenesTransitionSetupDataSO* self, StringW gameMode, GlobalNamespace::IDifficultyBeatmap* difficultyBeatmap, GlobalNamespace::IPreviewBeatmapLevel* previewBeatmapLevel, GlobalNamespace::OverrideEnvironmentSettings* overrideEnvironmentSettings, GlobalNamespace::ColorScheme* overrideColorScheme, GlobalNamespace::GameplayModifiers* gameplayModifiers, GlobalNamespace::PlayerSpecificSettings* playerSpecificSettings, GlobalNamespace::PracticeSettings* practiceSettings, StringW backButtonText, bool useTestNoteCutSoundEffects, bool startPaused)
{
    leftHanded = playerSpecificSettings->get_leftHanded();
	StandardLevelScenesTransitionSetupDataSO_Init(self, gameMode, difficultyBeatmap, previewBeatmapLevel, overrideEnvironmentSettings, overrideColorScheme, gameplayModifiers, playerSpecificSettings, practiceSettings, backButtonText, useTestNoteCutSoundEffects, startPaused);
}

MAKE_AUTO_HOOK_MATCH(ColorManager_ColorForSaberType, &GlobalNamespace::ColorManager::ColorForSaberType, UnityEngine::Color, GlobalNamespace::ColorManager* self, GlobalNamespace::SaberType type)
{
    if (config.oneColour) {
        return self->dyn__colorScheme()->get_saberBColor();
    }

    return ColorManager_ColorForSaberType(self, type);
}

MAKE_AUTO_HOOK_MATCH(NoteBasicCutInfoHelper_GetBasicCutInfo, &GlobalNamespace::NoteBasicCutInfoHelper::GetBasicCutInfo, void, UnityEngine::Transform* noteTransform, GlobalNamespace::ColorType colorType, GlobalNamespace::NoteCutDirection cutDirection, GlobalNamespace::SaberType saberType, float saberBladeSpeed, UnityEngine::Vector3 cutDirVec, float cutAngleTolerance, ByRef<bool> directionOK, ByRef<bool> speedOK, ByRef<bool> saberTypeOK, ByRef<float> cutDirDeviation, ByRef<float> cutDirAngle)
{
    NoteBasicCutInfoHelper_GetBasicCutInfo(noteTransform, colorType, cutDirection, saberType, saberBladeSpeed, cutDirVec, cutAngleTolerance, directionOK, speedOK, saberTypeOK, cutDirDeviation, cutDirAngle);
    if (config.oneColour) {
        saberTypeOK = true;
    }
}