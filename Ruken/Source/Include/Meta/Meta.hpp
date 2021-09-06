
#pragma once

// Meta-programing header

// Glues 2 value together
#define RUKEN_GLUE(a, b) __RUKEN_GLUE(a, b)
#define __RUKEN_GLUE(a, b) a ## b

// Converts a value to string
#define RUKEN_STRING(a) __RUKEN_STRING(a)
#define __RUKEN_STRING(a) # a

// Expands a value
#define RUKEN_EXPAND(...) __VA_ARGS__

// Extracts the first value off of a variadic pack
#define RUKEN_EXTRACT_FIRST(value, ...) value
#define RUKEN_EXTRACT_EXCEPT_FIRST(value, ...) __VA_ARGS__

// Selects a value based on the number of passed variadic arguments
#define RUKEN_SELECT_VARIADIC(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, NAME, ...) NAME

// GET_ARG_COUNT macro definitions, 70 args max are supported
#define RUKEN_GET_ARG_COUNT(...) RUKEN_INTERNAL_EXPAND_ARGS_PRIVATE(RUKEN_INTERNAL_ARGS_AUGMENTER(__VA_ARGS__))
#define RUKEN_INTERNAL_ARGS_AUGMENTER(...) unused, __VA_ARGS__
#define RUKEN_INTERNAL_EXPAND_ARGS_PRIVATE(...) RUKEN_EXPAND(RUKEN_SELECT_VARIADIC(__VA_ARGS__, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))