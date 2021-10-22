#pragma once
#include "my_pch.h"

#include "Entity.h"
#include "component_defs.h"

namespace indexing::space_partition
{

	extern std::unordered_multimap<size_t, Entity> enemy_bullet_position_map;
	extern std::unordered_multimap<size_t, Entity> player_bullet_position_map;

	size_t space_bucketing(const Position& p);

	int32_t find_bucket_one_dimension(float a);
}
