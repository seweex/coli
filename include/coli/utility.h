#ifndef COLI_UTILITY_H
#define COLI_UTILITY_H

#if COLI_DYNAMIC && _WIN32
    #if COLI_BUILD
        #define COLI_EXPORT __declspec(dllexport)
    #else
        #define COLI_EXPORT __declspec(dllimport)
    #endif
#else
    #define COLI_EXPORT
#endif

#include <memory>
#include <vector>
#include <concepts>
#include <algorithm>
#include <typeindex>

#include <entt/entt.hpp>

#endif
