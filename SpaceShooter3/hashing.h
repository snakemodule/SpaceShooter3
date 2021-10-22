#pragma once
#include "my_pch.h"

//https://stackoverflow.com/questions/37007307/fast-hash-function-for-stdvector


template <class T>
inline void hash_combine(std::size_t& seed, T const& v)
{
    seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}