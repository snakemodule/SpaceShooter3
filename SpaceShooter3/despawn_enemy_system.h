#pragma once
#include "my_pch.h"

#include "Manager.h"

#include "component_defs.h"



namespace systems::bounds_despawn {
	inline const signature_t signature = make_signature<BoundsDespawned, Position>();

	void register_archetype(Archetype& matching_archetype);

	void bounds_despawn(float dt, Manager& manager);

}
