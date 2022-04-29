#include "ScoreSubmission.hpp"

#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "hooks.hpp"
#include "config.hpp"
#include "questui/shared/BeatSaberUI.hpp"

#include "GlobalNamespace/GameplaySetupViewController.hpp"
#include "GlobalNamespace/GameplayModifiersPanelController.hpp"
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
#define MakeDelegate(DelegateType, varName) il2cpp_utils::MakeDelegate<DelegateType>(classof(DelegateType), varName))

QuestUI::IncrementSetting* modifiersIncrementSetting;
UnityEngine::UI::VerticalLayoutGroup* customModifierContainer;

UnityEngine::Transform* defaultModifiersView;

UnityEngine::Color positiveColourValue;

UnityEngine::UI::Toggle* DM1Button;
UnityEngine::UI::Toggle* UCMButton;
UnityEngine::UI::Toggle* OneSaber;
UnityEngine::UI::Toggle* OneColour;
UnityEngine::UI::Toggle* SwapTopBottomRow;
UnityEngine::UI::Toggle* HalfNotes;
UnityEngine::UI::Toggle* IgnoreChains;
UnityEngine::UI::Toggle* IgnoreArcs;


UnityEngine::UI::LayoutElement* getLayoutElement(UnityEngine::GameObject* object){
	auto layoutElement = object->GetComponent<UnityEngine::UI::LayoutElement*>();
	if(layoutElement == nullptr){
		layoutElement = object->AddComponent<UnityEngine::UI::LayoutElement*>();
	}
	return layoutElement;
}


UnityEngine::UI::Toggle* createEmptyModifierButton(UnityEngine::Transform* transform){
		UnityEngine::UI::Toggle* modifToggle = QuestUI::BeatSaberUI::CreateModifierButton(transform, "", false, nullptr, [=](bool val){});
		modifToggle->dyn_m_Interactable() = false;
		modifToggle->get_transform()->Find("BG")->get_gameObject()->SetActive(false);
		return modifToggle;
}

UnityEngine::UI::HorizontalLayoutGroup* createHorizontalGroup(){
	auto horzGroup = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(customModifierContainer);
	horzGroup->set_spacing(1.0);
	//horzGroup->m_Rect->set_offsetMax(1.0);
	//horzGroup->get_padding()->set_top(1);
	//horzGroup->get_padding()->set_bottom(0);
	return horzGroup;
}

