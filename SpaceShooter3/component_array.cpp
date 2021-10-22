#include "my_pch.h"
#include "component_array.h"

void erase_with_last_element_component_array(ComponentArray* array, unsigned idx)
{
	if (array->component_size == 0)
		return;

	if (array->count == 0)
		return;
		
	if (array->count - 1 == idx) {
		--array->count;
		return;
	}

	unsigned char* data = array->buffer.data();
	unsigned char* element = data + (static_cast<uint64_t>(idx) * static_cast<uint64_t>(array->component_size));
	unsigned char* last = data + (static_cast<uint64_t>((array->count - 1)) * static_cast<uint64_t>(array->component_size));

	memcpy(element, last, array->component_size);
	--array->count;
}

void allocate_element_back_component_array(ComponentArray* array) {
	if (array->component_size == 0)
		return;
	if ((static_cast<uint64_t>(array->count) + 1) * static_cast<uint64_t>(array->component_size) > array->buffer.size())
	{
		array->buffer.resize(array->buffer.size() * 2 + array->component_size);
	}
	++array->count;
}

void allocate_elements_back_component_array(ComponentArray* array, unsigned add_count) {
	if (array->component_size == 0)
		return;
	if ((static_cast<uint64_t>(array->count) + add_count) * static_cast<uint64_t>(array->component_size) > array->buffer.size())
	{
		array->buffer.resize(array->buffer.size() * 2 + (static_cast<uint64_t>(array->component_size)* static_cast<uint64_t>(add_count)));
	}
	array->count += add_count;
}
