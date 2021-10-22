#include "my_pch.h"
#include "player_collision_system.h"
#include "space_hash.h"

#include "enemy_attack_system.h"

#include "game_data.h"

namespace systems::player_collision
{
	std::vector<ComponentArray*> position_component_arrays;
	std::vector<Archetype*> matching_archetypes;
}

void systems::player_collision::register_archetype(Archetype& matching_archetype)
{
	matching_archetypes.push_back(&matching_archetype);
	for (size_t i = 0; i < matching_archetype.signature.size(); i++)
	{
		if (matching_archetype.signature[i].name == typeid(Position).name()) {
			position_component_arrays.push_back(&matching_archetype.components[i]);
		}
	}
}

void systems::player_collision::collide(float dt, Manager& manager)
{
	Archetype& bullets = manager.get_archetype(systems::enemy_attack::bullet_create_sig);
	Position* enemy_bullet_positions = get_data_of_component_array<Position>(bullets);

	size_t archetype_count = position_component_arrays.size();
	for (size_t i = 0; i < archetype_count; i++)
	{
		size_t component_count = position_component_arrays[i]->count;

		Position* p = get_data_component_array<Position>(position_component_arrays[i]);
		for (size_t j = 0; j < component_count; j++)
		{
			const float player_radius = game_data::player_properties.size / 2; 
			const float enemy_bullet_radius = game_data::enemy_properties.bullet_size / 2;
			const float collision_distance = player_radius + enemy_bullet_radius;

			const float xmax = p[j].x + collision_distance;
			const float xmin = p[j].x - collision_distance;
			const float ymax = p[j].y + collision_distance;
			const float ymin = p[j].y - collision_distance;
			const int32_t xminbucket = indexing::space_partition::find_bucket_one_dimension(xmin);
			const int32_t xmaxbucket = indexing::space_partition::find_bucket_one_dimension(xmax);
			const int32_t yminbucket = indexing::space_partition::find_bucket_one_dimension(ymin);
			const int32_t ymaxbucket = indexing::space_partition::find_bucket_one_dimension(ymax);


			for (int x = xminbucket; x <= xmaxbucket; x++)
			{
				for (int y = yminbucket; y <= ymaxbucket; y++)
				{
					size_t key = static_cast<size_t>(x) << 32;
					key |= y;
					auto range = indexing::space_partition::enemy_bullet_position_map.equal_range(key);
					for (auto it = range.first; it != range.second; ++it) 
					{
						Entity& bullet_entity = it->second;
						unsigned bullet_idx = manager.records[bullet_entity.index()].component_idx;
						Position bullet_pos = enemy_bullet_positions[bullet_idx];

						Position to_bullet = { {}, bullet_pos.x - p[j].x, bullet_pos.y - p[j].y };
						float sqr_dist_to_bullet = to_bullet.x * to_bullet.x + to_bullet.y * to_bullet.y;
						if (sqr_dist_to_bullet <= collision_distance * collision_distance)
						{

							manager.destroy(matching_archetypes[i]->entities[j]);
							manager.destroy(bullet_entity);
							return;
						}
					}
				}
			}
		}
	}
}
