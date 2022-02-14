## Instructions ACE ASE Interface 

This is the `debug` version of ACEinterfaces. 

This folder contains an ASE calculator to evaluate `ACE1.jl` potentials from python : 

The relevant files for debugging the C-Julia interface are:
* `ace/ace_c.h` and `ace/ace_c.c` : the C interface layer 
* `test_ace.c` is an executable that can reproduce the segfaults
* `Makefile` can be used to compile the project
Other files are not necessary for debugging the C-Julia embedding
* `ace/ase.py` : the python ASE calculator wrapping the C interface 
* `test_ase_calc.py` : a short test code

## Installation 

1. Compile the C interface, modifying the following commands as suitable: 
```
cd ace
export JULIA_DIR=/PATH/TO/JULIA/FOLDER
make
```
where `/PATH/TO/JULIA/FOLDER` refers to something like `~/Applications/julia-1.7.1` (the main folder of julia, not the `bin` folder of the julia executable)

Then you can simply run the `test_ace` executable. If the `for` loop is iterated over 100,000 times there is usually no problem, but if it is increased to `1,000,000` iterations a segfault will appear during running the code. 

If you do not have `ACE1.jl` set up just create a fresh julia environment with all the necessary packages installed by executing:

```julia
using Pkg; Pkg.activate("."); pkg"registry add https://github.com/JuliaRegistries/General"; pkg"registry add https://github.com/JuliaMolSim/MolSim.git"; pkg"add JuLIP ACE1"
```