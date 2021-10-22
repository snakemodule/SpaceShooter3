#pragma once
#include "my_pch.h"

#include "Manager.h"

#include "component_defs.h"


namespace systems::enemy_attack {
	inline const signature_t signature = make_signature<EnemyTag, Position, AttackTimer>();

	inline const signature_t bullet_create_sig =
		make_signature<Position, Velocity, BoundsDespawned, EnemyBullet>();

	

	void register_archetype(Archetype& matching_archetype);
	void shoot(float dt, Manager& manager);
}



