#include "my_pch.h"
#include "despawn_enemy_system.h"

#include "SDLWindow.h"

#include "player_attack_system.h"
#include "enemy_attack_system.h"
#include "spawn_system.h"
#include "space_hash.h"

namespace systems::bounds_despawn {
	std::vector<Archetype*> matching_archetypes;
	std::vector<ComponentArray*> position_component_arrays;	
}

void systems::bounds_despawn::register_archetype(Archetype& matching_archetype)
{	
	matching_archetypes.push_back(&matching_archetype);
	auto name = typeid(Position).name();
	for (size_t i = 0; i < matching_archetype.signature.size(); i++)
	{
		if (matching_archetype.signature[i].name == name)
		{
			position_component_arrays.push_back(&matching_archetype.components[i]); 
			break;
		}
	}
}

void systems::bounds_despawn::bounds_despawn(float dt, Manager& manager)
{
	size_t archetype_count = matching_archetypes.size();
	for (size_t i = 0; i < archetype_count; i++)
	{	
		Position* p = get_data_component_array<Position>(position_component_arrays[i]);		
				
		int64_t component_count = static_cast<int64_t>(position_component_arrays[i]->count);
		for (int64_t j = component_count - 1; j >= 0; j--)
		{
			if (p[j].y > SCREEN_HEIGHT || p[j].y <= -50)
			{
				Entity entity_to_delete = matching_archetypes[i]->entities[j];
				//indexing::space_partition::remove_from_index(index, p[j], entity_to_delete);
				manager.destroy(entity_to_delete);
			}
		}
	}
}
