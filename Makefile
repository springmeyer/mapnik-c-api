all: libmapnik_c.dylib

MAPNIK_CXXFLAGS:=$(shell mapnik-config --cflags)
MAPNIK_LDFLAGS:=$(shell mapnik-config --libs)

libmapnik_c.dylib: mapnik_c_api.c mapnik_c_api.h
	clang++ -x c++ -o libmapnik_c.dylib -Wl,-install_name,`pwd`/libmapnik_c.dylib -dynamiclib mapnik_c_api.c $(MAPNIK_CXXFLAGS) $(MAPNIK_LDFLAGS) -Wl,-undefined -Wl,dynamic_lookup -Wl,-dead_strip -fvisibility-inlines-hidden

test/c-api-test: libmapnik_c.dylib
	clang++ -x c -o ./test/c-api-test c-api-test.c -L./ -lmapnik_c

test: test/c-api-test
	@echo "Testing C API.."
	./test/c-api-test

check:
	./test/c-api-test

clean:
	rm -rf ./libmapnik_c.dylib*
	rm -rf ./test/c-api-test
	rm -f ./py-api/*pyc

lint:
	CC=scan-build $(MAKE)

grind:
	valgrind --leak-check=full --log-fd=1 ./test/c-api-test;

node:
	ffi-generate -f mapnik_c_api.h -l libmapnik_c > index.js
	node test/node-api-test.js

python:
	mkdir -p py-api
	touch py-api/__init__.py
	ctypesgen.py -a -llibmapnik_c mapnik_c_api.h -o py-api/mapnik_c.py
	python test/py-api-test.py;open py-test.png

lua:
	luajit test/lua-api-test.lua;open lua-test.png

.PHONY: clean lint test
