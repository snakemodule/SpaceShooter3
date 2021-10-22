#pragma once

#include "component_array.h"

#include "signature.h"

#include "Entity.h"

struct Archetype;

struct ArchetypeEdge {
	Archetype* added = nullptr;
	Archetype* removed = nullptr;

	bool operator==(const ArchetypeEdge& other) const;

	struct hasher
	{
		std::size_t operator()(const ArchetypeEdge& k) const;
	};
};

//after creation, signature and component lists should not change, 
//consider them stable pointers TODO use constructor with const signature
struct Archetype
{
	std::vector<component_info> signature;
	std::vector<ComponentArray> components;
	std::vector<Entity> entities;
	std::unordered_map<const char*, ArchetypeEdge> edges;
};

template<typename T>
void set_component(Archetype& archetype, unsigned idx, T component)
{
	if constexpr (T{}.is_tag()) {
		return;
	}
	for (size_t i = 0; i < archetype.signature.size(); i++)
	{
		if (archetype.signature[i].name == typeid(T).name())
		{
			T* data = get_data_component_array<T>(&archetype.components[i]);
			data[idx] = component;
			return;
		}
	}
};

template<typename T>
unsigned index_of_component_array(Archetype& archetype)
{
	auto name = typeid(T).name();
	for (size_t i = 0; i < archetype.signature.size(); i++)
	{
		if (archetype.signature[i].name == name)
			return i;
	}
}

template<typename T>
T* get_data_of_component_array(Archetype& archetype)
{
	auto name = typeid(T).name();
	for (size_t i = 0; i < archetype.signature.size(); i++)
	{
		if (archetype.signature[i].name == name)
			return get_data_component_array<T>(&archetype.components[i]);
	}
	return nullptr;
}