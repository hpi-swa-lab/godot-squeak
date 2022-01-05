BUILDDIR=build
TARGET=$(BUILDDIR)/libsmplugin.so
SOURCES=smplugin.c gdnativeUtils.c squeakUtils.c
OBJECTS=$(addprefix $(BUILDDIR)/,$(SOURCES:.c=.o))
DEPS=$(addprefix $(BUILDDIR)/,$(SOURCES:.c=.d))

CC=gcc
CFLAGS=-std=c11 -Wall 

all: $(TARGET)

$(BUILDDIR)/%.o: %.c
	$(CC) $(CFLAGS) -pthread -fPIC -c -Igodot-headers $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) -Wl,--no-as-needed -L. -Wl,-rpath=sqPlugin -lsqueak -rdynamic -shared $(OBJECTS) -o $@

.PHONY: clean
clean:
	rm -rf build

$(BUILDDIR)/%.d: %.c
	$(CC) $(CFLAGS) -MM -MG -MF $@ -MP -MT $@ -MT $(<:.c=.o) $<

include $(DEPS)

$(shell mkdir -p $(BUILDDIR))
