#pragma once
#include <stddef.h>

template <class T, size_t N>
class Array {
public:
    Array() = default;

private:
    T m_data[N];
};