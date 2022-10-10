#pragma once
#include <stdint.h>

// https://misc.flogisoft.com/bash/tip_colors_and_formatting
namespace Shared::Bash {
    using BashColor = const char*;
    namespace Color {
        static constexpr BashColor Reset = "\033[0m";
        static constexpr BashColor Default = "\033[39m";
        static constexpr BashColor Black = "\033[30m";
        static constexpr BashColor Red = "\033[31m";
        static constexpr BashColor Green = "\033[32m";
        static constexpr BashColor Yellow = "\033[33m";
        static constexpr BashColor Blue = "\033[34m";
        static constexpr BashColor Magenta = "\033[35m";
        static constexpr BashColor Cyan = "\033[36m";
        static constexpr BashColor LightGrey = "\033[37m";
        static constexpr BashColor DarkGrey = "\033[90m";
        static constexpr BashColor LightRed = "\033[91m";
        static constexpr BashColor LightGreen = "\033[92m";
        static constexpr BashColor LightYellow = "\033[93m";
        static constexpr BashColor LightBlue = "\033[94m";
        static constexpr BashColor LightMagenta = "\033[95m";
        static constexpr BashColor LightCyan = "\033[96m";
        static constexpr BashColor White = "\033[97m";
    }
}
