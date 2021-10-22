#include "my_pch.h"
#include "player_attack_system.h"

#include "space_hash.h"
#include "game_data.h"

namespace systems::player_attack {
	std::vector<ComponentArray*> position_arrays;
	std::vector<ComponentArray*> timer_arrays;
}

void systems::player_attack::register_archetype(Archetype& matching_archetype)
{
	auto pname = typeid(Position).name();
	auto tname = typeid(AttackTimer).name();

	for (size_t i = 0; i < matching_archetype.signature.size(); i++)
	{
		if (matching_archetype.signature[i].name == pname) {
			position_arrays.push_back(&matching_archetype.components[i]);
		}
		else if (matching_archetype.signature[i].name == tname) {
			timer_arrays.push_back(&matching_archetype.components[i]);
		}
	}
}

void systems::player_attack::shoot(float dt, Manager& manager, bool input_shooting)
{
	auto archetype_count = position_arrays.size();
	for (size_t i = 0; i < archetype_count; i++)
	{
		AttackTimer* timer_data = get_data_component_array<AttackTimer>(timer_arrays[i]);
		Position* pos_data = get_data_component_array<Position>(position_arrays[i]);

		auto component_count = position_arrays[i]->count;
		for (size_t j = 0; j < component_count; j++)
		{
			timer_data[j].timer -= dt;
			if (timer_data[j].timer <= 0 && input_shooting)
			{
				timer_data[j].timer = game_data::player_properties.attack_delay;

				Record record = manager.create_entities_with_archetype(bullet_create_sig, 1);
				Position* position_data_created = get_data_of_component_array<Position>(*record.archetype);
				Velocity* velocity_data_created = get_data_of_component_array<Velocity>(*record.archetype);

				position_data_created[record.component_idx].x = pos_data[j].x;
				position_data_created[record.component_idx].y = pos_data[j].y;

				velocity_data_created[record.component_idx].x = 0;
				velocity_data_created[record.component_idx].y = -game_data::player_properties.bullet_speed;				
			}
		}
	}
}
