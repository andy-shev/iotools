cmake_minimum_required(VERSION 3.0)

# VERSION_GREATER_EQUAL not available until 3.7
if ((CMAKE_VERSION VERSION_EQUAL 3.4) OR (CMAKE_VERSION VERSION_GREATER 3.4))
	# Enable -rdynamic link flag only if ENABLE_EXPORTS is True
	cmake_policy(SET CMP0065 NEW)
endif()

project(iotools
	VERSION 1.8
	LANGUAGES C)

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

option(STATIC "Build statically-linked iotools?" ON)

# Override implicit "-g"
set(CMAKE_C_FLAGS_DEBUG "-O0 -ggdb")

# Override implicit "-O3 -DNDEBUG"
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")

file(GLOB SRCS *.c)
configure_file(config.h.in config.h)

add_executable(iotools ${SRCS})
if ($<CONFIG:Debug)
	target_compile_options(iotools PRIVATE -Wall -Werror)
endif()

if (STATIC)
	set_target_properties(iotools PROPERTIES LINK_FLAGS -static)
endif()

add_definitions(-D_GNU_SOURCE)

install(TARGETS iotools DESTINATION sbin)
