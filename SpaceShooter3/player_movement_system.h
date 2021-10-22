#pragma once
#include "my_pch.h"

#include "Manager.h"

#include "component_defs.h"

namespace systems::player_movement
{
	inline const signature_t signature = make_signature<PlayerTag, Velocity>();
		
	void register_archetype(Archetype& matching_archetype);
	void movement(float dt, float x, float y);
}
