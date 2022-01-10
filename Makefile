BUILDDIR=build
TARGET=$(BUILDDIR)/libsqplugin.so
SOURCES=sqplugin.c gdnativeUtils.c squeakUtils.c sqMessage.c
OBJECTS=$(addprefix $(BUILDDIR)/,$(SOURCES:.c=.o))
DEPS=$(addprefix $(BUILDDIR)/,$(SOURCES:.c=.d))

CC=gcc
CFLAGS=-std=c11 -Wall 

all: $(TARGET)

$(BUILDDIR)/%.o: %.c
	$(CC) $(CFLAGS) -pthread -fPIC -c -Igodot-headers $< -o $@

liblfqueue.so:
	cd lfqueue && mkdir -p build && cd build && cmake .. && make && cp liblfqueue.so "../.."

$(TARGET): $(OBJECTS) liblfqueue.so
	$(CC) -Wl,--no-as-needed -L. -Wl,-rpath=sqPlugin -lsqueak -rdynamic -llfqueue -shared $(OBJECTS) -o $@

.PHONY: clean
clean:
	rm -rf build liblfqueue.so

$(BUILDDIR)/%.d: %.c
	$(CC) $(CFLAGS) -MM -MG -MF $@ -MP -MT $@ -MT $(<:.c=.o) $<

include $(DEPS)

$(shell mkdir -p $(BUILDDIR))
