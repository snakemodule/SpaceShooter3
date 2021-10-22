#pragma once
#include "my_pch.h"

#include "Manager.h"

#include "component_defs.h"

namespace systems::enemy_movement {
	inline const signature_t signature = make_signature<EnemyTag, EnemyMovementPeriod, Velocity>();

	void register_archetype(Archetype& matching_archetype);

	void enemy_movement(float dt);

}
