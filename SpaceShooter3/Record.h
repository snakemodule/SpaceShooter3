#pragma once

#include "Archetype.h"

struct Record {
	Archetype* archetype;
	unsigned component_idx = 0;

	bool operator==(const Record& other) const;

	struct hasher
	{
		std::size_t operator()(const Record& k) const;
	};
};

template<typename T>
void set_component(Record& record, T component)
{
	if constexpr (T{}.is_tag()) {
		return;
	}
	set_component<T>(*record.archetype, record.component_idx, component);
};