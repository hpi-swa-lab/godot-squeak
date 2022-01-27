BUILDDIR=build
TARGET=$(BUILDDIR)/libsqplugin.so
SOURCES=sqplugin.c gdnativeUtils.c squeakUtils.c sqMessage.c gdnativeSqueakBindings.gen.c
OBJECTS=$(addprefix $(BUILDDIR)/,$(SOURCES:.c=.o))
DEPS=$(addprefix $(BUILDDIR)/,$(SOURCES:.c=.d))

CC=gcc
CFLAGS=-std=c11 -Wall -g

all: $(TARGET)

$(BUILDDIR)/%.o: %.c
	$(CC) $(CFLAGS) -pthread -fPIC -c -Igodot-headers $< -o $@

liblfqueue.so:
	cd lfqueue && mkdir -p build && cd build && cmake .. && make && cp liblfqueue.so "../../build"

$(TARGET): $(OBJECTS) liblfqueue.so
	$(CC) $(CFLAGS) -Wl,--no-as-needed -L. -Lbuild -Wl,-rpath=sqPlugin -lsqueak -llfqueue -rdynamic -shared $(OBJECTS) -o $@

.PHONY: clean
clean:
	rm -rf build

$(BUILDDIR)/%.d: %.c
	$(CC) $(CFLAGS) -MM -MG -MF $@ -MP -MT $@ -MT $(<:.c=.o) $<

include $(DEPS)

$(shell mkdir -p $(BUILDDIR))
