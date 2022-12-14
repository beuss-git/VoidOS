#pragma once

template <class T>
const T& min(const T& a, const T& b) {
    return a < b ? a : b;
}

template <class T>
const T& max(const T& a, const T& b) {
    return a > b ? a : b;
}
template <class T>
const T& clamp(const T& value, const T& min, const T& max) {
    return value < min ? min : (value > max ? max : value);
}
