add_library (gtest gtest-all.cc gtest_main.cc)
set_target_properties(gtest
	PROPERTIES
	CXX_STANDARD 11
	INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/.."
	COMPILE_OPTIONS "-O2;-pthread;-Wall;-Wextra;-std=gnu++11"
	LINK_FLAGS "-O2 -pthread")
