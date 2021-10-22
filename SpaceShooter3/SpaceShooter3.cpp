#include "my_pch.h"
#undef main

#include "SDLWindow.h"

#include "Manager.h"

#include "move_system.h"
#include "spawn_system.h"
#include "enemy_movement_system.h"
#include "despawn_enemy_system.h"
#include "enemy_attack_system.h"
#include "player_movement_system.h"

#include "space_hash.h"
#include "player_collision_system.h"
#include "enemy_collision_system.h"

#include "space_partition_player_bullets_system.h"
#include "space_partition_enemy_bullets_system.h"

#include "draw.h"

#include "player_attack_system.h"

Manager man;

float input_x;
float input_y;
bool input_shoot;

namespace sys = systems;

int main()
{
    SDLInit();

    man.register_system(sys::spawn::signature, sys::spawn::register_archetype);
    man.register_system(sys::move::signature, sys::move::register_archetype);
    man.register_system(sys::enemy_movement::signature, sys::enemy_movement::register_archetype);
    man.register_system(sys::bounds_despawn::signature, sys::bounds_despawn::register_archetype);
    man.register_system(sys::enemy_attack::signature, sys::enemy_attack::register_archetype);
    
    man.register_system(sys::player_movement::signature, sys::player_movement::register_archetype);
    man.register_system(sys::player_collision::signature, sys::player_collision::register_archetype);
    man.register_system(sys::player_attack::signature, sys::player_attack::register_archetype);
    man.register_system(sys::enemy_collision::signature, sys::enemy_collision::register_archetype);

    man.register_system(sys::space_partition_player_bullets::signature, sys::space_partition_player_bullets::register_archetype);
    man.register_system(sys::space_partition_enemy_bullets::signature, sys::space_partition_enemy_bullets::register_archetype);

    man.register_system(sys::draw::draw_enemy::signature, sys::draw::draw_enemy::register_archetype);
    man.register_system(sys::draw::draw_player::signature, sys::draw::draw_player::register_archetype);
    man.register_system(sys::draw::draw_enemy_bullets::signature, sys::draw::draw_enemy_bullets::register_archetype);
    man.register_system(sys::draw::draw_player_bullets::signature, sys::draw::draw_player_bullets::register_archetype);
        
    const signature_t player_sig = make_signature<PlayerTag, Velocity, Position, AttackTimer>();
    Record rec = man.create_entities_with_archetype(player_sig, 1);
    auto idx = rec.component_idx;
    get_data_of_component_array<Position>(*rec.archetype)[rec.component_idx] = { {}, 320,400 };
    get_data_of_component_array<Velocity>(*rec.archetype)[rec.component_idx] = { {}, 0,0 };
    

    auto time = SDL_GetTicks();
    float dt = 0;
    int now = 0;

    //Handle events on queue
    SDL_Event e;
    bool quit = false;
    while (!quit)
    {
        now = SDL_GetTicks();
        dt = (now - time) * 0.001f;
        time = now;

        input_x = std::clamp(input_x, -1.f, 1.f);
        input_y = std::clamp(input_y, -1.f, 1.f);

        while (SDL_PollEvent(&e) != 0)
        {   
            if (e.type == SDL_QUIT)
            {
                quit = true;
                continue;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_UP:
                    input_y += -1;
                    break;
                case SDLK_DOWN:
                    input_y += 1;
                    break;
                case SDLK_LEFT:
                    input_x += -1;
                    break;
                case SDLK_RIGHT:
                    input_x += 1;
                    break;
                case SDLK_SPACE:
                    input_shoot = true;
                    break;
                default:
                    break;
                }
            }
            else if (e.type == SDL_KEYUP)
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_UP:
                    input_y -= -1;
                    break;
                case SDLK_DOWN:
                    input_y -= 1;
                    break;
                case SDLK_LEFT:
                    input_x -= -1;
                    break;
                case SDLK_RIGHT:
                    input_x -= 1;
                    break;
                case SDLK_SPACE:
                    input_shoot = false;
                    break;
                default:
                    break;
                }
            }
        }

        sys::spawn::spawn(dt, man);
        sys::enemy_attack::shoot(dt, man);
        sys::player_attack::shoot(dt, man, input_shoot);
        sys::enemy_movement::enemy_movement(dt);
        sys::player_movement::movement(dt, input_x, input_y);
        sys::move::move(dt);
        sys::space_partition_enemy_bullets::partition();
        sys::space_partition_player_bullets::partition();
        sys::player_collision::collide(dt, man);
        sys::enemy_collision::collide(dt, man);
        sys::bounds_despawn::bounds_despawn(dt, man);

        SDL_SetRenderDrawColor(window_renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(window_renderer);      
        
        sys::draw::draw_enemy::draw();
        sys::draw::draw_player::draw();
        sys::draw::draw_enemy_bullets::draw();
        sys::draw::draw_player_bullets::draw();
        
        SDL_RenderPresent(window_renderer);
    }

    SDLQuit();   

    return 0;    
}

