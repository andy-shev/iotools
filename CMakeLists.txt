cmake_minimum_required(VERSION 3.0)
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
