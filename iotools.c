/*
 Copyright 2008 Google Inc.

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "commands.h"
#include "config.h"

int
main(int argc, const char *argv[])
{
	int rc = run_command(argc, argv);
	exit((rc < 0) ? EXIT_FAILURE : EXIT_SUCCESS);
}

static void
usage(const char *bin_name, FILE *fstream)
{
	fprintf(fstream, "Usage: %s COMMAND [ARGUMENTS]\n", bin_name);
	fprintf(fstream, "   or: %s OPTION\n", bin_name);
	fprintf(fstream, "   or: COMMAND [ARGUMENTS]\n"
			"\nOPTIONS:\n"
			"    --make-links\tcreate command symlinks and exit\n"
			"    --list-cmds\t\tlist available commands and exit\n"
			"    --help\t\tprint this message and exit\n"
			"    -v, --version\tprint version and exit\n"
			"\nCOMMANDS:\n"
			"    See output of --list-cmds\n");
}

static void
version(const char *progname)
{
	printf("%s version %d.%d\n", progname, VER_MAJOR, VER_MINOR);
}

int
iotools_fallback(int argc, const char *argv[])
{
	if (argc != 2) {
		usage(argv[0], stderr);
		return -1;
	}

	if (strcmp(argv[1], "--help") == 0) {
		usage(argv[0], stdout);
		return 0;
	}

	if (strcmp(argv[1], "--make-links") == 0) {
		return make_command_links();
	}

	if (strcmp(argv[1], "--clean-links") == 0) {
		return clean_command_links();
	}

	if (strcmp(argv[1], "--list-cmds") == 0) {
		return list_commands();
	}

	if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
		version(argv[0]);
		return 0;
	}

	fprintf(stderr, "'%s' sub-command not supported by iotools\n", argv[1]);
	usage(argv[0], stderr);
	return -1;
}
