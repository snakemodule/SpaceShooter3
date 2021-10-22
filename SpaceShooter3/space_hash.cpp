#include "my_pch.h"
#include "space_hash.h"

#include "SDLWindow.h"

#include <iostream>

namespace indexing::space_partition
{	
	std::unordered_multimap<size_t, Entity> enemy_bullet_position_map;
	std::unordered_multimap<size_t, Entity> player_bullet_position_map;

	
	size_t space_bucketing(const Position& p) 
	{
		int32_t x;
		int32_t y;
		x = static_cast<int32_t>(p.x) / 80; //truncating
		y = static_cast<int32_t>(p.y) / 80;

		x = (p.x < 0) ? x - 1 : x;
		y = (p.y < 0) ? y - 1 : y;

		size_t result = static_cast<size_t>(x) << 32;
		result |= y;

		return result;
	}
	
	int32_t find_bucket_one_dimension(float a) {
		int32_t bucket = static_cast<int32_t>(a) / 80; //truncating
		bucket = (a < 0) ? bucket - 1 : bucket;
		return bucket;
	}
}