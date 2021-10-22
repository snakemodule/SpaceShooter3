#include "my_pch.h"
#include "Manager.h"



/// <summary>
/// Creates an entity and places it in the specified archetype with uninitialized components.
/// </summary>
/// <param name="signature">The list of components the entity will have</param>
/// <returns>Record of where the components can be found</returns>
Record Manager::create_entity_with_archetype(const signature_t& signature)
{
	Entity e = create();
	Archetype& arc = get_archetype(signature);

	for (size_t i = 0; i < arc.components.size(); i++)
	{
		allocate_element_back_component_array(&arc.components[i]);
	}
	arc.entities.push_back(e);

	Record r = { &arc, static_cast<unsigned>(arc.entities.size() - 1) };
	add_to_records(e, r);

	return r;
}

/// <summary>
/// Creates multiple entities in the specified archetype with uninitialized components.
/// </summary>
/// <param name="signature"></param>
/// <param name="count"></param>
/// <returns>The first created entity record</returns>
Record Manager::create_entities_with_archetype(const signature_t& signature, unsigned count)
{	
	assert(count > 0);

	Archetype& arc = get_archetype(signature);

	//std::vector<Record> created_records = std::vector<Record>(count);
	arc.entities.reserve(arc.entities.size()+count);

	Entity e = create();
	arc.entities.push_back(e);
	Record first_record = { &arc , static_cast<unsigned>(arc.entities.size() - 1) };
	add_to_records(e, first_record);

	for (size_t i = 1; i < count; i++)
	{
		Entity e = create();
		arc.entities.push_back(e);

		Record r = { &arc, static_cast<unsigned>(arc.entities.size() - 1) };	
		add_to_records(e, r);
	}

	for (size_t i = 0; i < arc.components.size(); i++)
	{
		allocate_elements_back_component_array(&arc.components[i], count);
	}
	
	return first_record;
}

void Manager::add_to_records(Entity e, Record r)
{
	auto eidx = e.index();
	if (eidx <= records.size() - 1 && records.size() > 0)
	{
		records[eidx] = r;
	}
	else {
		records.push_back(r);
		assert(records.size() == _generation.size());
	}
}

/// <summary>
/// creates an archetype in storage, and connecting it to a preceding archetype
/// </summary>
/// <param name="preceding"></param>
/// <param name="component"></param>
/// <returns>the created archetype</returns>
Archetype& Manager::create_archetype(Archetype * preceding, component_info component)
{
	Archetype new_archetype;
	new_archetype.edges[component.name].removed = preceding;

	//setup new signature
	new_archetype.signature = preceding->signature;
	new_archetype.signature.push_back(component);
	std::sort(new_archetype.signature.begin(), new_archetype.signature.end(), 
		[] (component_info& left, component_info& right)
		{
			return left.name < right.name;
		});

	//create component arrays
	for (size_t i = 0; i < new_archetype.signature.size(); i++)
	{
		new_archetype.components.push_back({ 0, new_archetype.signature[i].size, {} });
	}

	archetype_storage.push_back(new_archetype);
		
	matching_archetype_cache.clear();//dirty the lookup cache

	return archetype_storage.back();
}

/// <summary>
/// creates a new entity id without any components.
/// </summary>
/// <returns></returns>
Entity Manager::create()
{
	unsigned idx;
	if (_free_indices.size() > MINIMUM_FREE_INDICES) {
		idx = _free_indices.front();
		_free_indices.pop_front();
	}
	else {
		_generation.push_back(0);
		idx = static_cast<unsigned>(_generation.size()) - 1;
		assert(idx < (1 << ENTITY_INDEX_BITS));
	}

	auto result = make_entity(idx, _generation[idx]);

	return result;
}


/// <summary>
/// gets the archetype from storage that matches the signature, 
/// if it doesn't exist yet it is created.
/// </summary>
/// <param name="signature"></param>
/// <returns>archetype in storage matching signature</returns>
Archetype& Manager::get_archetype(const signature_t& signature)
{
	Archetype* node = &archetype_storage[0];
	for (int i = 0; i < signature.size(); i++)
	{
		ArchetypeEdge* edge = &node->edges[signature[i].name];
		if (!edge->added) {
			Archetype& new_archetype = create_archetype(node, signature[i]);
			edge->added = &new_archetype;

			signal_archetype_created(new_archetype);
		}
		node = edge->added;
	}
	return *node;
}

/// <summary>
/// signals registered systems via callbacks that an archetype matching their signature has been created.
/// </summary>
/// <param name="archetype">newly created signature which needs to be passed to matching systems</param>
void Manager::signal_archetype_created(Archetype& archetype)
{
	const signature_t& archetype_sig = archetype.signature;
	auto as_begin = archetype_sig.begin();
	auto as_end = archetype_sig.end();

	for (auto it = system_callbacks.begin(); it != system_callbacks.end(); it++)
	{
		const signature_t& system_sig = it->first;
		if (std::includes(as_begin, as_end, system_sig.begin(), system_sig.end(), 
			[](const component_info left, const component_info right) -> bool
			{
				return left.name < right.name;
			}))
		{
			for (std::function<void(Archetype&)> callback : it->second)
				callback(archetype);
		}
	}
}



bool Manager::alive(Entity e) const
{
	return _generation[e.index()] == e.generation();
}

void Manager::destroy(Entity e)
{
	const unsigned idx = e.index();
	++_generation[idx];
	_free_indices.push_back(idx);

	remove_entity_from_archetype(e);
}

void Manager::remove_entity_from_archetype(Entity e)
{
	if (e.index() >= records.size())
		return;

	Record record = records[e.index()];
	auto idx = record.component_idx;

	//swap & pop components
	auto& comps = record.archetype->components;
	for (size_t i = 0; i < comps.size(); i++)
	{
		erase_with_last_element_component_array(&comps[i], idx);
	}

	//swap & pop entity id
	auto& ents = record.archetype->entities;	

	Entity& dest = ents[idx];
	Entity& back = ents[ents.size() - 1];
	if (idx != ents.size() - 1)
	{
		Entity swap_temp;
		swap_temp = back;
		back = dest;
		dest = swap_temp;
		records[dest.index()].component_idx = idx;
	}
	records[back.index()].component_idx = 0;
	records[back.index()].archetype = nullptr; //invalidate record
	ents.pop_back();	
}

const std::vector<Archetype*>& Manager::lookup_matching_archetypes(const signature_t search_sig)
{
	assert(search_sig.size());

	auto cache_it = matching_archetype_cache.find(search_sig);
	if (cache_it != matching_archetype_cache.end())
	{
		return cache_it->second;
	}

	std::vector<Archetype*> result;
	for (auto it = archetype_storage.begin(); it != archetype_storage.end(); it++)
	{
		auto as_begin = it->signature.begin();
		auto as_end = it->signature.end();

		if (std::includes(as_begin, as_end, search_sig.begin(), search_sig.end(),
			[](const component_info left, const component_info right) -> bool
			{
				return left.name < right.name;
			}))
		{
			result.push_back(&(*it));
		}
	}
	matching_archetype_cache[search_sig] = result;
	return matching_archetype_cache[search_sig];
}

void Manager::register_system(const signature_t& system_signature, void (*callback)(Archetype&))
{
	system_callbacks[system_signature].push_back(callback);
};


