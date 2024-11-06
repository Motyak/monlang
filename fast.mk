include utils.mk # buildmacros, askmake, not, shell_onrun, shouldrebuild

export CXX := ccache g++

SHELL := /bin/bash
RM := rm -rf
CXXFLAGS := --std=c++23 -Wall -Wextra -Og -ggdb3 -I include
CXXFLAGS_RELEASE := --std=c++23 -Wall -Wextra -Werror -O3 -I include
CXXFLAGS_TEST = $(CXXFLAGS) $(addprefix -I ,$(LIB_INCLUDE_DIRS))
DEPFLAGS = -MMD -MP -MF .deps/$*.d
DEPFLAGS_TEST = -MMD -MP -MF .deps/test/$*.d
ARFLAGS := rcsv

BUILD_LIBS_ONCE ?= x # disable by passing `BUILD_LIBS_ONCE=`
TRACE ?= # enable by passing `TRACE=x`
DISABLE_WORDS ?= # e.g.: DISABLE_WORDS=SBG,
DISABLE_POSTFIXES ?= # e.g.: DISABLE_POSTFIXES=PG_IN_ATOM,

ifdef CLANG
CXX := ccache clang++
# ugly workaround to support clang
CXXFLAGS += -D__cpp_concepts=202002L
LDFLAGS += -lstdc++
endif

ifdef X86
CXXFLAGS += -m32
LDFLAGS += -m32
endif

###########################################################

ENTITIES := \
Atom \
CurlyBracketsGroup \
ParenthesesGroup \
Program \
ProgramSentence \
SquareBracketsGroup \
SquareBracketsTerm \
Term \
Word \

OBJS := $(ENTITIES:%=obj/%.o) obj/common.o
DEPS := $(ENTITIES:%=.deps/%.d) .deps/common.d

RELEASE_OBJS := $(ENTITIES:%=obj/release/%.o) obj/release/common.o

TEST_FILENAMES := $(foreach file,$(wildcard src/test/[!all]*.cpp),$(file:src/test/%.cpp=%))
TEST_OBJS := $(TEST_FILENAMES:%=obj/test/%.o)
TEST_BINS := $(TEST_FILENAMES:%=bin/test/%.elf)

LIB_OBJ_DIRS := $(foreach lib,$(wildcard lib/*/),$(lib:%/=%)/obj) # for cleaning
LIB_INCLUDE_DIRS := $(foreach lib,$(wildcard lib/*/),$(lib:%/=%)/include)

###########################################################

all: main

main: $(OBJS)

test: bin/test/all.elf
	./run_tests.sh

# performs "sanity" check
check: bin/test/bigbang.elf
	bin/test/bigbang.elf

dist: $(RELEASE_OBJS)
	./release.sh

clean:
	$(RM) obj/* .deps/*

mrproper:
	$(RM) obj .deps bin dist lib/libs.a lib/test-libs.a $(LIB_OBJ_DIRS)

.PHONY: all main test check dist clean mrproper

###########################################################

%/common.o: trace_macro := $(if $(TRACE), -D TRACE)
%/Word.o: word_macros := $(call buildmacros, DISABLE_, $(DISABLE_WORDS))
%/Atom.o: postfix_macros := $(call buildmacros, DISABLE_P, $(DISABLE_POSTFIXES), %_IN_ATOM)
%/ParenthesesGroup.o: postfix_macros := $(call buildmacros, DISABLE_P, $(DISABLE_POSTFIXES), %_IN_PG)
%/SquareBracketsGroup.o: postfix_macros := $(call buildmacros, DISABLE_P, $(DISABLE_POSTFIXES), %_IN_SBG)
%/CurlyBracketsGroup.o: postfix_macros := $(call buildmacros, DISABLE_P, $(DISABLE_POSTFIXES), %_IN_CBG)
macros = $(strip $(trace_macro) $(word_macros) $(postfix_macros))

$(OBJS): obj/%.o: src/%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(DEPFLAGS) $(macros)

$(RELEASE_OBJS): obj/release/%.o: src/%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS_RELEASE) $(DEPFLAGS) $(macros)

$(TEST_OBJS): obj/test/%.o: src/test/%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS_TEST) $(DEPFLAGS_TEST)

$(TEST_BINS): bin/test/%.elf: obj/test/%.o $(OBJS) lib/test-libs.a
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

bin/test/all.elf: $(TEST_OBJS) $(OBJS) lib/test-libs.a
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

-include $(DEPS) $(TEST_DEPS)

############################################################
# libs
############################################################

## aggregate all lib objects into one static lib ##
.SECONDEXPANSION:
lib/libs.a: $$(lib_objects)
	$(AR) $(ARFLAGS) $@ $^

## aggregate all test lib objects into one static test lib ##
.SECONDEXPANSION:
lib/test-libs.a: $$(test_lib_objects)
# when BUILD_LIBS_ONCE is unset => we always enter this recipe
	$(if $(call shouldrebuild, $@, $^), \
		$(AR) $(ARFLAGS) $@ $^)

## compiles lib used for testing (catch2) ##
test_lib_objects += lib/catch2/obj/catch_amalgamated.o
lib/catch2/obj/catch_amalgamated.o:
	$(MAKE) -C lib/catch2

## compiles our own lib used for testing (montree) ##
test_lib_objects += lib/montree/obj/montree.o
$(if $(and $(call not,$(BUILD_LIBS_ONCE)),$(call askmake, lib/montree)), \
	.PHONY: lib/montree/obj/montree.o)
lib/montree/obj/montree.o:
	$(MAKE) -C lib/montree

###########################################################

# will create all necessary directories after the Makefile is parsed
$(call shell_onrun, mkdir -p obj/release obj/test .deps/test bin/test $(LIB_OBJ_DIRS))

## debug settings ##
$(call shell_onrun, [ ! -e bin/test/.gdbinit ] && cp .gdbinit bin/test/.gdbinit)
$(call shell_onrun, grep -qs '^set auto-load safe-path /$$' ~/.gdbinit || echo "set auto-load safe-path /" >> ~/.gdbinit)

.DELETE_ON_ERROR:
.SUFFIXES: