#pragma once
#include "my_pch.h"

#include "Manager.h"

#include "component_defs.h"



namespace systems::player_collision {
	inline const signature_t signature = make_signature<PlayerTag, Position>();

	void register_archetype(Archetype& matching_archetype);
	void collide(float dt, Manager& manager);
}
