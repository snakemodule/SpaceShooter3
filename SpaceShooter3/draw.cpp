#include "my_pch.h"
#include "draw.h"

#include "SDLWindow.h"
#include "game_data.h"


namespace systems::draw {
	void draw_squares(std::vector<ComponentArray*>& position_component_arrays, float size, game_data::Color c)
	{
		SDL_SetRenderDrawColor(window_renderer, c.r, c.g, c.b, c.a);
		size_t archetype_count = position_component_arrays.size();
		for (size_t i = 0; i < archetype_count; i++)
		{
			Position* p = get_data_component_array<Position>(position_component_arrays[i]);

			size_t component_count = position_component_arrays[i]->count;
			for (size_t j = 0; j < component_count; j++)
			{
				float width = size;
				float height = size;
				SDL_FRect rect{ p[j].x - (width * 0.5f), p[j].y - (height * 0.5f), width, height };
				SDL_RenderFillRectF(window_renderer, &rect);
			}
		}
	}

	void register_position(std::vector<ComponentArray*>& position_component_arrays, Archetype& matching_archetype)
	{
		auto name = typeid(Position).name();
		for (size_t i = 0; i < matching_archetype.signature.size(); i++)
		{
			if (matching_archetype.signature[i].name == name)
			{
				position_component_arrays.push_back(&matching_archetype.components[i]);
			}
		}
	}
}

namespace systems::draw::draw_enemy
{	
	std::vector<ComponentArray*> position_component_arrays;

	void register_archetype(Archetype& matching_archetype)
	{
		register_position(position_component_arrays, matching_archetype);
	}

	void draw()
	{
		draw_squares(position_component_arrays,
			game_data::enemy_properties.size,
			game_data::enemy_properties.color);
	}
}

namespace systems::draw::draw_player
{
	std::vector<ComponentArray*> position_component_arrays;

	void register_archetype(Archetype& matching_archetype)
	{
		register_position(position_component_arrays, matching_archetype);
	}

	void draw()
	{
		draw_squares(position_component_arrays,
			game_data::player_properties.size,
			game_data::player_properties.color);
	}
}

namespace systems::draw::draw_enemy_bullets
{
	std::vector<ComponentArray*> position_component_arrays;

	void register_archetype(Archetype& matching_archetype)
	{
		register_position(position_component_arrays, matching_archetype);
	}

	void draw()
	{
		draw_squares(position_component_arrays,
			game_data::enemy_properties.bullet_size,
			game_data::enemy_properties.bullets_color);
	}
}

namespace systems::draw::draw_player_bullets
{
	std::vector<ComponentArray*> position_component_arrays;

	void register_archetype(Archetype& matching_archetype)
	{
		register_position(position_component_arrays, matching_archetype);
	}

	void draw()
	{
		draw_squares(position_component_arrays,
			game_data::player_properties.bullet_size,
			game_data::player_properties.bullets_color);
	}
}
