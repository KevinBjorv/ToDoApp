#pragma once
#include <string>

namespace color {
    // Reset
    const std::string reset = "\033[0m";

    // Regular Colors
    const std::string black = "\033[30m";
    const std::string red = "\033[31m";
    const std::string green = "\033[32m";
    const std::string yellow = "\033[33m";
    const std::string blue = "\033[34m";
    const std::string magenta = "\033[35m";
    const std::string cyan = "\033[36m";
    const std::string white = "\033[37m";

    // Bold Colors
    const std::string bold_black = "\033[1;30m";
    const std::string bold_red = "\033[1;31m";
    const std::string bold_green = "\033[1;32m";
    const std::string bold_yellow = "\033[1;33m";
    const std::string bold_blue = "\033[1;34m";
    const std::string bold_magenta = "\033[1;35m";
    const std::string bold_cyan = "\033[1;36m";
    const std::string bold_white = "\033[1;37m";

    const std::string underline_white = "\033[4;37m";
}

// Path: Colors.h