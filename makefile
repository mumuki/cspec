RM=rm -rf
CC=gcc

C_SRCS=$(shell find . -iname "*.c" | tr '\n' ' ')
H_SRCS=$(shell find . -iname "*.h" | tr '\n' ' ')

OBJS=$(C_SRCS:./%.c=release/%.o)

UNAME=$(shell uname)

ifneq ($(shell id -un),root)
SUDO=sudo
endif

# Clean and compile .so
all: release/libcspecs.so

release/cspecs:
	mkdir -p release/cspecs/

release/libcspecs.so: release/cspecs $(OBJS)
	$(CC) -shared -o "release/libcspecs.so" $(OBJS)

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
	$(SUDO) cp release/libcspecs.so /usr/lib
	$(SUDO) ditto $(H_SRCS) /usr/include
else
	$(SUDO) cp -u release/libcspecs.so /usr/lib
	$(SUDO) cp --parents -u $(H_SRCS) /usr/include
endif

uninstall:
	rm -f /usr/lib/libcspecs.so
	rm -rf /usr/include/cspecs

.PHONY: all clean install uninstall
