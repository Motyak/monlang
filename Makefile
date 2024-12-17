include utils.mk # buildmacros, askmake, not, shell_onrun, shouldrebuild

SHELL := /bin/bash
RM := rm -rf
CXXFLAGS := --std=c++23 -Wall -Wextra -Og -ggdb3 -I include
CXXFLAGS_TEST = $(CXXFLAGS) $(addprefix -I ,$(LIB_INCLUDE_DIRS))
DEPFLAGS = -MMD -MP -MF .deps/$*.d
DEPFLAGS_TEST = -MMD -MP -MF .deps/test/$*.d
ARFLAGS = D -M < <(tools/aggregate-libs.mri.sh $@ $^); :

BUILD_LIBS_ONCE ?= x # disable by passing `BUILD_LIBS_ONCE=`
TRACE ?= # enable by passing `TRACE=x`
DISABLE_WORDS ?= # e.g.: DISABLE_WORDS=SBG,
DISABLE_POSTFIXES ?= # e.g.: DISABLE_POSTFIXES=PG_IN_ATOM,
DISABLE_ASSOCS ?= # e.g.: DISABLE_ASSOCSS=ATOM,

ifdef CLANG
CXX := clang++
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
SquareBracketsTerm \
Term \
Word \

ENTITY_OBJS := $(ENTITIES:%=obj/%.o) obj/common.o
ENTITY_DEPS := $(ENTITIES:%=.deps/%.d) .deps/common.d

VISITOR_SRC_FILES := $(shell find src -path '*/visitors/*.cpp')
VISITOR_OBJS := $(VISITOR_SRC_FILES:src/%.cpp=obj/%.o)
VISITOR_DEPS := $(VISITOR_SRC_FILES:src/%.cpp=.deps/%.d)

TEST_FILENAMES := $(foreach file,$(wildcard src/test/*.cpp),$(file:src/test/%.cpp=%))
TEST_DEPS := $(TEST_FILENAMES:%=.deps/test/%.d)
TEST_BINS := $(TEST_FILENAMES:%=bin/test/%.elf)

LIB_OBJ_DIRS := $(foreach lib,$(wildcard lib/*/),$(lib:%/=%)/obj) # for cleaning
LIB_INCLUDE_DIRS := $(foreach lib,$(wildcard lib/*/),$(lib:%/=%)/include)

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
	$(RM) obj .deps bin dist lib/libs.a lib/test-libs.a $(LIB_OBJ_DIRS)

.PHONY: all main test dist clean mrproper

###########################################################

macros := # filled by below makefile inclusion
include handle_macros.mk # uses $(TRACE) $(DISABLE_WORDS) $(DISABLE_POSTFIXES) $(DISABLE_ASSOCS)

$(ENTITY_OBJS) $(VISITOR_OBJS): obj/%.o: src/%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(DEPFLAGS) $(macros)

$(TEST_BINS): bin/test/%.elf: src/test/%.cpp $(ENTITY_OBJS) lib/test-libs.a
	$(CXX) -o $@ $< $(ENTITY_OBJS) lib/test-libs.a $(CXXFLAGS_TEST) $(DEPFLAGS_TEST) $(LDFLAGS) $(LDLIBS)

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
# when BUILD_LIBS_ONCE is unset => we always enter this recipe
	$(if $(call shouldrebuild, $@, $^), \
		$(AR) $(ARFLAGS) $@ $^)

## compiles lib used for testing (catch2) ##
test_libs += lib/catch2/obj/catch_amalgamated.o
lib/catch2/obj/catch_amalgamated.o:
	$(MAKE) -C lib/catch2

## compiles our own lib used for testing (montree) ##
test_libs += lib/montree/dist/montree.a
$(if $(and $(call not,$(BUILD_LIBS_ONCE)),$(call askmake, lib/montree)), \
	.PHONY: lib/montree/dist/montree.a)
lib/montree/dist/montree.a:
	$(MAKE) -C lib/montree

###########################################################

# will create all necessary directories after the Makefile is parsed
${call shell_onrun, mkdir -p {.deps,bin}/test {.deps,obj}/ast/visitors ${LIB_OBJ_DIRS}}

## debug settings ##
$(call shell_onrun, [ ! -e bin/test/.gdbinit ] && cp .gdbinit bin/test/.gdbinit)
$(call shell_onrun, grep -qs '^set auto-load safe-path /$$' ~/.gdbinit || echo "set auto-load safe-path /" >> ~/.gdbinit)

# .DELETE_ON_ERROR: # shall not rely on this
.SUFFIXES:
