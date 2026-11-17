# --- Enabling and adding testing dependencies
enable_testing()
CPMAddPackage(URL "https://github.com/google/googletest/archive/refs/tags/v1.18.0.zip" NAME googletest)
include(GoogleTest)

add_compile_options(-fsanitize=address -fsanitize=undefined)
add_link_options   (-fsanitize=address -fsanitize=undefined)

# ---
add_subdirectory(${RUKEN_TESTS_DIR}/Core)