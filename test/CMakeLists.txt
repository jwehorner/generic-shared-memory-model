include(FetchContent)
FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

if (UNIX AND NOT APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
	set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

add_executable(test_generic_shared_memory_model					"${CMAKE_SOURCE_DIR}/test/test_generic_shared_memory_model.cpp")

target_include_directories(test_generic_shared_memory_model 	PUBLIC "${CMAKE_SOURCE_DIR}")

target_link_libraries(test_generic_shared_memory_model			GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(test_generic_shared_memory_model)
