#include "my_pch.h"
#include "move_system.h"

#include <iostream>

#include "player_attack_system.h"
#include "enemy_attack_system.h"
#include "spawn_system.h"
#include "space_hash.h"

namespace systems::move {
	std::vector<Archetype*> matching_archetypes;
	std::vector<ComponentArray*> position_component_arrays;
	std::vector<ComponentArray*> velocity_component_arrays;
}


void systems::move::register_archetype(Archetype& matching_archetype) {
	matching_archetypes.push_back(&matching_archetype);
	for (size_t i = 0; i < matching_archetype.signature.size(); i++)
	{
		ComponentArray* array_to_add = &matching_archetype.components[i];
		auto component_id = matching_archetype.signature[i].name;
		
		if (matching_archetype.signature[i].name == typeid(Position).name()) {
			position_component_arrays.push_back(&matching_archetype.components[i]);
		}
		else if (matching_archetype.signature[i].name == typeid(Velocity).name()) {
			velocity_component_arrays.push_back(&matching_archetype.components[i]);
		}
	}
}

void systems::move::move(float dt)
{
	size_t archetype_count = matching_archetypes.size();
	for (size_t i = 0; i < archetype_count; i++)
	{
		assert(position_component_arrays[i]->count == velocity_component_arrays[i]->count);

		size_t component_count = velocity_component_arrays[i]->count;

		Position* pos = get_data_component_array<Position>(position_component_arrays[i]);
		Velocity* v = get_data_component_array<Velocity>(velocity_component_arrays[i]);
		
		for (size_t j = 0; j < component_count; j++)
		{
			pos[j].x += v[j].x * dt;
			pos[j].y += v[j].y * dt;
		}
	}
}

void create_enemy(Manager& man) 
{
	signature_t signature = make_signature<Position, EnemyTag>();
	Record record = man.create_entity_with_archetype(signature);

	Position pos;	
	set_component<Position>(record, pos);
}

