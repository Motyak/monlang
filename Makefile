CXXFLAGS_RELEASE := --std=c++17 -Wall -Wextra -O0 -I include -I lib
CXXFLAGS_DEBUG := --std=c++17 -Wall -Wextra -Og -g -I include -I lib
CXXFLAGS_TEST := --std=c++17 -Wall -Wextra -Og -g -I include -I lib
DEPFLAGS = -MMD -MP
ARFLAGS := rvs
RM := rm -rf

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

SRCS := $(foreach ent,$(ENTITIES),src/$(ent).cpp)
TEST_SRCS := $(foreach ent,$(ENTITIES),src/test/$(ent).cpp)

RELEASE_OBJS := $(foreach ent,$(ENTITIES),obj/release/$(ent).o)
DEBUG_OBJS := $(foreach ent,$(ENTITIES),obj/debug/$(ent).o)
TEST_OBJS := $(foreach ent,$(ENTITIES),obj/test/$(ent).o)

DEPS := $(foreach ent,$(ENTITIES),obj/$(ent).d)
TEST_DEPS := $(foreach ent,$(ENTITIES),obj/test/$(ent).d)

TEST_BINS := $(foreach ent,$(ENTITIES),bin/test/$(ent))

LIBS_OBJ_DIR := $(foreach lib,$(wildcard lib/*/),$(lib:%/=%)/obj) # for cleaning

###########################################################

all: release

release: bin/release/monlang

debug: bin/debug/monlang

test: $(TEST_BINS)
	for test_binary in bin/test/*; do echo "$$test_binary:"; ./$$test_binary || exit $$?; done

clean:
	$(RM) obj $(DEPS) $(TEST_DEPS)

mrproper:
	$(RM) obj $(DEPS) $(TEST_DEPS) lib/libs.a $(LIBS_OBJ_DIR) bin

###########################################################

bin/release/monlang: lib/libs.a $(RELEASE_OBJS) obj/release/monlang.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

bin/debug/monlang: lib/libs.a $(DEBUG_OBJS) obj/debug/monlang.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

.SECONDEXPANSION:
$(TEST_BINS): %: lib/test-libs.a obj/debug/$$(notdir %.o) obj/test/$$(notdir %.o)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

.SECONDEXPANSION:
$(RELEASE_OBJS) obj/release/monlang.o: %.o: src/$$(notdir %.cpp)
	$(CXX) -o $@ -c $< $(CXXFLAGS_RELEASE) $(DEPFLAGS)

.SECONDEXPANSION:
$(DEBUG_OBJS) obj/debug/monlang.o: %.o: src/$$(notdir %.cpp)
	$(CXX) -o $@ -c $< $(CXXFLAGS_DEBUG) $(DEPFLAGS)

.SECONDEXPANSION:
$(TEST_OBJS): %.o: src/test/$$(notdir %.cpp)
	$(CXX) -o $@ -c $^ $(CXXFLAGS_TEST) $(DEPFLAGS)

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

.PHONY: all release debug test clean mrproper

# will create all necessary directories after the Makefile is parsed #
$(shell mkdir -p obj/release obj/debug obj/test bin/release bin/debug bin/test $(LIBS_OBJ_DIR))
