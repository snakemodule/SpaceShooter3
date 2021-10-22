#include "my_pch.h"

#include "Manager.h"

#include "component_defs.h"

#include "enemy_attack_system.h"

namespace systems::space_partition_enemy_bullets {
	inline const signature_t signature = make_signature<EnemyBullet, Position>();

	void register_archetype(Archetype& matching_archetype);
	void partition();
}