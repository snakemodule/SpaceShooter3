#pragma once
#include "my_pch.h"

const unsigned ENTITY_INDEX_BITS = 24; //upgraded from 22
const unsigned ENTITY_INDEX_MASK = (1 << ENTITY_INDEX_BITS) - 1;

const unsigned ENTITY_GENERATION_BITS = 8;
const unsigned ENTITY_GENERATION_MASK = (1 << ENTITY_GENERATION_BITS) - 1;

struct Entity {
	unsigned id = 0;

	unsigned index() const;
	unsigned generation() const;

	bool operator==(const Entity& other) const;
	
};

Entity make_entity(uint32_t idx, uint32_t generation);

Entity nil_entity();