all: libmapnik_c.dylib

libmapnik_c.dylib: mapnik_c_api.c mapnik_c_api.h
	clang++ -x c++ -o libmapnik_c.dylib -Wl,-install_name,`pwd`/libmapnik_c.dylib -dynamiclib mapnik_c_api.c `mapnik-config --cflags --libs --ldflags | tr "\n" " "` -lboost_system

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

.PHONY: clean lint test
