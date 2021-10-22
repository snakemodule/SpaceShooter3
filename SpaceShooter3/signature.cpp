#include "my_pch.h"
#include "signature.h"

#include "hashing.h"

bool component_info::operator==(component_info& other) const
{
	return name == other.name;
}

bool signature_equals(const signature_t& a, const signature_t& b)
{
	return std::equal(a.begin(), a.end(), b.begin(), b.end(),
		[](const component_info left, const component_info right) -> bool
		{
			return left.name == right.name;
		});
}

bool signature_equal::operator()(const signature_t& a, const signature_t& b) const
{
	return std::equal(a.begin(), a.end(), b.begin(), b.end(),
		[](const component_info left, const component_info right) -> bool
		{
			return left.name == right.name;
		});
}

std::size_t signature_hasher::operator()(signature_t const& in) const
{
	size_t size = in.size();
	size_t seed = 0;
	for (size_t i = 0; i < size; i++)
		hash_combine(seed, in[i].name);
	return seed;
}