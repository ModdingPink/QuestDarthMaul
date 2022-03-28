
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "hooks.hpp"
#include "config.hpp"
#include "levelInfo.hpp"
#include "questui/shared/BeatSaberUI.hpp"

#include "GlobalNamespace/BeatmapObjectSpawnController.hpp"
#include "GlobalNamespace/NoteData.hpp"
#include "GlobalNamespace/SliderData.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Quaternion.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "UnityEngine/Events/UnityAction.hpp"

MAKE_AUTO_HOOK_MATCH(BeatmapObjectSpawnController_HandleNoteDataCallback, &GlobalNamespace::BeatmapObjectSpawnController::HandleNoteDataCallback, void, GlobalNamespace::BeatmapObjectSpawnController* self, GlobalNamespace::NoteData* noteData)
{
    if(config.oneColour && ! config.oneSaber){
        noteData->colorType = 1;
    }else if(config.oneSaber){
        int toCheckFor = 0;
        if(leftHanded) toCheckFor = 1;
        if(noteData->colorType == toCheckFor){
            return;
        }
    }

    
    if(config.ignoreBurstSliders){
        if(noteData->gameplayType == 2){
            noteData->gameplayType = 0;
        }

        if(noteData->scoringType == 4 || noteData->scoringType == 5){
            noteData->scoringType = 1;
        }
    }

    if(config.ignoreArcSliders){
        if(noteData->scoringType == 2 || noteData->scoringType == 3){
            noteData->scoringType = 1;
        }
    }


    if(config.swapTopAndBottomRow){

        if(noteData->noteLineLayer == 0){
            noteData->noteLineLayer = 2;
        }else if(noteData->noteLineLayer == 2){
            noteData->noteLineLayer = 0;
        }

    }


    if(config.halfNotes){
        if(noteData->gameplayType != 1){
            noteData->ChangeToBurstSliderHead();
        }
    }

    BeatmapObjectSpawnController_HandleNoteDataCallback(self, noteData);
}

MAKE_AUTO_HOOK_MATCH(BeatmapObjectSpawnController_HandleSliderDataCallback, &GlobalNamespace::BeatmapObjectSpawnController::HandleSliderDataCallback, void, GlobalNamespace::BeatmapObjectSpawnController* self, GlobalNamespace::SliderData* sliderNoteData)
{
    if(config.oneColour && ! config.oneSaber){
        sliderNoteData->colorType = 1;
    }else if(config.oneSaber){
        int toCheckFor = 0;
        if(leftHanded) toCheckFor = 1;
        if(sliderNoteData->colorType == toCheckFor){
            return;
        }
    }

    if(config.ignoreBurstSliders){
        if(sliderNoteData->sliderType == 1){
            return;
        }
        
    }

    if(config.ignoreArcSliders){
        if(sliderNoteData->sliderType == 0){
            return;
        }
        
    }

    BeatmapObjectSpawnController_HandleSliderDataCallback(self, sliderNoteData);
}
