#include "my_pch.h"
#include "player_movement_system.h"

#include "game_data.h"


namespace systems::player_movement 
{
	std::vector<ComponentArray*> velocity_component_arrays;
}

void systems::player_movement::register_archetype(Archetype& matching_archetype)
{
	for (size_t i = 0; i < matching_archetype.signature.size(); i++)
	{
		if (matching_archetype.signature[i].name == typeid(Velocity).name()) {
			velocity_component_arrays.push_back(&matching_archetype.components[i]);
		}
	}
}

void systems::player_movement::movement(float dt, float input_x, float input_y)
{
	size_t archetype_count = velocity_component_arrays.size();
	for (size_t i = 0; i < archetype_count; i++)
	{
		size_t component_count = velocity_component_arrays[i]->count;

		Velocity* v = get_data_component_array<Velocity>(velocity_component_arrays[i]);
		for (size_t j = 0; j < component_count; j++)
		{
			float magnitude = std::sqrt(input_x * input_x + input_y * input_y);
			if (magnitude == 0)
			{
				magnitude = 1;
			}
			float x = input_x / magnitude;
			float y = input_y / magnitude;
			
			v[j].x = x * game_data::player_properties.speed;
			v[j].y = y * game_data::player_properties.speed;
		}
	}


}
