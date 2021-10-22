#pragma once
#include "my_pch.h"

struct ComponentArray {
	unsigned count = 0;
	unsigned component_size = 0;
	std::vector<unsigned char> buffer = {};	
};

template<typename T>
bool is_full_component_array(ComponentArray* array)
{
	if constexpr (T{}.is_tag()) {
		return true;
	}
	return sizeof(T) * (array->count) >= array->buffer.size();
}

template<typename T>
T* get_data_component_array(ComponentArray* array)
{
	if constexpr (T{}.is_tag()) {
		return nullptr;
	}
	T* result = reinterpret_cast<T*>(array->buffer.data());
	return result;
}

template<typename T>
void push_back_component_array(ComponentArray* array, T& component)
{
	if constexpr (T{}.is_tag()) {
		return;
	}
	if (is_full_component_array<T>(array))
	{
		array->buffer.resize(array->buffer.size() * 2 + sizeof(T));
	}
	++array->count;
	T* data = get_data_component_array<T>(array);
	data[array->count] = component;
}

template<typename T>
void erase_back_component_array(ComponentArray* array)
{
	if constexpr (T{}.is_tag()) {
		return;
	}
	if (array->count > 0)
	{
		--array->count;
	}
}

template<typename T>
void swap_component_array(ComponentArray* array, unsigned a_idx, unsigned b_idx)
{
	if constexpr (T{}.is_tag()) {
		return;
	}
	T* data = get_data_component_array<T>(array);
	T temp = data[a_idx];
	data[a_idx] = data[b_idx];
	data[b_idx] = temp;
}

void erase_with_last_element_component_array(ComponentArray* array, unsigned idx);

void allocate_element_back_component_array(ComponentArray* array);
void allocate_elements_back_component_array(ComponentArray* array, unsigned add_count);
