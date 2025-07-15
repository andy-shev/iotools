# About

The *iotools* package provides a set of simple command line tools which allow
access to hardware device registers. Supported register interfaces include PCI,
IO, memory mapped IO, SMBus, CPUID, and MSR. Also included are some utilities
which allow for simple arithmetic, logical, and other operations.

If you ever have to debug hardware, you could probably use these tools.

# Building

*iotools* uses CMake[^1] to build, but has only been tested to run and build on
Linux. Additionally, it is assumed that a `gcc` or `clang` toolchain is being
used. Building *iotools* is very simple, as shown below.

[^1]: https://cmake.org/

## In-Tree Build

```
cmake .
make install
```

## Out-of-Tree Build

```
cmake -B /build/output/path .
make -C /build/output/path install
```

## Configuration Environment

* `CC` - This environment variable, during initial configuration, can be used to
select a toolchain other than the system default.

## Configuration Options

There are a few options that can be used to configure the build. They can be
set using any of CMake's configuration mechanisms: command-line (`cmake`),
ncurses terminal UI (`ccmake`), or graphical UI (`cmake-gui`).

* `CMAKE_BUILD_TYPE` - If set to *Debug*, or unset, debugging symbols will be
generated in the binary and compiler optimizations are disabled. If set to
*Release*, only compiler optimizations are enabled.

* `CMAKE_INSTALL_PREFIX` - Defaults to /usr/local.

* `INSTALL_LINKS` - If set to ON (or 1, etc.), command symlinks to the binary
are created during install. This is the default.

* `STATIC` - If set to ON (or 1, etc.), the binary is linked statically. This is
the default.

## Build Environment

* `DESTDIR` - This `make` variable can be used for staged installs[^2].

[^2]: https://www.gnu.org/prep/standards/html_node/DESTDIR.html


# Usage

Depending on environmental constraints, any of the commands provided by
*iotools* can be accessed either implicitly using the installed symlinks or
explicitly through the `iotools` binary.

## Help Output

```
Usage: iotools COMMAND [ARGUMENTS]
   or: iotools OPTION
   or: COMMAND [ARGUMENTS]

OPTIONS:
    --make-links        create command symlinks and exit
    --list-cmds         list available commands and exit
    --help              print this message and exit
    -v, --version       print version and exit

COMMANDS:
    See output of --list-cmds
```
