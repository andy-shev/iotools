cmake_minimum_required(VERSION 3.16)

project(iotools
	VERSION 1.8
	LANGUAGES C)

include(GNUInstallDirs)

# Default to a Debug build for single-config generators, but keep it a
# regular cache entry so it can be changed from ccmake/cmake-gui.
get_property(is_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if (NOT is_multi_config AND NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
		Debug Release RelWithDebInfo MinSizeRel)
endif()

option(STATIC "Build statically-linked iotools?" ON)
option(INSTALL_LINKS "Generate symlinks during install?" ON)

# Override implicit "-g"
set(CMAKE_C_FLAGS_DEBUG "-O0 -ggdb")

# Override implicit "-O3 -DNDEBUG"
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")

set(COMMAND_PREFIX CMD:)
configure_file(config.h.in config.h)

add_executable(iotools
	cmos_rw.c
	commands.c
	io_rw.c
	iotools.c
	logic.c
	misc.c
	mmio_rw.c
	msr.c
	pci_rw.c
	scom.c
	smbus_rw.c
)

target_include_directories(iotools BEFORE PRIVATE ${PROJECT_BINARY_DIR})
target_compile_definitions(iotools PRIVATE _GNU_SOURCE)
target_compile_options(iotools PRIVATE $<$<CONFIG:Debug>:-Wall -Werror>)

if (STATIC)
	target_link_options(iotools PRIVATE -static)
endif()

install(TARGETS iotools DESTINATION ${CMAKE_INSTALL_SBINDIR})

# Install symlinks á la `iotools --make-links`, but in a cross-build
# compatible manner: the command names are read from the binary with the
# (cross) toolchain's readelf instead of executing it.
if (INSTALL_LINKS)
	if (NOT CMAKE_READELF)
		message(FATAL_ERROR "INSTALL_LINKS requires readelf, but CMAKE_READELF is not set")
	endif()

	# Configure-time values for the install script; everything else is
	# evaluated at install time so `cmake --install --prefix` and DESTDIR work.
	install(CODE "
		set(SAVED_READELF \"${CMAKE_READELF}\")
		set(SAVED_BINARY \"$<TARGET_FILE:iotools>\")
		set(SAVED_PREFIX \"${COMMAND_PREFIX}\")
		set(SAVED_SBINDIR \"${CMAKE_INSTALL_SBINDIR}\")
	")
	install(CODE [[
		if (IS_ABSOLUTE "${SAVED_SBINDIR}")
			set(link_dir "$ENV{DESTDIR}${SAVED_SBINDIR}")
		else()
			set(link_dir "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/${SAVED_SBINDIR}")
		endif()

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
