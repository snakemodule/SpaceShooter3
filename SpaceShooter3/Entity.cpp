#include "my_pch.h"
#include "Entity.h"

Entity make_entity(uint32_t idx, uint32_t generation)
{
	Entity e = Entity{ (generation << ENTITY_INDEX_BITS) | idx };
	return e;
}

Entity nil_entity() {
	Entity e = { 0 };
	return e;
}

unsigned Entity::index() const { return id & ENTITY_INDEX_MASK; }

unsigned Entity::generation() const { return (id >> ENTITY_INDEX_BITS) & ENTITY_GENERATION_MASK; }

bool Entity::operator==(const Entity& other) const 
{
	return id == other.id;
}

