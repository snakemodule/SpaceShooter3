#include "my_pch.h"
#include "Record.h"

bool Record::operator==(const Record& other) const
{
	return (&archetype == &other.archetype && component_idx == other.component_idx);
}

std::size_t Record::hasher::operator()(const Record& k) const
{
	return ((std::hash<Archetype*>()(k.archetype) ^ (std::hash<unsigned>()(k.component_idx) << 1)) >> 1);
}