all: libmapnik_c.dylib

# inherit from env
CXX := $(CXX)
CXXFLAGS := $(CXXFLAGS)
LDFLAGS := $(LDFLAGS)

# mapnik settings
MAPNIK_CXXFLAGS := $(shell mapnik-config --cflags)
MAPNIK_LDFLAGS := $(shell mapnik-config --libs)
MAPNIK_PLUGINDIR := $(shell mapnik-config --input-plugins)

OS:=$(shell uname -s)
ifeq ($(OS),Darwin)
	SHARED_FLAG = -dynamiclib
	LIBNAME = libmapnik_c.dylib
	LDFLAGS += -Wl,-install_name,`pwd`/libmapnik_c.dylib -Wl,-undefined -Wl,dynamic_lookup
else
	SHARED_FLAG = -shared
	LIBNAME = libmapnik_c.so
	LDFLAGS += -Wl,-rpath=. -fPIC
endif

libmapnik_c.dylib: mapnik_c_api.c mapnik_c_api.h Makefile
	$(CXX) -x c++ -o $(LIBNAME) $(SHARED_FLAG) mapnik_c_api.c $(LDFLAGS) $(MAPNIK_CXXFLAGS) $(MAPNIK_LDFLAGS)

test/c-api-test-cfg.h:
	echo "#define MAPNIK_PLUGINDIR \"$(MAPNIK_PLUGINDIR)\"" > test/c-api-test-cfg.h

test/c-api-test: libmapnik_c.dylib test/c-api-test.cpp test/c-api-test-cfg.h
	$(CXX) -x c++ -o ./test/c-api-test test/c-api-test.cpp $(LDFLAGS) -L./ -I./ -lmapnik_c

test: test/c-api-test
	@./test/c-api-test

check:
	./test/c-api-test

clean:
	rm -rf ./libmapnik_c.dylib* ./libmapnik_c.so*
	rm -rf ./test/c-api-test test/c-api-test-cfg.h
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

ruby:
	ruby test/rb-api-test.rb;open rb-test.png

jruby:
	jruby -S test/rb-api-test.rb;open rb-test.png

.PHONY: test clean lint
