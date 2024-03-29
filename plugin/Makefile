SOURCEDIR=src
BUILDDIR=build
SCRIPTSDIR=scripts
TARGET=$(BUILDDIR)/libsqplugin.so
GDNATIVE_BINDINGS=gdnativeSqueakBindings.gen.c
SOURCE_NAMES=sqplugin.c gdnativeUtils.c squeakUtils.c sqMessage.c $(GDNATIVE_BINDINGS)
SOURCES=$(addprefix $(SOURCEDIR)/,$(SOURCE_NAMES))
OBJECTS=$(addprefix $(BUILDDIR)/,$(SOURCE_NAMES:.c=.o))
DEPS=$(addprefix $(BUILDDIR)/,$(SOURCE_NAMES:.c=.d))

$(info ${DEPS})

CC=gcc
CFLAGS=-std=c11 -Wall -g
INC=-Ithirdparty -Igodot-headers

all: $(TARGET)

# requires a symlink to run.sh called smalltalkci (https://github.com/hpi-swa/smalltalkCI)
$(SOURCEDIR)/$(GDNATIVE_BINDINGS):
	SPEC_FILE="${CURDIR}/godot-headers/gdnative_api.json" OUTPUT_FILE="${CURDIR}/$@" smalltalkci --headful -s "Squeak64-5.3" $(SCRIPTSDIR)/generateGDNativeCBindingsConfig.ston

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c
	$(CC) $(CFLAGS) -pthread -fPIC -c $(INC) $< -o $@

$(BUILDDIR)/liblfqueue.so:
	cd thirdparty/lfqueue && mkdir -p build && cd build && cmake .. && cmake --build . && cp liblfqueue.so $(CURDIR)/$(BUILDDIR)

$(BUILDDIR)/libsqueak.so:
	cd thirdparty/opensmalltalk-vm && ./compile && cp opensmalltalk-vm/builddir/libsqueak.so $(CURDIR)/$@

$(TARGET): $(OBJECTS) $(BUILDDIR)/liblfqueue.so $(BUILDDIR)/libsqueak.so
	$(CC) $(CFLAGS) -Wl,--no-as-needed -L. -Lbuild -Wl,-rpath=sqPlugin -lsqueak -llfqueue -rdynamic -shared $(OBJECTS) -o $@

.PHONY: clean
clean:
	rm -rf $(BUILDDIR) $(SOURCEDIR)/$(GDNATIVE_BINDINGS)

$(BUILDDIR)/%.d: $(SOURCEDIR)/%.c
	$(CC) $(CFLAGS) -MM -MG -MF $@ -MP -MT $@ -MT $(<:.c=.o) $<

include $(DEPS)

$(shell mkdir -p $(BUILDDIR))
