
Compile using some suitable variant of 
```
export JULIA_DIR=/Users/ortner/gits/julia16/usr
gcc -o test -fPIC -I$JULIA_DIR/include/julia -L$JULIA_DIR/lib -Wl,-rpath,$JULIA_DIR/lib test.c -ljulia
```
