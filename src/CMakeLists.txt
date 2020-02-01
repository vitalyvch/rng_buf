##
# @author Vitalii Chernookyi
#                           
# @copyright BSD

add_library(rng_buf STATIC
	rng_buf.c)
set_target_properties(rng_buf PROPERTIES
	PREFIX ""
	C_STANDARD 99
	COMPILE_OPTIONS "-g;-O2;-pthread;-Wall;-Wextra;-std=gnu99"
	LINK_FLAGS "-g -O2 -pthread")

target_include_directories(rng_buf PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/..)

################################
# GTest
################################
#include(GoogleTest)
enable_testing()
include_directories(${gtest_SOURCE_DIR})

################################
# Unit Tests
################################
# Add test cpp file
add_executable( rng_buf_tests rng_buf_tests.cc)
target_link_libraries(rng_buf_tests gtest rng_buf rt)
target_include_directories(rng_buf_tests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/..)

set_target_properties(rng_buf_tests
	PROPERTIES
	CXX_STANDARD 11
	COMPILE_OPTIONS "-g;-O2;-pthread;-Wall;-Wextra;-std=gnu++11"
	LINK_FLAGS "-g -O2 -pthread -uRound")

execute_process(COMMAND date "+%F_%T"
	OUTPUT_VARIABLE VALGRIND_TESTS_TIMESTAMP
	OUTPUT_STRIP_TRAILING_WHITESPACE)

#gtest_discover_tests(rng_buf_tests)

add_test( NAME runUnitTests
	COMMAND valgrind
	"--log-file=valgrind/rng_buf_tests.${VALGRIND_TESTS_TIMESTAMP}.valgrind"
	-v
	--read-var-info=yes
	--track-origins=yes
	--show-reachable=yes
	--leak-check=full
	--show-leak-kinds=all
		$<TARGET_FILE:rng_buf_tests> )
