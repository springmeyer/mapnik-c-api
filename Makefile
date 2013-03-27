all: libmapnik_c.dylib runtest.c python

libmapnik_c.dylib:
	clang++ -x c++ -o libmapnik_c.dylib -Wl,-install_name,`pwd`/libmapnik_c.dylib -dynamiclib mapnik_c_api.c `mapnik-config --cflags --libs --ldflags | tr "\n" " "` -lboost_system

runtest.c: libmapnik_c.dylib
	clang++ -x c -o ./test/c-api-test c-api-test.c -L./ -lmapnik_c

test:
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

.PHONY: clean lint test
