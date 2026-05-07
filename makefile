RM=rm -rf

UNAME=$(shell uname)

# On macOS the default `gcc` is Apple Clang, which rejects nested functions
# used by cspec's DSL macros. Auto-detect the newest Homebrew GCC available
# (gcc-13, gcc-14, gcc-15, ...) unless the user overrides CC explicitly.
ifeq ($(origin CC),default)
ifeq ($(UNAME),Darwin)
CC := $(shell ls -1 /opt/homebrew/bin/gcc-[0-9]* /usr/local/bin/gcc-[0-9]* 2>/dev/null | sort -V | tail -n 1)
ifeq ($(CC),)
$(error Homebrew GCC not found. Install it with 'brew install gcc', or set CC explicitly)
endif
else
CC=gcc
endif
endif

C_SRCS=$(shell find . -iname "*.c" | tr '\n' ' ')
H_SRCS=$(shell find . -iname "*.h" | tr '\n' ' ')

OBJS=$(C_SRCS:./%.c=release/%.o)

ifneq ($(shell id -un),root)
SUDO=sudo
endif

# Clean and compile .so
all: release/libcspecs.so

release/cspecs:
	mkdir -p release/cspecs/

release/libcspecs.so: release/cspecs $(OBJS)
ifeq ($(UNAME), Darwin)
	$(CC) -shared -install_name /usr/local/lib/libcspecs.so -o "release/libcspecs.so" $(OBJS)
else
	$(CC) -shared -o "release/libcspecs.so" $(OBJS)
endif

release/cspecs/%.o: cspecs/%.c
	$(CC) -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"

release/cspecs/collections/%.o: cspecs/collections/%.c
	$(CC) -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"

# Add debug parameters and compile
debug: CC += -DDEBUG -g
debug: all

# Clean release files
clean:
	$(RM) release

install: all
ifeq ($(UNAME), Darwin)
	$(SUDO) cp release/libcspecs.so /usr/local/lib
	$(SUDO) ditto $(H_SRCS) /usr/local/include
else
	$(SUDO) cp -u release/libcspecs.so /usr/lib
	$(SUDO) cp --parents -u $(H_SRCS) /usr/include
endif

uninstall:
	rm -f /usr/lib/libcspecs.so
	rm -rf /usr/include/cspecs

.PHONY: all clean install uninstall
