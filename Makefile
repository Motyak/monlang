SHELL := /bin/bash
RM := rm -rf
CXXFLAGS_RELEASE := --std=c++23 -Wall -Wextra -O0 -I include -I lib
CXXFLAGS_DEBUG := --std=c++23 -Wall -Wextra -Og -g -I include -I lib
CXXFLAGS_TEST := --std=c++23 -Wall -Wextra -Og -g -I include -I lib
DEPFLAGS = -MMD -MP -MF .deps/$(notdir $*.d)
DEPFLAGS_TEST = -MMD -MP -MF .deps/test/$(notdir $*.d)
ARFLAGS := rvs

###########################################################

# ENTITIES := $(foreach file,$(wildcard src/[A-Z]*.cpp),$(notdir $(file:%.cpp=%)))
ENTITIES := \
Association \
Atom \
CurlyBracketsGroup \
ParenthesesGroup \
PostfixParenthesesGroup \
PostfixSquareBracketsGroup \
Program \
ProgramWord \
Quotation \
Quoted \
SquareBracketsGroup \
Term \

RELEASE_OBJS := $(ENTITIES:%=obj/release/%.o)
DEBUG_OBJS := $(ENTITIES:%=obj/debug/%.o)
TEST_OBJS := $(ENTITIES:%=obj/test/%.o)
TEST_BINS := $(ENTITIES:%=bin/test/%)

DEPS := $(ENTITIES:%=.deps/%.d) .deps/common.d
TEST_DEPS := $(ENTITIES:%=.deps/test/%.d)

LIB_OBJ_DIRS := $(foreach lib,$(wildcard lib/*/),$(lib:%/=%)/obj) # for cleaning

###########################################################

all: release

release: bin/release/monlang

debug: bin/debug/monlang

test: $(TEST_BINS)
	for f in bin/test/*; do [ -x "$$f" ] || continue; echo "$$f:"; ./$$f --allow-running-no-tests; done

clean:
	$(RM) $(RELEASE_OBJS) $(DEBUG_OBJS) $(TEST_OBJS) $(DEPS) $(TEST_DEPS) \
			obj/release/common.o obj/debug/common.o \
			obj/release/monlang.o obj/debug/monlang.o

mrproper:
	$(RM) obj .deps lib/libs.a lib/test-libs.a $(LIB_OBJ_DIRS) bin


.PHONY: all release debug test clean mrproper

###########################################################

bin/release/monlang: $(RELEASE_OBJS) lib/libs.a obj/release/common.o obj/release/monlang.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

bin/debug/monlang: $(DEBUG_OBJS) lib/libs.a obj/debug/common.o obj/debug/monlang.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

.SECONDEXPANSION:
$(TEST_BINS): %: $(DEBUG_OBJS) obj/test/$$(notdir %.o) lib/test-libs.a obj/debug/common.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

.SECONDEXPANSION:
$(RELEASE_OBJS) obj/release/common.o obj/release/monlang.o: %.o: src/$$(notdir %.cpp)
	$(CXX) -o $@ -c $< $(CXXFLAGS_RELEASE) $(DEPFLAGS)

.SECONDEXPANSION:
$(DEBUG_OBJS) obj/debug/common.o obj/debug/monlang.o: %.o: src/$$(notdir %.cpp)
	$(CXX) -o $@ -c $< $(CXXFLAGS_DEBUG) $(DEPFLAGS)

.SECONDEXPANSION:
$(TEST_OBJS): %.o: src/test/$$(notdir %.cpp)
	$(CXX) -o $@ -c $< $(CXXFLAGS_TEST) $(DEPFLAGS_TEST)

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
	$(AR) $(ARFLAGS) $@ $^

# compiles lib used for unit testing (catch2) #
test_lib_objects += lib/catch2/obj/catch_amalgamated.o
lib/catch2/obj/catch_amalgamated.o: lib/catch2/src/catch_amalgamated.cpp lib/catch2/catch_amalgamated.hpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) -I lib/catch2

###########################################################

# will create all necessary directories after the Makefile is parsed #
$(shell mkdir -p obj/release obj/debug obj/test .deps/test bin/release bin/debug bin/test $(LIB_OBJ_DIRS))
