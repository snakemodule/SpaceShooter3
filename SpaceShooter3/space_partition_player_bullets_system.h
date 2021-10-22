#pragma once
#include "my_pch.h"

#include "Manager.h"

#include "component_defs.h"

#include "player_attack_system.h"

namespace systems::space_partition_player_bullets 
{
	inline const signature_t signature = make_signature<PlayerBullet, Position>();

	void register_archetype(Archetype& matching_archetype);
	void partition();
}