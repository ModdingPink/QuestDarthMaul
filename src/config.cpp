#include "config.hpp"
#include "logging.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "static-defines.h"

config_t config;

Configuration& getConfig() {
	static Configuration config({ID, VERSION});
	config.Load();
	return config;
}

#define SAVE(name) \
	doc.AddMember(#name, config.name, allocator)

#define LOAD(name, method) \
	auto itr_ ##name = doc.FindMember(#name); \
	if (itr_ ##name != doc.MemberEnd()) { \
		config.name = itr_ ##name->value.method; \
	}else { foundEverything = false; } 

void SaveConfig()
{
	INFO("Saving Configuration...");
	rapidjson::Document& doc = getConfig().config;
	doc.RemoveAllMembers();
	doc.SetObject();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	
	SAVE(darthMaulOneHand);
	SAVE(darthMaulBothHands);
	SAVE(unicornMode);
	SAVE(oneSaber);
	SAVE(oneColour);
	SAVE(mainHand);
	SAVE(disableRumble);
	SAVE(swapControllers);
	SAVE(swapTopAndBottomRow);
	SAVE(halfNotes);
	SAVE(ignoreBurstSliders);
	SAVE(ignoreArcSliders);


	getConfig().Write();
	INFO("Saved Configuration!");
}

bool LoadConfig()
{
	INFO("Loading Configuration...");
	bool foundEverything = true;
	rapidjson::Document& doc = getConfig().config;

	LOAD(mainHand, GetInt());
	LOAD(disableRumble, GetBool());
	LOAD(darthMaulOneHand, GetBool());
	LOAD(darthMaulBothHands, GetBool());
	LOAD(unicornMode, GetBool());
	LOAD(oneSaber, GetBool());
	LOAD(oneColour, GetBool());
	LOAD(swapControllers, GetBool());
	LOAD(swapTopAndBottomRow, GetBool());
	LOAD(ignoreBurstSliders, GetBool());
	LOAD(halfNotes, GetBool());
	LOAD(ignoreArcSliders, GetBool());

	if (!foundEverything) 
	{
		ERROR("Configuration values were missing!");
		return false;
	}
	INFO("Loaded Configuration!");
	return true;
}