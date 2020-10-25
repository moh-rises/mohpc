#pragma once

#include "../Global.h"
#include "../Common/str.h"
#include "../Vector.h"
#include <map>

namespace MOHPC
{
	class PropertyDef
	{
	private:
		str propertyName;

	public:
		PropertyDef(const char* inPropertyName);
		PropertyDef(str&& inPropertyName);
		PropertyDef(const str& inPropertyName);

		bool operator<(const PropertyDef& right) const;

		MOHPC_EXPORTS const char* GetPropertyName() const;
		MOHPC_EXPORTS const char* GetFullPropertyName() const;

	private:
		bool IsKeyed() const;
	};
	using PropertyMap = std::map<PropertyDef, str>;

	class PropertyMapIterator
	{
	private:
		PropertyMap::const_iterator* it;
		const PropertyMap* map;

	public:
		MOHPC_EXPORTS PropertyMapIterator();
		MOHPC_EXPORTS PropertyMapIterator(const PropertyMap& map);
		MOHPC_EXPORTS PropertyMapIterator(PropertyMapIterator&& other);
		MOHPC_EXPORTS PropertyMapIterator& operator=(PropertyMapIterator&& other);
		MOHPC_EXPORTS PropertyMapIterator(const PropertyMapIterator& iterator) = delete;
		MOHPC_EXPORTS PropertyMapIterator& operator=(const PropertyMapIterator& iterator) = delete;
		MOHPC_EXPORTS ~PropertyMapIterator();

		MOHPC_EXPORTS operator bool() const;
		MOHPC_EXPORTS const PropertyMap* getMap() const;

		MOHPC_EXPORTS const PropertyDef& key() const;
		MOHPC_EXPORTS const str& value() const;

		MOHPC_EXPORTS PropertyMapIterator& operator++();
	};

	class PropertyObject
	{
	private:
		PropertyMap keyValues;

	public:
		MOHPC_EXPORTS void clear();
		MOHPC_EXPORTS bool HasProperty(const char* Key) const;
		MOHPC_EXPORTS bool hasAnyProperty() const;
		MOHPC_EXPORTS size_t count() const;
		MOHPC_EXPORTS const char* GetPropertyRawValue(const char* Key) const;
		MOHPC_EXPORTS const str& GetPropertyStringValue(const char* Key, const str& defaultValue = "") const;
		MOHPC_EXPORTS int8_t GetPropertyCharValue(const char* Key, int8_t defaultValue = 0) const;
		MOHPC_EXPORTS int16_t GetPropertyShortValue(const char* Key, int16_t defaultValue = 0) const;
		MOHPC_EXPORTS int32_t GetPropertyIntegerValue(const char* Key, int32_t defaultValue = 0) const;
		MOHPC_EXPORTS int64_t GetPropertyLongValue(const char* Key, int64_t defaultValue = 0) const;
		MOHPC_EXPORTS uint8_t GetPropertyByteValue(const char* Key, uint8_t defaultValue = 0) const;
		MOHPC_EXPORTS uint16_t GetPropertyUnsignedShortValue(const char* Key, uint16_t defaultValue = 0) const;
		MOHPC_EXPORTS uint32_t GetPropertyUnsignedIntegerValue(const char* Key, uint32_t defaultValue = 0) const;
		MOHPC_EXPORTS uint64_t GetPropertyUnsignedLongValue(const char* Key, uint64_t defaultValue = 0) const;
		MOHPC_EXPORTS float GetPropertyFloatValue(const char* Key, float defaultValue = 0) const;
		MOHPC_EXPORTS double GetPropertyDoubleValue(const char* Key, double defaultValue = 0) const;
		MOHPC_EXPORTS long double GetPropertyLongDoubleValue(const char* Key, long double defaultValue = 0) const;
		MOHPC_EXPORTS Vector GetPropertyVectorValue(const char* Key, const Vector& defaultValue = Vector(0, 0, 0)) const;
		MOHPC_EXPORTS void SetPropertyValue(const char* Key, const char* Value);
		void SetPropertyDef(const PropertyDef& Key, str&& Value);

		MOHPC_EXPORTS PropertyMapIterator GetIterator() const;

		// Can't be used outside of the module due to STL cross-boundaries
		const PropertyMap& GetPropertyMap() const;

	private:
		const str* GetPropertyValuePointer(const char* Key) const;
	};
}
