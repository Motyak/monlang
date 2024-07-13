include utils.mk # buildmake, missingfile, ifnotmakeflag, clean

SHELL := /bin/bash
RM := rm -rf
CXXFLAGS := --std=c++23 -Wall -Wextra -Og -g -I include
CXXFLAGS_TEST := --std=c++23 -Wall -Wextra -Og -g -I include -I lib
DEPFLAGS = -MMD -MP -MF .deps/$(notdir $*.d)
DEPFLAGS_TEST = -MMD -MP -MF .deps/test/$(notdir $*.d)
ARFLAGS := rcsv

BUILD_LIBS_ONCE ?= y

###########################################################

ENTITIES := \
Atom \
Program \
ProgramSentence \
ProgramWord \
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
	bin/test/all.elf

# able to run in parallel mode, e.g.: make -j clean <targets>
clean:
	@# $@ DONE
$(call clean, $(RM) $(OBJS) $(TEST_OBJS) $(DEPS) $(TEST_DEPS))

mrproper:
	$(RM) bin obj .deps lib/libs.a lib/test-libs.a $(LIB_OBJ_DIRS)

.PHONY: all main test clean mrproper

###########################################################

$(OBJS): obj/%.o: src/%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(DEPFLAGS)

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
# we always enter the recipe because out-of-source info comes from a sub-makefile,..
# .. so the logic for deciding if we should repackage is inside the recipe itself
	$(if $(or $(call missingfile, $@), $(should_repackage_test_libs)), \
		$(AR) $(ARFLAGS) $@ $^)

# compiles lib used for testing (catch2) #
test_lib_objects += lib/catch2/obj/catch_amalgamated.o
lib/catch2/obj/catch_amalgamated.o:
	$(call ifnotmakeflag, q, \
		$(eval should_repackage_test_libs += $(call buildmake, lib/catch2)) \
		$(if $(.BUILDMAKESTATUS:0=), @exit $(.BUILDMAKESTATUS)))

# compiles our own lib used for testing (montree) #
test_lib_objects += lib/montree/obj/montree.o
$(if $(BUILD_LIBS_ONCE),, \
	$(call ifnotmakeflag, q, \
		.PHONY: lib/montree/obj/montree.o))
lib/montree/obj/montree.o:
	$(call ifnotmakeflag, q, \
		$(eval should_repackage_test_libs += $(call buildmake, lib/montree)) \
		$(if $(.BUILDMAKESTATUS:0=), @exit $(.BUILDMAKESTATUS)))

###########################################################

# will create all necessary directories after the Makefile is parsed #
$(shell mkdir -p obj/test .deps/test bin/test $(LIB_OBJ_DIRS))

.DELETE_ON_ERROR:
.SUFFIXES:
