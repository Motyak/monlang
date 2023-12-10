CFLAGS_RELEASE = -c --std=c++17 -Wall -Wextra -O0
CFLAGS_DEBUG = -c --std=c++17 -Wall -Wextra -Og -g


# make (default builds release)
all: release

# make release
release: bin/release/monlang

# make debug
debug: bin/debug/monlang

# make clean
clean:
	rm -rf obj

# make mrproper
mrproper:
	rm -rf obj bin


##########################
# RELEASE
##########################

# compiles monlang in release
obj/release/monlang.o: src/monlang.cpp
	g++ $(CFLAGS_RELEASE) $^ -o $@

# link object files and build binary for monlang in release
bin/release/monlang: obj/release/monlang.o
	g++ $^ -o $@

##########################
# RELEASE (end)
##########################


##########################
# DEBUG
##########################

# compiles monlang in debug
obj/debug/monlang.o: src/monlang.cpp
	g++ $(CFLAGS_DEBUG) $^ -o $@

# link object files and build binary for monlang in debug
bin/debug/monlang: obj/debug/monlang.o
	g++ $^ -o $@

##########################
# DEBUG (end)
##########################


.PHONY: all release debug clean mrproper

# will create all necessary directories after the Makefile is parsed
$(shell mkdir -p obj/release obj/debug bin/release bin/debug)
