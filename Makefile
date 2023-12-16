CFLAGS_RELEASE = -c --std=c++17 -Wall -Wextra -O0
CFLAGS_DEBUG = -c --std=c++17 -Wall -Wextra -Og -g
CFLAGS_TEST = -c --std=c++17 -Wall -Wextra -Og -g

LIBS_OBJ_DIR := $(foreach lib,$(wildcard lib/*/),$(lib)/obj)

# make (default builds release)
all: release

# make release
release: bin/release/monlang

# make debug
debug: bin/debug/monlang

# make test #
.SECONDEXPANSION:
test: $$(test_binaries)
	for test_binary in bin/test/*; do echo "$$test_binary:"; ./$$test_binary; echo "Exit code: $$?"; done

# make clean
clean:
	rm -rf obj lib/libs.a $(LIBS_OBJ_DIR)

# make mrproper
mrproper:
	rm -rf obj bin lib/libs.a $(LIBS_OBJ_DIR)


############################################################
# monlang
############################################################

# link object files and build binary for monlang in release #
.SECONDEXPANSION:
bin/release/monlang: lib/libs.a $$(release_objects)
	g++ $^ -o $@

# compiles monlang.cpp in release #
release_objects += obj/release/monlang.o
obj/release/monlang.o: src/monlang.cpp
	g++ $(CFLAGS_RELEASE) -I include -I lib $< -o $@

# link object files and build binary for monlang in debug #
.SECONDEXPANSION:
bin/debug/monlang: lib/libs.a $$(debug_objects)
	g++ $^ -o $@

# compiles monlang.cpp in debug #
debug_objects += obj/debug/monlang.o
obj/debug/monlang.o: src/monlang.cpp
	g++ $(CFLAGS_DEBUG) -I include -I lib $< -o $@


############################################################
# Program
############################################################

# link object files and build binary for Program in test
test_binaries += bin/test/Program
bin/test/Program: lib/libs.a obj/debug/Program.o obj/test/Program.o
	g++ $^ -o $@

# compiles Program.cpp in test
obj/test/Program.o: src/test/Program.cpp
	g++ $(CFLAGS_TEST) -I include -I lib $< -o $@

# compiles Program.cpp in release #
release_objects += obj/release/Program.o
obj/release/Program.o: src/Program.cpp
	g++ $(CFLAGS_RELEASE) -I include -I lib $< -o $@

# compiles Program.cpp in debug #
debug_objects += obj/release/Program.o
obj/debug/Program.o: src/Program.cpp
	g++ $(CFLAGS_DEBUG) -I include -I lib $< -o $@


############################################################
# ProgramSentence
############################################################

# link object files and build binary for ProgramSentence in test
test_binaries += bin/test/ProgramSentence
bin/test/ProgramSentence: lib/libs.a obj/debug/ProgramSentence.o obj/test/ProgramSentence.o
	g++ $^ -o $@

# compiles ProgramSentence.cpp in test
obj/test/ProgramSentence.o: src/test/ProgramSentence.cpp
	g++ $(CFLAGS_TEST) -I include -I lib $< -o $@

# compiles ProgramSentence.cpp in release
release_objects += obj/release/ProgramSentence.o
obj/release/ProgramSentence.o: src/ProgramSentence.cpp
	g++ $(CFLAGS_RELEASE) -I include -I lib $< -o $@

# compiles ProgramSentence.cpp in debug
debug_objects += obj/debug/ProgramSentence.o
obj/debug/ProgramSentence.o: src/ProgramSentence.cpp
	g++ $(CFLAGS_DEBUG) -I include -I lib $< -o $@


############################################################
# Term
############################################################

# link object files and build binary for Term in test
test_binaries += bin/test/Term
bin/test/Term: lib/libs.a obj/debug/Term.o obj/test/Term.o
	g++ $^ -o $@

# compiles Term.cpp in test
obj/test/Term.o: src/test/Term.cpp
	g++ $(CFLAGS_TEST) -I include -I lib $< -o $@

# compiles Term.cpp in release
release_objects += obj/release/Term.o
obj/release/Term.o: src/Term.cpp
	g++ $(CFLAGS_RELEASE) -I include -I lib $< -o $@

# compiles Term.cpp in debug
debug_objects += obj/debug/Term.o
obj/debug/Term.o: src/Term.cpp
	g++ $(CFLAGS_DEBUG) -I include -I lib $< -o $@

############################################################
# ProgramWord
############################################################

# link object files and build binary for ProgramWord in test
test_binaries += bin/test/ProgramWord
bin/test/ProgramWord: lib/libs.a obj/debug/ProgramWord.o obj/test/ProgramWord.o
	g++ $^ -o $@

# compiles ProgramWord.cpp in test
obj/test/ProgramWord.o: src/test/ProgramWord.cpp
	g++ $(CFLAGS_TEST) -I include -I lib $< -o $@

# compiles ProgramWord.cpp in release
release_objects += obj/release/ProgramWord.o
obj/release/ProgramWord.o: src/ProgramWord.cpp
	g++ $(CFLAGS_RELEASE) -I include -I lib $< -o $@

# compiles ProgramWord.cpp in debug
debug_objects += obj/debug/ProgramWord.o
obj/debug/ProgramWord.o: src/ProgramWord.cpp
	g++ $(CFLAGS_DEBUG) -I include -I lib $< -o $@

############################################################
# Association
############################################################

# link object files and build binary for Association in test
test_binaries += bin/test/Association
bin/test/Association: lib/libs.a obj/debug/Association.o obj/test/Association.o
	g++ $^ -o $@

# compiles Association.cpp in test
obj/test/Association.o: src/test/Association.cpp
	g++ $(CFLAGS_TEST) -I include -I lib $< -o $@

# compiles Association.cpp in release
release_objects += obj/release/Association.o
obj/release/Association.o: src/Association.cpp
	g++ $(CFLAGS_RELEASE) -I include -I lib $< -o $@

# compiles Association.cpp in debug
debug_objects += obj/debug/Association.o
obj/debug/Association.o: src/Association.cpp
	g++ $(CFLAGS_DEBUG) -I include -I lib $< -o $@

############################################################
# ParenthesesGroup
############################################################

# link object files and build binary for ParenthesesGroup in test
test_binaries += bin/test/ParenthesesGroup
bin/test/ParenthesesGroup: lib/libs.a obj/debug/ParenthesesGroup.o obj/test/ParenthesesGroup.o
	g++ $^ -o $@

# compiles ParenthesesGroup.cpp in test
obj/test/ParenthesesGroup.o: src/test/ParenthesesGroup.cpp
	g++ $(CFLAGS_TEST) -I include -I lib $< -o $@

# compiles ParenthesesGroup.cpp in release
release_objects += obj/release/ParenthesesGroup.o
obj/release/ParenthesesGroup.o: src/ParenthesesGroup.cpp
	g++ $(CFLAGS_RELEASE) -I include -I lib $< -o $@

# compiles ParenthesesGroup.cpp in debug
debug_objects += obj/debug/ParenthesesGroup.o
obj/debug/ParenthesesGroup.o: src/ParenthesesGroup.cpp
	g++ $(CFLAGS_DEBUG) -I include -I lib $< -o $@

############################################################
# SquareBracketsGroup
############################################################

# link object files and build binary for SquareBracketsGroup in test
test_binaries += bin/test/SquareBracketsGroup
bin/test/SquareBracketsGroup: lib/libs.a obj/debug/SquareBracketsGroup.o obj/test/SquareBracketsGroup.o
	g++ $^ -o $@

# compiles SquareBracketsGroup.cpp in test
obj/test/SquareBracketsGroup.o: src/test/SquareBracketsGroup.cpp
	g++ $(CFLAGS_TEST) -I include -I lib $< -o $@

# compiles SquareBracketsGroup.cpp in release
release_objects += obj/release/SquareBracketsGroup.o
obj/release/SquareBracketsGroup.o: src/SquareBracketsGroup.cpp
	g++ $(CFLAGS_RELEASE) -I include -I lib $< -o $@

# compiles SquareBracketsGroup.cpp in debug
debug_objects += obj/debug/SquareBracketsGroup.o
obj/debug/SquareBracketsGroup.o: src/SquareBracketsGroup.cpp
	g++ $(CFLAGS_DEBUG) -I include -I lib $< -o $@

############################################################
# PostfixParenthesesGroup
############################################################

# link object files and build binary for PostfixParenthesesGroup in test
test_binaries += bin/test/PostfixParenthesesGroup
bin/test/PostfixParenthesesGroup: lib/libs.a obj/debug/PostfixParenthesesGroup.o obj/test/PostfixParenthesesGroup.o
	g++ $^ -o $@

# compiles PostfixParenthesesGroup.cpp in test
obj/test/PostfixParenthesesGroup.o: src/test/PostfixParenthesesGroup.cpp
	g++ $(CFLAGS_TEST) -I include -I lib $< -o $@

# compiles PostfixParenthesesGroup.cpp in release
release_objects += obj/release/PostfixParenthesesGroup.o
obj/release/PostfixParenthesesGroup.o: src/PostfixParenthesesGroup.cpp
	g++ $(CFLAGS_RELEASE) -I include -I lib $< -o $@

# compiles PostfixParenthesesGroup.cpp in debug
debug_objects += obj/debug/PostfixParenthesesGroup.o
obj/debug/PostfixParenthesesGroup.o: src/PostfixParenthesesGroup.cpp
	g++ $(CFLAGS_DEBUG) -I include -I lib $< -o $@

############################################################
# PostfixSquareBracketsGroup
############################################################

# link object files and build binary for PostfixSquareBracketsGroup in test
test_binaries += bin/test/PostfixSquareBracketsGroup
bin/test/PostfixSquareBracketsGroup: lib/libs.a obj/debug/PostfixSquareBracketsGroup.o obj/test/PostfixSquareBracketsGroup.o
	g++ $^ -o $@

# compiles PostfixSquareBracketsGroup.cpp in test
obj/test/PostfixSquareBracketsGroup.o: src/test/PostfixSquareBracketsGroup.cpp
	g++ $(CFLAGS_TEST) -I include -I lib $< -o $@

# compiles PostfixSquareBracketsGroup.cpp in release
release_objects += obj/release/PostfixSquareBracketsGroup.o
obj/release/PostfixSquareBracketsGroup.o: src/PostfixSquareBracketsGroup.cpp
	g++ $(CFLAGS_RELEASE) -I include -I lib $< -o $@

# compiles PostfixSquareBracketsGroup.cpp in debug
debug_objects += obj/debug/PostfixSquareBracketsGroup.o
obj/debug/PostfixSquareBracketsGroup.o: src/PostfixSquareBracketsGroup.cpp
	g++ $(CFLAGS_DEBUG) -I include -I lib $< -o $@

############################################################
# CurlyBracketsGroup
############################################################

# link object files and build binary for CurlyBracketsGroup in test
test_binaries += bin/test/CurlyBracketsGroup
bin/test/CurlyBracketsGroup: lib/libs.a obj/debug/CurlyBracketsGroup.o obj/test/CurlyBracketsGroup.o
	g++ $^ -o $@

# compiles CurlyBracketsGroup.cpp in test
obj/test/CurlyBracketsGroup.o: src/test/CurlyBracketsGroup.cpp
	g++ $(CFLAGS_TEST) -I include -I lib $< -o $@

# compiles CurlyBracketsGroup.cpp in release
release_objects += obj/release/CurlyBracketsGroup.o
obj/release/CurlyBracketsGroup.o: src/CurlyBracketsGroup.cpp
	g++ $(CFLAGS_RELEASE) -I include -I lib $< -o $@

# compiles CurlyBracketsGroup.cpp in debug
debug_objects += obj/debug/CurlyBracketsGroup.o
obj/debug/CurlyBracketsGroup.o: src/CurlyBracketsGroup.cpp
	g++ $(CFLAGS_DEBUG) -I include -I lib $< -o $@

############################################################
# Quotation
############################################################

# link object files and build binary for Quotation in test
test_binaries += bin/test/Quotation
bin/test/Quotation: lib/libs.a obj/debug/Quotation.o obj/test/Quotation.o
	g++ $^ -o $@

# compiles Quotation.cpp in test
obj/test/Quotation.o: src/test/Quotation.cpp
	g++ $(CFLAGS_TEST) -I include -I lib $< -o $@

# compiles Quotation.cpp in release
release_objects += obj/release/Quotation.o
obj/release/Quotation.o: src/Quotation.cpp
	g++ $(CFLAGS_RELEASE) -I include -I lib $< -o $@

# compiles Quotation.cpp in debug
debug_objects += obj/debug/Quotation.o
obj/debug/Quotation.o: src/Quotation.cpp
	g++ $(CFLAGS_DEBUG) -I include -I lib $< -o $@

############################################################
# Quoted
############################################################

# link object files and build binary for Quoted in test
test_binaries += bin/test/Quoted
bin/test/Quoted: lib/libs.a obj/debug/Quoted.o obj/test/Quoted.o
	g++ $^ -o $@

# compiles Quoted.cpp in test
obj/test/Quoted.o: src/test/Quoted.cpp
	g++ $(CFLAGS_TEST) -I include -I lib $< -o $@

# compiles Quoted.cpp in release
release_objects += obj/release/Quoted.o
obj/release/Quoted.o: src/Quoted.cpp
	g++ $(CFLAGS_RELEASE) -I include -I lib $< -o $@

# compiles Quoted.cpp in debug
debug_objects += obj/debug/Quoted.o
obj/debug/Quoted.o: src/Quoted.cpp
	g++ $(CFLAGS_DEBUG) -I include -I lib $< -o $@

############################################################
# Atom
############################################################

# link object files and build binary for Atom in test
test_binaries += bin/test/Atom
bin/test/Atom: lib/libs.a obj/debug/Atom.o obj/test/Atom.o
	g++ $^ -o $@

# compiles Atom.cpp in test
obj/test/Atom.o: src/test/Atom.cpp
	g++ $(CFLAGS_TEST) -I include -I lib $< -o $@

# compiles Atom.cpp in release
release_objects += obj/release/Atom.o
obj/release/Atom.o: src/Atom.cpp
	g++ $(CFLAGS_RELEASE) -I include -I lib $< -o $@

# compiles Atom.cpp in debug
debug_objects += obj/debug/Atom.o
obj/debug/Atom.o: src/Atom.cpp
	g++ $(CFLAGS_DEBUG) -I include -I lib $< -o $@

############################################################
# libs
############################################################

# aggregate all lib objects into one static lib #
.SECONDEXPANSION:
lib/libs.a: $$(lib_objects)
	ar rvs $@ $^

# compiles lib used for unit testing (catch2) #
lib_objects += lib/catch2/obj/catch_amalgamated.o
lib/catch2/obj/catch_amalgamated.o: lib/catch2/src/catch_amalgamated.cpp lib/catch2/catch_amalgamated.hpp
	g++ -c $< -o $@ -I lib/catch2

############################################################


.PHONY: all release debug test clean mrproper

# will create all necessary directories after the Makefile is parsed
$(shell mkdir -p obj/release obj/debug obj/test bin/release bin/debug bin/test $(LIBS_OBJ_DIR))
