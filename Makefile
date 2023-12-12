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

# compiles parse-functions.cpp in release
obj/release/parse-functions.o: src/parse-functions.cpp
	g++ $(CFLAGS_RELEASE) -I include $^ -o $@

# compiles monlang.cpp in release
obj/release/monlang.o: src/monlang.cpp
	g++ $(CFLAGS_RELEASE) -I include $^ -o $@

# link object files and build binary for monlang in release
bin/release/monlang: obj/release/parse-functions.o obj/release/monlang.o
	g++ $^ -o $@

##########################
# RELEASE (end)
##########################


##########################
# DEBUG
##########################

# compiles parse-functions.cpp in release
obj/debug/parse-functions.o: src/parse-functions.cpp
	g++ $(CFLAGS_DEBUG) -I include $^ -o $@

# compiles monlang.cpp in release
obj/debug/monlang.o: src/monlang.cpp
	g++ $(CFLAGS_DEBUG) -I include $^ -o $@

# link object files and build binary for monlang in release
bin/debug/monlang: obj/release/parse-functions.o obj/release/monlang.o
	g++ $^ -o $@

##########################
# DEBUG (end)
##########################


.PHONY: all release debug clean mrproper

# will create all necessary directories after the Makefile is parsed
$(shell mkdir -p obj/release obj/debug bin/release bin/debug)
