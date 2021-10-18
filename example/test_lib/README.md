# Example C-ACE

This example shows how one could build ACE wrapper in C.

## Setup

1. Install julia and [ACE.jl](https://github.com/ACEsuit/ACE.jl/tree/dev-v0.8.x)

2. Run makefile using the following command

```bash
make JULIA_DIR=(path to Julia directory)
```

3. (Windows only) Add path to dynamic folder to <code>PATH</code> system enviromental variables

4. When compiling executable link dynamic library and include header file using the following command

```bash
gcc -L../test_lib/dynamic -I../test_lib/include -Wl,-rpath,../test_lib/dynamic  main.c -ltest
```