MAKE_AUTO_HOOK_MATCH(GameplaySetupViewController_RefreshContent, &GlobalNamespace::GameplaySetupViewController::RefreshContent, void, GlobalNamespace::GameplaySetupViewController* self)
{
	GameplaySetupViewController_RefreshContent(self);

	//positiveColourValue = self->dyn__gameplayModifiersPanelController()->gameplayModifierToggles[0]->positiveColor;


	if (modifiersIncrementSetting == nullptr) {

		defaultModifiersView = self->dyn__gameplayModifiersPanelController()->get_transform()->Find("Modifiers");
		customModifierContainer = QuestUI::BeatSaberUI::CreateModifierContainer(self->dyn__gameplayModifiersPanelController()->get_transform());
		customModifierContainer->get_transform()->Translate(0,-0.154,0);

		getLayoutElement(customModifierContainer->get_gameObject())->set_preferredWidth(98.0);
		customModifierContainer->set_spacing(1.37);


		auto horzGroup1 = createHorizontalGroup();
		auto horzGroup2 = createHorizontalGroup();
		auto horzGroup3 = createHorizontalGroup();
		auto horzGroup4 = createHorizontalGroup();
		auto horzGroup5 = createHorizontalGroup();


		

		DM1Button = QuestUI::BeatSaberUI::CreateModifierButton(horzGroup1->get_transform(), "Darth Maul", (config.darthMaulOneHand || config.darthMaulBothHands), [=](bool val){
			if(val){
				HMUI::ModalView* DarthMaulModal = QuestUI::BeatSaberUI::CreateModal(self->dyn__gameplayModifiersPanelController()->get_transform(), [=](auto modal){
					UnityEngine::GameObject::Destroy(modal);
					if(!(config.darthMaulBothHands || config.darthMaulOneHand)){
						DM1Button->set_isOn(false);
					}
				}, true);
				auto DarthMaulContainer = QuestUI::BeatSaberUI::CreateModifierContainer(DarthMaulModal->get_transform());
				getLayoutElement(DarthMaulContainer->get_gameObject())->set_preferredWidth(30.0);
				QuestUI::BeatSaberUI::CreateModifierButton(DarthMaulContainer->get_transform(), "Darth Maul (One Hand)", false, [=](bool val){ 
					DarthMaulModal->Hide(true, nullptr); 
					if(UCMButton->dyn_m_IsOn()){
					config.unicornMode = false;
					UCMButton->set_isOn(false);
					}
					if(OneSaber->dyn_m_IsOn()){
						config.oneSaber = false;
						OneSaber->set_isOn(false);
					}
					config.darthMaulOneHand = true;

					SaveConfig();
					enableScoreSubmission();
				});
				createEmptyModifierButton(DarthMaulContainer->get_transform());
				QuestUI::BeatSaberUI::CreateModifierButton(DarthMaulContainer->get_transform(), "Darth Maul (Two Hands)", false, [=](bool val){ 
					DarthMaulModal->Hide(true, nullptr); 
					if(UCMButton->dyn_m_IsOn()){
					config.unicornMode = false;
					UCMButton->set_isOn(false);
					}
					if(OneSaber->dyn_m_IsOn()){
						config.oneSaber = false;
						OneSaber->set_isOn(false);
					}
					config.darthMaulBothHands = true;

					SaveConfig();
					enableScoreSubmission();

				});

				DarthMaulModal->Show(true, true, nullptr);

			}else{

				config.darthMaulBothHands = val;
				config.darthMaulOneHand = val;

			}

		});

		UCMButton = QuestUI::BeatSaberUI::CreateModifierButton(horzGroup1->get_transform(), "Unicorn Mode<br><color=#f22>Disables Score Submission</color>", config.unicornMode, [=](bool val){

            auto isOn = OneColour->dyn_m_IsOn();
			config.unicornMode = val;
			if(val){
				if(DM1Button->dyn_m_IsOn()){
					config.darthMaulOneHand = false;
					DM1Button->set_isOn(false);
				}
				if(isOn){
					config.oneSaber = false;
					OneSaber->set_isOn(false);
				}
				if(!isOn){
					config.oneColour = true;
					OneColour->set_isOn(true);
				}
				SaveConfig();
				disableScoreSubmission();
			}
		});

		OneSaber = QuestUI::BeatSaberUI::CreateModifierButton(horzGroup1->get_transform(), "Single Saber<br><color=#f22>Disables Score Submission</color>", config.oneSaber, [=](bool val){
			config.oneSaber = val;

			if(val){
				if(DM1Button->dyn_m_IsOn()){
					config.darthMaulOneHand = false;
					DM1Button->set_isOn(false);
				}
				if(UCMButton->dyn_m_IsOn()){
					config.unicornMode = false;
					UCMButton->set_isOn(false);
				}
				if(OneColour->dyn_m_IsOn()){
					config.oneColour = false;
					OneColour->set_isOn(false);
				}
				SaveConfig();
				disableScoreSubmission();
			}
		});

		OneColour = QuestUI::BeatSaberUI::CreateModifierButton(horzGroup2->get_transform(), "Single Colour<br><color=#f22>Disables Score Submission</color>", config.oneColour, [=](bool val){
			if(!val && config.unicornMode){
				OneColour->set_isOn(true);
				return;
			}

			config.oneColour = val;

			if(val){
				if(OneSaber->dyn_m_IsOn()){
					config.oneSaber = false;
					OneSaber->set_isOn(false);
				}
			}
			SaveConfig();
			disableScoreSubmission();
		});

		SwapTopBottomRow = QuestUI::BeatSaberUI::CreateModifierButton(horzGroup2->get_transform(), "TECH<br><color=#f22>Disables Score Submission</color>", config.swapTopAndBottomRow, [=](bool val){
			if(!IgnoreChains->dyn_m_IsOn()){
				config.ignoreBurstSliders = true;
				IgnoreChains->set_isOn(true);
			}
			if(!IgnoreArcs->dyn_m_IsOn()){
				config.ignoreBurstSliders = true;
				IgnoreArcs->set_isOn(true);
			}

			config.swapTopAndBottomRow = val;


			SaveConfig();
			disableScoreSubmission();
		});

		HalfNotes = QuestUI::BeatSaberUI::CreateModifierButton(horzGroup2->get_transform(), "Half Notes<br><color=#f22>Disables Score Submission</color>", config.halfNotes, [=](bool val){


			config.halfNotes = val;


			SaveConfig();
			disableScoreSubmission();
		});

		IgnoreChains = QuestUI::BeatSaberUI::CreateModifierButton(horzGroup3->get_transform(), "Ignore Chains<br><color=#f22>Disables Score Submission</color>", config.ignoreBurstSliders, [=](bool val){
			if(!val && config.swapTopAndBottomRow){
				IgnoreChains->set_isOn(true);
				return;
			}

			config.ignoreBurstSliders = val;


			SaveConfig();
			disableScoreSubmission();
		});

		IgnoreArcs = QuestUI::BeatSaberUI::CreateModifierButton(horzGroup3->get_transform(), "Ignore Arcs<br><color=#f22>Disables Score Submission</color>", config.ignoreArcSliders, [=](bool val){
			if(!val && config.swapTopAndBottomRow){
				IgnoreArcs->set_isOn(true);
				return;
			}

			config.ignoreArcSliders = val;


			SaveConfig();
			disableScoreSubmission();
		});



		createEmptyModifierButton(horzGroup3->get_transform());

		createEmptyModifierButton(horzGroup4->get_transform());

		createEmptyModifierButton(horzGroup5->get_transform());

		modifiersIncrementSetting = QuestUI::BeatSaberUI::CreateIncrementSetting(self->dyn__gameplayModifiersPanelController()->get_transform(), "Select Modifiers", 0, 1.0, 0.0, true, true, 0.0, 1.0, {0, 0}, [=](float val) {
			if (val == 0.0) {
				modifiersIncrementSetting->Text->SetText("Default Modifiers");
				defaultModifiersView->get_gameObject()->SetActive(true);

				customModifierContainer->get_gameObject()->SetActive(false);

			}
			else if (val == 1.0) {
				modifiersIncrementSetting->Text->SetText("Custom Modifiers");
				defaultModifiersView->get_gameObject()->SetActive(false);

				customModifierContainer->get_gameObject()->SetActive(true);

			}

			});
		customModifierContainer->get_gameObject()->SetActive(false);

		modifiersIncrementSetting->get_transform()->Translate({ 0,-1.9,0 });
		modifiersIncrementSetting->Text->SetText("Default Modifiers");
	}

}

