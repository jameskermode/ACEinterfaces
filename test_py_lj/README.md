## Instructions
To compile do 
```
gcc -fPIC -c lj.c
gcc lj.o -shared -o lj.so
```

`ACE_ASE.py` contains an example implementation of an ASE calculator calling a C function
            it passes positions and number of atoms and computes the energy and forces. 
            It could be extended to include the cell, periodic boundary conditions, atomic numbers and virials. 

`test_ase_calc.py` contains a small test of the calculator

`lj.py` was just used to debug the C code of the LJ potential