#pragma once
#include "my_pch.h"

#include "Manager.h"

#include "component_defs.h"


namespace systems::spawn {
	inline const signature_t signature = make_signature<EnemyTag>();

	inline const signature_t enemy_create_sig =
		make_signature<Position, EnemyMovementPeriod, Velocity, EnemyTag, BoundsDespawned, AttackTimer>();

	extern std::vector<Archetype*> matching_archetypes;

	void register_archetype(Archetype& matching_archetype);
	void spawn(float dt, Manager& manager);
}



