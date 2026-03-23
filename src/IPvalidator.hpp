#pragma once

#include <string>

#include "GetIPv4IPv6.hpp"

inline IP validIP(const std::string &ipAddress) {
#ifdef _WIN32
    IN_ADDR ipv4Buffer{};
    IN6_ADDR ipv6Buffer{};
#else
    in_addr ipv4Buffer{};
    in6_addr ipv6Buffer{};
#endif

    if (inet_pton(AF_INET, ipAddress.c_str(), &ipv4Buffer) == 1) {
        return IPv4;
    }

    if (inet_pton(AF_INET6, ipAddress.c_str(), &ipv6Buffer) == 1) {
        return IPv6;
    }

    return Invalid;
}
