# --- Enabling and adding testing dependencies
enable_testing()
CPMAddPackage(URL "https://github.com/google/googletest/archive/refs/tags/v1.18.0.zip" NAME googletest)
include(GoogleTest)

# ---
add_subdirectory(${RUKEN_TESTS_DIR}/Core)