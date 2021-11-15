
#pragma once

#include "Build/Compiler.hpp"

// Because MSVC is such a shitty compiler we need to attach a custom attribute
// to classes inheriting from empty classes in order to enable the 'empty base optimization' (EBO).
// This is reportedly a long standing bug that hasn't been fixed since 2006
// https://stackoverflow.com/questions/12701469/why-is-the-empty-base-class-optimization-ebo-is-not-working-in-msvc
#ifdef RUKEN_COMPILER_MSVC
    #define RUKEN_EMPTY_BASES __declspec(empty_bases)
#else
    #define RUKEN_EMPTY_BASES
#endif
