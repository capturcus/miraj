#pragma once

#ifndef COLOREDCHAR_HPP
#define COLOREDCHAR_HPP

#include <cstdint>
#include <uchar.h>

template<typename Char>
struct coloredChar_t
{
    Char code;
    uint8_t color;
};

using coloredChar8_t = coloredChar_t<char>;
using coloredChar16_t = coloredChar_t<char16_t>;
using coloredChar32_t = coloredChar_t<char32_t>;

#endif // MIRAJ_COLOREDCHAR_HPP

