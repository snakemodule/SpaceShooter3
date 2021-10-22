#include "my_pch.h"

#include "spawn_system.h"

#include <random>

#include "SDLWindow.h"

#include "space_hash.h"

namespace systems::spawn {
    std::vector<Archetype*> matching_archetypes;    
}

void systems::spawn::register_archetype(Archetype& matching_archetype)
{
	matching_archetypes.push_back(&matching_archetype);
}

void systems::spawn::spawn(float dt, Manager& manager)
{
    unsigned enemy_count = 0;
    for (size_t i = 0; i < matching_archetypes.size(); i++)
    {
        enemy_count += static_cast<unsigned>(matching_archetypes[i]->entities.size());
    }

    if (enemy_count < 3)
    {
        Record first_entity_record = manager.create_entities_with_archetype(enemy_create_sig, 8);
        Position* position_data = get_data_of_component_array<Position>(*first_entity_record.archetype);
        EnemyMovementPeriod* period_data = get_data_of_component_array<EnemyMovementPeriod>(*first_entity_record.archetype);
        AttackTimer* attack_data = get_data_of_component_array<AttackTimer>(*first_entity_record.archetype);

        unsigned current;

        for (unsigned i = 0; i < 8; i++)
        {
            current = first_entity_record.component_idx + i;
            float r = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
            r = 100+((SCREEN_WIDTH-200)*r);
            position_data[current].x = r;            
            position_data[current].y = -10 + (static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX) * -20);                        
        }

        for (unsigned i = 0; i < 8; i++)
        {
            current = first_entity_record.component_idx + i;
        }

        for (unsigned i = 0; i < 8; i++)
        {
            current = first_entity_record.component_idx + i;
            float r = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
            r = 2*3.14f * r;
            period_data[current].period = r;
        }

        for (unsigned i = 0; i < 8; i++)
        {
            current = first_entity_record.component_idx + i;
            float r = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
            r = 3 * r;
            attack_data[current].timer = r;
        }
    }
}
