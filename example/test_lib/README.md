# Example C-ACE

This example shows how one could build ACE wrapper in C.

## Setup

### Windows

1. Install julia and [ACE.jl](https://github.com/ACEsuit/ACE.jl/tree/dev-v0.8.x)

2. Set path to julia directory by running the following in bash

```bash
set JULIA_DIR=(path to Julia directory)
```

3. Run makefile

4. Add path to dynamic folder to <code>PATH</code> system enviromental variables

5. When compiling executable link dynamic library and include header file using the following command

```bash
gcc -L../test_lib/dynamic -I../test_lib/include -Wl,-rpath,../test_lib/dynamic  main.c -ltest
```

