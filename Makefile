SHELL := /bin/bash
RM := rm -rf
CXXFLAGS_RELEASE := --std=c++17 -Wall -Wextra -O0 -I include -I lib
CXXFLAGS_DEBUG := --std=c++17 -Wall -Wextra -Og -g -I include -I lib
CXXFLAGS_TEST := --std=c++17 -Wall -Wextra -Og -g -I include -I lib
DEPFLAGS = -MMD -MP
ARFLAGS := rvs

###########################################################

# ENTITIES := $(foreach file,$(wildcard src/*.cpp),$(notdir $(file:%.cpp=%)))
ENTITIES := \
Association \
Atom \
CurlyBracketsGroup \
ParenthesesGroup \
PostfixParenthesesGroup \
PostfixSquareBracketsGroup \
Program \
ProgramSentence \
ProgramWord \
Quotation \
Quoted \
SquareBracketsGroup \
Term \

RELEASE_OBJS := $(foreach ent,$(ENTITIES),obj/release/$(ent).o)
DEBUG_OBJS := $(foreach ent,$(ENTITIES),obj/debug/$(ent).o)
TEST_BINS := $(foreach ent,$(ENTITIES),bin/test/$(ent))

DEPS := $(foreach ent,$(ENTITIES),obj/$(ent).d)
TEST_DEPS := $(foreach ent,$(ENTITIES),bin/test/$(ent).d)

LIB_OBJ_DIRS := $(foreach lib,$(wildcard lib/*/),$(lib:%/=%)/obj) # for cleaning

###########################################################

all: release

release: bin/release/monlang

debug: bin/debug/monlang

test: $(TEST_BINS)
	for f in bin/test/*; do [ -x "$$f" ] || continue; echo "$$f:"; ./$$f || exit $$?; done

clean:
	$(RM) obj $(DEPS) $(TEST_DEPS)

mrproper:
	$(RM) obj $(DEPS) $(TEST_DEPS) lib/libs.a $(LIB_OBJ_DIRS) bin


.PHONY: all release debug test clean mrproper

###########################################################

bin/release/monlang: lib/libs.a $(RELEASE_OBJS) obj/release/monlang.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

bin/debug/monlang: lib/libs.a $(DEBUG_OBJS) obj/debug/monlang.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

.SECONDEXPANSION:
$(TEST_BINS): %: lib/test-libs.a obj/debug/$$(notdir %.o) src/test/$$(notdir %.cpp)
	$(CXX) -o $@ $^ $(CXXFLAGS_TEST) $(DEPFLAGS) $(LDFLAGS) $(LDLIBS)

.SECONDEXPANSION:
$(RELEASE_OBJS) obj/release/monlang.o: %.o: src/$$(notdir %.cpp)
	$(CXX) -o $@ -c $< $(CXXFLAGS_RELEASE) $(DEPFLAGS)

.SECONDEXPANSION:
$(DEBUG_OBJS) obj/debug/monlang.o: %.o: src/$$(notdir %.cpp)
	$(CXX) -o $@ -c $< $(CXXFLAGS_DEBUG) $(DEPFLAGS)

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
$(shell mkdir -p obj/release obj/debug bin/release bin/debug bin/test $(LIB_OBJ_DIRS))
