#include "config.hpp"
#include "logging.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"

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
	
	SAVE(playStyleMode);
	SAVE(mainHand);
	SAVE(disableRumble);


	getConfig().Write();
	INFO("Saved Configuration!");
}

bool LoadConfig()
{
	INFO("Loading Configuration...");
	bool foundEverything = true;
	rapidjson::Document& doc = getConfig().config;

	LOAD(playStyleMode, GetInt());
	LOAD(mainHand, GetInt());
	LOAD(disableRumble, GetBool());

	if (!foundEverything) 
	{
		ERROR("Configuration values were missing!");
		return false;
	}
	INFO("Loaded Configuration!");
	return true;
}