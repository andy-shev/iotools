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
if ($<CONFIG:Debug)
	target_compile_options(iotools PRIVATE -Wall -Werror)
endif()

if (STATIC)
	set_target_properties(iotools PROPERTIES LINK_FLAGS -static)
endif()

add_definitions(-D_GNU_SOURCE)

install(TARGETS iotools DESTINATION sbin)

# Install symlinks á la `iotools --make-links`, but in a cross-build
# compatible manner.
if (INSTALL_LINKS)
	install(CODE "\
		function(make_links install_dir bin)
			execute_process(
				COMMAND ${CMAKE_READELF} --string-dump .rodata \${bin}
				OUTPUT_VARIABLE rodata)

			# Gather list of tagged commands
			string(REGEX MATCHALL \"${COMMAND_PREFIX}[0-9a-z_]+\" cmdlist \${rodata})

			foreach(cmdtag IN LISTS cmdlist)
				# Drop tagging prefix
				string(REPLACE ${COMMAND_PREFIX} \"\" cmdname \${cmdtag})
				message(STATUS \"Symlink: \${cmdname} -> iotools\")
				execute_process(
					COMMAND ${CMAKE_COMMAND} -E create_symlink
						iotools \${install_dir}/\${cmdname})
			endforeach(cmdtag)
		endfunction(make_links)

		make_links(\$ENV{DESTDIR}/${CMAKE_INSTALL_PREFIX}/sbin ${PROJECT_BINARY_DIR}/iotools)
	")
endif()
