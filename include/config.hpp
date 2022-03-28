#pragma once
#include <string>


typedef struct config_t {
	bool darthMaulOneHand = false;
	bool darthMaulBothHands = false;
	bool unicornMode = false;
	bool swapControllers = false;
	bool oneSaber = false;
	bool oneColour = false;
	bool swapTopAndBottomRow = false;
	bool halfNotes = false;
	bool ignoreBurstSliders = false;
	bool ignoreArcSliders = false;
	int mainHand = 0;
	bool disableRumble = false;

} config_t;

extern config_t config;

bool LoadConfig();
void SaveConfig();