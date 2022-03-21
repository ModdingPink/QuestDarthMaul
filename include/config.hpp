#pragma once
#include <string>


typedef struct config_t {
	int playStyleMode = 0;
	int mainHand = 0;
	bool disableRumble = false;

} config_t;

extern config_t config;

bool LoadConfig();
void SaveConfig();