# About

The *iotools* package provides a set of simple command line tools which allow
access to hardware device registers. Supported register interfaces include PCI,
IO, memory mapped IO, SMBus, CPUID, and MSR. Also included are some utilities
which allow for simple arithmetic, logical, and other operations.

If you ever have to debug hardware, you could probably use these tools.

# Building

*iotools* currently does not use an autoconf script or anything similar to build.
*iotools* has only been tested to run and build on Linux. Additionally, it is
assumed that `gcc` is being used to build. In order to build *iotools*, one just
has to type `make`.

There are a few options that can be used during the build.

* `DEBUG` - If set to 1, debugging symbols will be generated in the binaries. The
build currently turns this option on if the `DEBUG` variable has not been
assigned.

* `STATIC` - If set to 1, the binary is linked statically. The build currently
turns this option on if the `STATIC` variable has not been assigned.

* `CROSS_COMPILE` - This variable can be set to a path of another gcc. The build
will use the compiler pointed to by `CROSS_COMPILE`.

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
