all: build/libsmplugin.so

build/smplugin.o: smplugin.c
	gcc -std=c11 -pthread -fPIC -c -Igodot-headers smplugin.c -o build/smplugin.o

build/libsmplugin.so: build/smplugin.o
	gcc -Wl,--no-as-needed -L. -Wl,-rpath=sqPlugin -lsqueak -rdynamic -shared build/smplugin.o -o build/libsmplugin.so

.PHONY: clean
clean:
	rm -rf build

$(shell mkdir -p build)
