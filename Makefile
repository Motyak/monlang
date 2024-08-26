include utils.mk # buildmake, clean, not, checkmakeflags, shouldrebuild

SHELL := /bin/bash
RM := rm -rf
CXXFLAGS := --std=c++23 -Wall -Wextra -Og -ggdb3 -I include
CXXFLAGS_TEST := --std=c++23 -Wall -Wextra -Og -ggdb3 -I include -I lib
DEPFLAGS = -MMD -MP -MF .deps/$(notdir $*.d)
DEPFLAGS_TEST = -MMD -MP -MF .deps/test/$(notdir $*.d)
ARFLAGS := rcsv

DISABLE_WORDS ?= $(empty) # e.g.: DISABLE_WORDS=SBG,
BUILD_LIBS_ONCE ?= y
ifdef CLANG
	CXX := clang++
#	ugly workaround to support clang
	CXXFLAGS += -D__cpp_concepts=202002L
	CXXFLAGS_TEST += -D__cpp_concepts=202002L
	LDFLAGS += -lstdc++
endif
ifdef X86
	CXXFLAGS += -m32
	CXXFLAGS_TEST += -m32
	LDFLAGS += -m32
endif

###########################################################

ENTITIES := \
Atom \
Program \
ProgramSentence \
ProgramWord \
SquareBracketsGroup \
Term \
Word \

OBJS := $(ENTITIES:%=obj/%.o) obj/common.o
DEPS := $(ENTITIES:%=.deps/%.d) .deps/common.d

TEST_FILENAMES := $(foreach file,$(wildcard src/test/*.cpp),$(file:src/test/%.cpp=%))

TEST_OBJS := $(TEST_FILENAMES:%=obj/test/%.o)
TEST_DEPS := $(TEST_FILENAMES:%=.deps/test/%.d)
TEST_BINS := $(TEST_FILENAMES:%=bin/test/%.elf)

LIB_OBJ_DIRS := $(foreach lib,$(wildcard lib/*/),$(lib:%/=%)/obj) # for cleaning

###########################################################

all: main

main: $(OBJS)

test: bin/test/all.elf
	./run_tests.sh

# able to run in parallel mode, e.g.: make -j clean <targets>
clean:
	@# $@ DONE
$(call clean, $(RM) $(OBJS) $(TEST_OBJS) $(DEPS) $(TEST_DEPS))

mrproper:
	$(RM) bin obj .deps lib/libs.a lib/test-libs.a $(LIB_OBJ_DIRS)

.PHONY: all main test clean mrproper

###########################################################

# OBJS #
$(filter-out obj/Word.o,$(OBJS)): obj/%.o: src/%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(DEPFLAGS)
word_macros := $(addprefix -D DISABLE_,$(subst $(comma),$(space),$(DISABLE_WORDS)))
obj/Word.o: src/Word.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(DEPFLAGS) $(word_macros)

$(TEST_OBJS): obj/test/%.o: src/test/%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS_TEST) $(DEPFLAGS_TEST)

$(TEST_BINS): bin/test/%.elf: obj/test/%.o $(OBJS) lib/test-libs.a
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

-include $(DEPS) $(TEST_DEPS)

############################################################
# libs
############################################################

# aggregate all lib objects into one static lib #
.SECONDEXPANSION:
lib/libs.a: $$(lib_objects)
	$(AR) $(ARFLAGS) $@ $^

# aggregate all test lib objects into one static test lib #
.SECONDEXPANSION:
lib/test-libs.a: $$(test_lib_objects)
# (in 'BUILD_LIBS_ONCE= ' mode) we always enter the recipe..
# .. to check if libs are outdated (by questioning their make)
ifneq (,$(or $(BUILD_LIBS_ONCE), \
			 $(call not, $(BUILD_LIBS_ONCE) $(call checkmakeflags, n q))))
	$(if $(call shouldrebuild, $@, $^), \
		$(AR) $(ARFLAGS) $@ $^)
endif

# compiles lib used for testing (catch2) #
test_lib_objects += lib/catch2/obj/catch_amalgamated.o
lib/catch2/obj/catch_amalgamated.o:
ifneq (,$(call askmake, lib/catch2))
	$(call buildmake, lib/catch2)
endif

# compiles our own lib used for testing (montree) #
test_lib_objects += lib/montree/obj/montree.o
$(if $(BUILD_LIBS_ONCE),, \
	.PHONY: lib/montree/obj/montree.o)
lib/montree/obj/montree.o:
ifneq (,$(call askmake, lib/montree))
	$(call buildmake, lib/montree)
endif

###########################################################

# will create all necessary directories after the Makefile is parsed #
$(shell mkdir -p obj/test .deps/test bin/test $(LIB_OBJ_DIRS))

# debug settings #
$(shell [ ! -e bin/test/.gdbinit ] && cp .gdbinit bin/test/.gdbinit)
$(shell grep -qs '^set auto-load safe-path /$$' ~/.gdbinit || echo "set auto-load safe-path /" >> ~/.gdbinit)

.DELETE_ON_ERROR:
.SUFFIXES:
