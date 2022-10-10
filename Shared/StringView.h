#pragma once
#include <stddef.h>
#include <sys/types.h>

class StringView {
public:
    StringView(const char* characters)
        : m_data(characters)
        , m_size(strlen(characters))
    {
    }
    const char* data() const { return m_data; }

private:
    const char* m_data;
    size_t m_size;
};