## Instructions ACE ASE Interface 

This folder contains: 
* `ace_c.h` and `ace_c.c` : the C interface layer 
* `ase.py` : the python package wrapping the C interface 
* `test_ase_calc.py` : a short test code

## Installation 

1. Compile the C interface, modifying the following commands as suitable: 
```
export JULIA_DIR=/PATH/TO/JULIA/FOLDER
gcc -fPIC -c ace_c.c -I$JULIA_DIR/include/julia 
gcc ace_c.o -L$JULIA_DIR/lib -Wl,-rpath,$JULIA_DIR/lib -ljulia -shared -o ace_c.so
```

2. Run `python setup.py install`

3. Try it out
```
python test_ase_calc.py
```

TODO: probably needs an installation script `setup.py`?