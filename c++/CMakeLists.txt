cmake_minimum_required(VERSION 3.5.2)

project(openwar)

find_package(Boost COMPONENTS filesystem program_options system REQUIRED)

add_executable(mz_header_parser
	mz_header_parser.cpp
	mz_header.hpp mz_header.cpp
	index_list.hpp
)

target_include_directories(mz_header_parser
	PRIVATE ${Boost_INCLUDE_DIRECTORIES}
)

target_link_libraries(mz_header_parser
	PRIVATE ${Boost_LIBRARIES}
)

target_compile_options(mz_header_parser
	PRIVATE "-g3"
	PRIVATE "-std=c++14"
)
