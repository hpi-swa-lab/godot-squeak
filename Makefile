all: libsmplugin.so

smplugin.o: smplugin.c
	gcc -std=c11 -pthread -fPIC -c -Igodot-headers smplugin.c -o smplugin.o

libsmplugin.so: smplugin.o
	gcc -Wl,--no-as-needed -L. -Wl,-rpath=. -lsqueak -rdynamic -shared smplugin.o -o libsmplugin.so

.PHONY: clean
clean:
	rm -rf smplugin.o libsmplugin.so
