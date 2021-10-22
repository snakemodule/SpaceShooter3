#pragma once
#include "my_pch.h"

#include "Manager.h"

#include "component_defs.h"
#include "game_data.h"

namespace systems::draw {
	void draw_squares(std::vector<ComponentArray*>& position_component_arrays,
		float size, game_data::Color c);
	void register_position(std::vector<ComponentArray*>& position_component_arrays, Archetype& matching_archetype);
}

namespace systems::draw::draw_enemy {
	inline const signature_t signature = make_signature<EnemyTag, Position>();	
	void register_archetype(Archetype& matching_archetype);
	void draw();
}

namespace systems::draw::draw_player
{
	inline const signature_t signature = make_signature<PlayerTag, Position>();
	void register_archetype(Archetype& matching_archetype);
	void draw();
}

namespace systems::draw::draw_enemy_bullets
{
	inline const signature_t signature = make_signature<EnemyBullet, Position>();
	void register_archetype(Archetype& matching_archetype);
	void draw();
}

namespace systems::draw::draw_player_bullets
{
	inline const signature_t signature = make_signature<PlayerBullet, Position>();
	void register_archetype(Archetype& matching_archetype);
	void draw();
}