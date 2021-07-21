CC=gcc

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

ifeq ($(OS),Windows_NT)
	LIB_TYPE := libtest.dll
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		LIB_TYPE := libtest.so
	endif
	ifeq ($(UNAME_S),Darwin)
		LIB_TYPE := libtest.dylib
	endif
endif

test: ./dynamic/$(LIB_TYPE)

./dynamic/libtest.dll: ./static/test.o
	$(CC) -shared -o ./dynamic/libtest.dll -fPIC -I$(JULIA_DIR)/include/julia -L$(JULIA_DIR)/lib \
		-Wl,-rpath,$(JULIA_DIR)/lib \
		-Wl,--subsystem,windows,--out-implib,./dynamic/libtest.dll.a \
		-Wl,-export-all-symbols \
		-Wl,-enable-auto-image-base \
		./static/test.o -ljulia

./dynamic/libtest.so: ./static/test.o
	$(CC) -shared -o ./dynamic/libtest.so -fPIC \
		-I$(JULIA_DIR)/include/julia -L$(JULIA_DIR)/lib \
		-Wl,-rpath,$(JULIA_DIR)/lib \
		./static/test.o -ljulia

./dynamic/libtest.dylib: ./static/test.o
	$(CC) -dynamiclib -o ./dynamic/libtest.dylib -fPIC \
		-I$(JULIA_DIR)/include/julia -L$(JULIA_DIR)/lib \
		-Wl,-rpath,$(JULIA_DIR)/lib \
		./static/test.o -ljulia

./static/test.o: ./src/test.c
	$(CC) -c -o ./static/test.o \
	-fPIC -I$(JULIA_DIR)/include/julia \
	-DMKDIR=\"$(mkfile_dir)\" \
	./src/test.c -ljulia