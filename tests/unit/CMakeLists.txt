
find_package(GTest REQUIRED)
add_executable(cog2d_tests_unit
	ext.cpp)
target_link_libraries(cog2d_tests_unit PRIVATE cog2d GTest::gtest GTest::gtest_main)
target_sources(cog2d_tests_unit PRIVATE
	vector_test.cpp
)
target_compile_definitions(cog2d_tests_unit PUBLIC COG2D_NUM_COLGROUPS=4)
target_compile_definitions(cog2d_tests_unit PUBLIC COG2D_NUM_ASSET_COLLECTIONS=1)

include(CTest)
include(GoogleTest)
gtest_discover_tests(cog2d_tests_unit)
