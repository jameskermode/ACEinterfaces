
CFLAGS = -g -fPIC -I${JULIA_DIR}/include/julia -I.
LDFLAGS = -Wl,-rpath,${JULIA_DIR}/lib -L${JULIA_DIR}/lib -ljulia 

all: ace_c.so test_ace

clean:
	rm *.o *.so test_ace

%.o: %.c
	gcc ${CFLAGS} -c $< -o $@

ace_c.o: ace_c.h

ace_c.so: ace_c.o
	gcc $< ${LDFLAGS} -shared -o $@

test_ace: test_ace.o ace_c.o
	gcc $^ ${LDFLAGS} -o $@
