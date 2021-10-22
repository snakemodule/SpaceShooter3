#include "my_pch.h"
#include "Archetype.h"

std::size_t ArchetypeEdge::hasher::operator()(const ArchetypeEdge& k) const
{
	return ((std::hash<Archetype*>()(k.added) ^ (std::hash<Archetype*>()(k.removed) << 1)) >> 1);
}

bool ArchetypeEdge::operator==(const ArchetypeEdge& other) const
{
	return (added == other.added && removed == other.removed);
}