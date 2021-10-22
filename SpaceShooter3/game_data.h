#pragma once
#include "my_pch.h"

namespace game_data 
{
	struct Color
	{
		Uint8 r, g, b, a;
	};

	inline const struct {
		Color color = { 0x8B, 0x00, 0x8B, 0xFF }; //purple / darkmagenta
		Color bullets_color = { 0xFF, 0xA5, 0x00, 0xFF }; //orange
		float size = 20.f;
		float speed = 50.f;
		float bullet_size = 8.f;
		float bullet_speed = 100.f;
		float attack_delay = 3.f;

		float sideways_speed = 50.f;
		float swaying_pace = 2.f;
	} enemy_properties;
	
	inline const struct {
		Color color = { 0xFF, 0xFF, 0xFF, 0xFF }; //white
		Color bullets_color = { 0xDC, 0x14, 0x3C, 0xFF }; //crimson
		float size = 14.f;
		float speed = 200.f;
		float bullet_size = 8.f;
		float bullet_speed = 300.f;
		float attack_delay = 0.3f;
	} player_properties;

}

