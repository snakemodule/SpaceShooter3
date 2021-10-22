#include "my_pch.h"
#include "space_partition_player_bullets_system.h"

#include "space_hash.h"

namespace systems::space_partition_player_bullets
{
	std::vector<ComponentArray*> position_component_arrays;
	std::vector<Archetype*> matching_archetypes;
}

void systems::space_partition_player_bullets::register_archetype(Archetype& matching_archetype)
{
	matching_archetypes.push_back(&matching_archetype);
	for (size_t i = 0; i < matching_archetype.signature.size(); i++)
	{
		if (matching_archetype.signature[i].name == typeid(Position).name()) {
			position_component_arrays.push_back(&matching_archetype.components[i]);
		}
	}
}

void systems::space_partition_player_bullets::partition()
{
	indexing::space_partition::player_bullet_position_map.clear();
	size_t archetype_count = position_component_arrays.size();
	for (size_t i = 0; i < archetype_count; i++)
	{
		size_t component_count = position_component_arrays[i]->count;

		Position* p = get_data_component_array<Position>(position_component_arrays[i]);
		for (size_t j = 0; j < component_count; j++)
		{
			indexing::space_partition::player_bullet_position_map.insert(
				{ indexing::space_partition::space_bucketing(p[j]) , matching_archetypes[i]->entities[j] });

		}
	}
}
