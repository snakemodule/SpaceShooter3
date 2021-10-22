#pragma once
#include "my_pch.h"

#include "Manager.h"

#include "component_defs.h"

namespace systems::move {
	inline const signature_t signature = make_signature<Position,Velocity>();	
	
	void register_archetype(Archetype& matching_archetype);;

	void move(float dt);
}
