# this alternate makefile is functionally equivalent,..
# ..but tests compilation is faster (due to test objs)
# however, the makefile is unnecessarily uglier and uses 'ccache' as well
# (for productivity)

include utils.mk # buildmacros, askmake, not, shell_onrun, shouldrebuild

export CXX := ccache g++

SHELL := /bin/bash
RM := rm -rf
CXXFLAGS := --std=c++23 -Wall -Wextra -Og -ggdb3 -I include
CXXFLAGS_TEST = $(CXXFLAGS) $(addprefix -I ,$(LIB_INCLUDE_DIRS))
DEPFLAGS = -MMD -MP -MF .deps/$*.d
DEPFLAGS_TEST = -MMD -MP -MF .deps/test/$*.d
ARFLAGS = D -M < <(tools/aggregate-libs.mri.sh $@ $^); :

TRACE ?= # enable by passing `TRACE=x`
DISABLE_WORDS ?= # e.g.: DISABLE_WORDS=SBG,
DISABLE_POSTFIXES ?= # disable by passing `DISABLE_POSTFIXES=x`
DISABLE_ASSOCS ?= # disable by passing `DISABLE_ASSOCS=x`
DISABLE_SPECIAL_ATOMS ?= # disable by passing `DISABLE_SPECIAL_ATOMS=x`
DISABLE_DOLLARS_CBG ?= # disable by passing `DISABLE_DOLLARS_CBG=x`
DISABLE_PSBG_SUFFICES ?= # disable by passing `DISABLE_PSBG_SUFFIX=x`
DISABLE_ATOM_QUOT ?= # disable by passing `DISABLE_ATOM_QUOT=x`

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
Association \
Atom \
CurlyBracketsGroup \
ParenthesesGroup \
PostfixParenthesesGroup \
PostfixSquareBracketsGroup \
Program \
ProgramSentence \
SquareBracketsGroup \
MultilineSquareBracketsGroup \
SquareBracketsTerm \
Term \
Word \

ENTITY_OBJS := $(ENTITIES:%=obj/%.o) obj/common.o
ENTITY_DEPS := $(ENTITIES:%=.deps/%.d) .deps/common.d

VISITOR_SRC_FILES := $(shell find src -path '*/visitors/*.cpp')
VISITOR_OBJS := $(VISITOR_SRC_FILES:src/%.cpp=obj/%.o)
VISITOR_DEPS := $(VISITOR_SRC_FILES:src/%.cpp=.deps/%.d)

TEST_FILENAMES := $(filter-out all, $(foreach file,$(wildcard src/test/*.cpp),$(file:src/test/%.cpp=%)))
TEST_DEPS := $(TEST_FILENAMES:%=.deps/test/%.d)
TEST_OBJS = $(TEST_FILENAMES:%=obj/test/%.o)
TEST_BINS := $(TEST_FILENAMES:%=bin/test/%.elf)

LIB_INCLUDE_DIRS := $(foreach lib,$(wildcard lib/*/),$(lib:%/=%)/include)
LIB_ARTIFACT_DIRS := ${foreach lib,${wildcard lib/*/},$(lib:%/=%)/{.deps,obj,dist,bin}}# for cleaning

###########################################################

all: main

main: $(ENTITY_OBJS) $(VISITOR_OBJS)

test: bin/test/all.elf
	./run_tests.sh

dist: main
	./release.sh

clean:
	$(RM) obj/* .deps/*

mrproper:
	$(RM) obj .deps bin dist lib/libs.a lib/test-libs.a $(LIB_ARTIFACT_DIRS)

.PHONY: all main test dist clean mrproper

###########################################################

macros = # filled by below makefile inclusion
include handle_macros.mk # uses $(TRACE) $(DISABLE_WORDS) $(DISABLE_POSTFIXES) $(DISABLE_ASSOCS) ..

$(ENTITY_OBJS) $(VISITOR_OBJS): obj/%.o: src/%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(DEPFLAGS) $(macros)

$(TEST_OBJS): obj/test/%.o: src/test/%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS_TEST) $(DEPFLAGS_TEST)

$(TEST_BINS): bin/test/%.elf: obj/test/%.o $(ENTITY_OBJS) lib/test-libs.a
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

bin/test/all.elf: TEST_FILENAMES = base sbg cbg postfix assoc int sbt bigbang
.SECONDEXPANSION:
bin/test/all.elf: $$(TEST_OBJS) $(ENTITY_OBJS) lib/test-libs.a
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

-include $(ENTITY_DEPS) $(VISITOR_DEPS) $(TEST_DEPS)

############################################################
# libs
############################################################

## aggregate all libs (.o, .a) into one static lib ##
.SECONDEXPANSION:
lib/libs.a: $$(libs)
	$(AR) $(ARFLAGS) $@ $^

## aggregate all test lib (.o, .a) into one static lib ##
.SECONDEXPANSION:
lib/test-libs.a: $$(test_libs)
	$(if $(call shouldrebuild, $@, $^), \
		$(AR) $(ARFLAGS) $@ $^)

## compiles lib used for testing (catch2) ##
test_libs += lib/catch2/obj/catch_amalgamated.o
lib/catch2/obj/catch_amalgamated.o:
	$(MAKE) -C lib/catch2

## compiles our own lib used for testing (montree) ##
test_libs += lib/montree/dist/montree.a
$(if $(call askmake, lib/montree), \
	.PHONY: lib/montree/dist/montree.a)
lib/montree/dist/montree.a:
	$(MAKE) -C lib/montree dist

###########################################################

# will create all necessary directories after the Makefile is parsed
${call shell_onrun, mkdir -p {.deps,obj,bin}/test {.deps,obj}/ast/visitors}

## debug settings ##
$(call shell_onrun, [ ! -e bin/test/.gdbinit ] && cp .gdbinit bin/test/.gdbinit)
$(call shell_onrun, grep -qs '^set auto-load safe-path /$$' ~/.gdbinit || echo "set auto-load safe-path /" >> ~/.gdbinit)

## shall not rely on these ##
# .DELETE_ON_ERROR:
.SUFFIXES:
