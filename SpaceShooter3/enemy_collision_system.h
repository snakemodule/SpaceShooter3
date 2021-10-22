#pragma once
#pragma once
#include "my_pch.h"

#include "Manager.h"

#include "component_defs.h"



namespace systems::enemy_collision {
	inline const signature_t signature = make_signature<EnemyTag, Position>();

	void register_archetype(Archetype& matching_archetype);
	void collide(float dt, Manager& manager);
}
