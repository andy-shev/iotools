cmake_minimum_required(VERSION 3.16)

project(iotools
	VERSION 1.8
	LANGUAGES C)

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

option(STATIC "Build statically-linked iotools?" ON)
option(INSTALL_LINKS "Generate symlinks during install?" ON)

# Override implicit "-g"
set(CMAKE_C_FLAGS_DEBUG "-O0 -ggdb")

# Override implicit "-O3 -DNDEBUG"
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")

file(GLOB SRCS *.c)

set(COMMAND_PREFIX CMD:)
configure_file(config.h.in config.h)
include_directories(BEFORE ${PROJECT_BINARY_DIR})

add_executable(iotools ${SRCS})
target_compile_options(iotools PRIVATE $<$<CONFIG:Debug>:-Wall -Werror>)

if (STATIC)
	set_target_properties(iotools PROPERTIES LINK_FLAGS -static)
endif()

add_definitions(-D_GNU_SOURCE)

install(TARGETS iotools DESTINATION sbin)

# Install symlinks á la `iotools --make-links`, but in a cross-build
# compatible manner: the command names are read from the binary with the
# (cross) toolchain's readelf instead of executing it.
if (INSTALL_LINKS)
	# Configure-time values for the install script; everything else is
	# evaluated at install time so `cmake --install --prefix` and DESTDIR work.
	install(CODE "
		set(SAVED_READELF \"${CMAKE_READELF}\")
		set(SAVED_BINARY \"$<TARGET_FILE:iotools>\")
		set(SAVED_PREFIX \"${COMMAND_PREFIX}\")
	")
	install(CODE [[
		set(link_dir "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sbin")

		execute_process(
			COMMAND "${SAVED_READELF}" --string-dump .rodata "${SAVED_BINARY}"
			OUTPUT_VARIABLE rodata
			RESULT_VARIABLE rc)
		if (NOT rc EQUAL 0)
			message(FATAL_ERROR "Failed to read command list from ${SAVED_BINARY}")
		endif()

		# Gather list of tagged commands
		string(REGEX MATCHALL "${SAVED_PREFIX}[0-9a-z_]+" cmdlist "${rodata}")
		list(REMOVE_DUPLICATES cmdlist)

		foreach(cmdtag IN LISTS cmdlist)
			# Drop tagging prefix
			string(REPLACE "${SAVED_PREFIX}" "" cmdname "${cmdtag}")
			message(STATUS "Symlink: ${link_dir}/${cmdname} -> iotools")
			file(CREATE_LINK iotools "${link_dir}/${cmdname}" SYMBOLIC)
		endforeach()
	]])
endif()
