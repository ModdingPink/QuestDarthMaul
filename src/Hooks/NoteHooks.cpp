#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "hooks.hpp"
#include "config.hpp"
#include "levelInfo.hpp"
#include "questui/shared/BeatSaberUI.hpp"

#include "GlobalNamespace/BeatmapObjectSpawnController.hpp"
#include "GlobalNamespace/NoteData.hpp"
#include "GlobalNamespace/SliderData.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "UnityEngine/RectOffset.hpp"

MAKE_AUTO_HOOK_MATCH(BeatmapObjectSpawnController_HandleNoteDataCallback, &GlobalNamespace::BeatmapObjectSpawnController::HandleNoteDataCallback, void, GlobalNamespace::BeatmapObjectSpawnController *self, GlobalNamespace::NoteData *noteData) {
    auto colorType = noteData->get_colorType();
    if (config.oneColour && !config.oneSaber) {
        noteData->set_colorType(1);
    } else if (config.oneSaber) {
        int toCheckFor = 0;
        if (leftHanded) toCheckFor = 1;
        if (colorType == toCheckFor) {
            return;
        }
    }

    auto gamePlayType = noteData->get_gameplayType();
    auto scoringType = noteData->get_scoringType();
    if (config.ignoreBurstSliders) {
        if (gamePlayType == 2) {
            noteData->set_gameplayType(0);
        }

        if (scoringType == 4 || scoringType == 5) {
            noteData->set_scoringType(1);
        }
    }

    if (config.ignoreArcSliders && (scoringType == 2 || scoringType == 3)) {
        noteData->set_scoringType(1);
    }

    auto noteLineLayer = noteData->get_noteLineLayer();
    if (config.swapTopAndBottomRow) {
        if (noteLineLayer == 0) {
            noteData->set_noteLineLayer(2);
        } else if (noteLineLayer == 2) {
            noteData->set_noteLineLayer(0);
        }
    }

    if (config.halfNotes && gamePlayType != 1) {
        noteData->ChangeToBurstSliderHead();
    }

    BeatmapObjectSpawnController_HandleNoteDataCallback(self, noteData);
}

MAKE_AUTO_HOOK_MATCH(BeatmapObjectSpawnController_HandleSliderDataCallback, &GlobalNamespace::BeatmapObjectSpawnController::HandleSliderDataCallback, void, GlobalNamespace::BeatmapObjectSpawnController *self, GlobalNamespace::SliderData *sliderNoteData) {
    auto colorType = sliderNoteData->get_colorType();
    if (config.oneColour && !config.oneSaber) {
        sliderNoteData->set_colorType(1);
    } else if (config.oneSaber) {
        int toCheckFor = 0;
        if (leftHanded) toCheckFor = 1;
        if (colorType == toCheckFor) {
            return;
        }
    }

    auto sliderType = sliderNoteData->get_sliceCount();
    if (config.ignoreBurstSliders && sliderType == 1) {
        return;
    }

    if (config.ignoreArcSliders && sliderType == 0) {
        return;
    }

    BeatmapObjectSpawnController_HandleSliderDataCallback(self, sliderNoteData);
}
