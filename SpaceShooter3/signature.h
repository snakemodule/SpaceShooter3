#pragma once


struct component_info {
	const char* name;
	unsigned size;

	bool operator==(component_info& other) const;

};

template<typename T>
component_info make_component_info() {
	if constexpr (T{}.is_tag()) {
		return { typeid(T).name(), 0 };
	}
	else
	{
		return { typeid(T).name(), sizeof(T) };
	}
}

using signature_t = std::vector<component_info>;

bool signature_equals(const signature_t& a, const signature_t& b);

template <typename... Types>
signature_t make_signature()
{
	signature_t sig;

	sig.insert(sig.end(), { make_component_info<Types>()... });

	std::sort(sig.begin(), sig.end(), [](component_info& left, component_info& right)
		{
			return left.name < right.name;
		});
	return sig;
}

struct signature_equal
{
	bool operator()(const signature_t& lhs, const signature_t& rhs) const;
};

struct signature_hasher
{
	std::size_t operator()(signature_t const& in) const;
};