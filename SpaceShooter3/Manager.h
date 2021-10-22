#pragma once
#include "my_pch.h"

#include "Entity.h"

#include "component_array.h"

#include "signature.h"

#include "Archetype.h"

#include "Record.h"

class Manager
{	
	const unsigned MINIMUM_FREE_INDICES = 1024;
public:

	std::vector<unsigned char> _generation;
	std::deque<unsigned> _free_indices;
	std::vector<Record> records;

	Entity create();
	bool alive(Entity) const;
	void destroy(Entity);

	void remove_entity_from_archetype(Entity e);

	Record create_entity_with_archetype(const signature_t& sig);
	Record create_entities_with_archetype(const signature_t& signature, unsigned count);
	
	void add_to_records(Entity e, Record r);

	//deque for stable pointers
	std::deque<Archetype> archetype_storage = std::deque<Archetype>(1);

	Archetype& create_archetype(Archetype* preceding, component_info component);
	Archetype& get_archetype(const signature_t& signature);
	void signal_archetype_created(Archetype& archetype);

	//system signatures and callbacks for them
	std::unordered_map<signature_t, std::vector<void (*)(Archetype&)>, signature_hasher, signature_equal> system_callbacks;

	//matching archetype lookup cache
	std::unordered_map<signature_t, std::vector<Archetype*>, signature_hasher, signature_equal> matching_archetype_cache;

	const std::vector<Archetype*>& lookup_matching_archetypes(const signature_t search_sig);
	
	void register_system(const signature_t& system_signature, void (*callback)(Archetype&));

};

