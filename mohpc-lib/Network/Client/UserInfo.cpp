#include <MOHPC/Network/Client/UserInfo.h>
#include <MOHPC/Utility/Info.h>

using namespace MOHPC;
using namespace MOHPC::Network;

MOHPC_OBJECT_DEFINITION(UserInfo);

UserInfo::UserInfo()
	: snaps(20)
	, rate(5000)
{
}

UserInfo::UserInfo(UserInfo&& other)
	: name(std::move(other.name))
	, rate(other.rate)
	, snaps(other.snaps)
	, properties(std::move(other.properties))
{
}

UserInfo& UserInfo::operator=(UserInfo&& other)
{
	name = std::move(other.name);
	rate = other.rate;
	snaps = other.snaps;
	properties = std::move(other.properties);
	return *this;
}

UserInfo::~UserInfo()
{
}

void UserInfo::setRate(uint32_t rateVal)
{
	rate = rateVal;
}

uint32_t UserInfo::getRate() const
{
	return rate;
}

void UserInfo::setSnaps(uint32_t snapsVal)
{
	snaps = snapsVal;
}

uint32_t UserInfo::getSnaps() const
{
	return snaps;
}

void UserInfo::setName(const char* newName)
{
	name = newName;
}

const char* UserInfo::getName() const
{
	return name.c_str();
}

void UserInfo::setPlayerAlliedModel(const char* newModel)
{
	properties.SetPropertyValue("dm_playermodel", newModel);
}

const char* UserInfo::getPlayerAlliedModel() const
{
	return properties.GetPropertyRawValue("dm_playermodel");
}

void UserInfo::setPlayerGermanModel(const char* newModel)
{
	properties.SetPropertyValue("dm_playergermanmodel", newModel);
}

const char* UserInfo::getPlayerGermanModel() const
{
	return properties.GetPropertyRawValue("dm_playergermanmodel");
}

void UserInfo::setUserKeyValue(const char* key, const char* value)
{
	properties.SetPropertyValue(key, value);
}

const char* UserInfo::getUserKeyValue(const char* key) const
{
	return properties.GetPropertyRawValue(key);
}

const PropertyObject& UserInfo::getPropertyObject() const
{
	return properties;
}

void ClientInfoHelper::fillInfoString(const UserInfo& userInfo, Info& info)
{
	// Build mandatory variables
	info.SetValueForKey("rate", str::printf("%i", userInfo.getRate()));
	info.SetValueForKey("snaps", str::printf("%i", userInfo.getSnaps()));
	info.SetValueForKey("name", userInfo.getName());

	// Build miscellaneous values
	for (PropertyMapIterator it = userInfo.getPropertyObject().GetIterator(); it; ++it)
	{
		info.SetValueForKey(
			it.key().GetFullPropertyName(),
			it.value()
		);
	}
}
