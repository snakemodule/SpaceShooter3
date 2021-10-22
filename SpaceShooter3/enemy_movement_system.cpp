#include "my_pch.h"
#include "enemy_movement_system.h"

#include <cmath>
#include "game_data.h"

namespace systems::enemy_movement 
{
	std::vector<ComponentArray*> velocity_component_arrays;
	std::vector<ComponentArray*> period_component_arrays;
}


void systems::enemy_movement::register_archetype(Archetype& matching_archetype)
{
	auto vname = typeid(Velocity).name();
	auto pname = typeid(EnemyMovementPeriod).name();
	for (size_t i = 0; i < matching_archetype.signature.size(); i++)
	{		
		if (matching_archetype.signature[i].name == vname)
		{
			velocity_component_arrays.push_back(&matching_archetype.components[i]);
		}
		if (matching_archetype.signature[i].name == pname)
		{
			period_component_arrays.push_back(&matching_archetype.components[i]);
		}
	}
}

void systems::enemy_movement::enemy_movement(float dt)
{
	size_t archetype_count = velocity_component_arrays.size();
	for (size_t i = 0; i < archetype_count; i++)
	{
		Velocity* v = get_data_component_array<Velocity>(velocity_component_arrays[i]);
		EnemyMovementPeriod* p = get_data_component_array<EnemyMovementPeriod>(period_component_arrays[i]);

		size_t component_count = velocity_component_arrays[i]->count;
		for (size_t j = 0; j < component_count; j++)
		{
			p[j].period += game_data::enemy_properties.swaying_pace*dt;
			v[j].x = std::cos(p[j].period) * game_data::enemy_properties.sideways_speed;
			v[j].y = game_data::enemy_properties.speed;
		}
	}

}
