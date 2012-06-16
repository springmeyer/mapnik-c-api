all: libmapnik_c.dylib runtest.c

libmapnik_c.dylib:
	clang++ -x c++ -o libmapnik_c.dylib -dynamiclib mapnik_c_api.c `mapnik-config --cflags --libs`

runtest.c: libmapnik_c.dylib
	clang++ -x c++ -o runtest c-api-test.c -L./ -lmapnik_c
	./runtest

test:
	./runtest

check:
	./runtest

clean:
	rm libmapnik_c.dylib
	rm runtest

lint:
	CC=scan-build $(MAKE)

grind:
	valgrind --leak-check=full --log-fd=1 ./runtest | grep definitely;

.PHONY: clean lint
