CPMAddPackage(URL "https://github.com/g-truc/glm/releases/download/1.0.3/glm-1.0.3.zip" NAME glm)

add_executable            (RkCoreTest Maths/TestMaths.cpp Maths/TestVector.cpp Maths/TestMatrix.cpp Maths/TestQuaternion.cpp)
target_compile_definitions(RkCoreTest PRIVATE GLM_ENABLE_EXPERIMENTAL GLM_FORCE_RADIANS GLM_FORCE_DEPTH_ZERO_TO_ONE)
target_link_libraries     (RkCoreTest PRIVATE GTest::gtest_main glm RkCore)
gtest_discover_tests      (RkCoreTest)