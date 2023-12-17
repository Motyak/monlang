RM := rm -rf
CXXFLAGS_RELEASE := --std=c++17 -Wall -Wextra -O0 -I include -I lib
CXXFLAGS_DEBUG := --std=c++17 -Wall -Wextra -Og -g -I include -I lib
CXXFLAGS_TEST := --std=c++17 -Wall -Wextra -Og -g -I include -I lib
DEPFLAGS = -MMD -MP
ARFLAGS := rvs

###########################################################

SRCS := $(wildcard src/*.cpp)
TEST_SRCS := $(foreach src,$(SRCS),src/test/$(src))
LIB_SRCS := $(foreach lib,$(wildcard lib/*/),$(patsubst %/,%,$(lib))/src/*.cpp)

RELEASE_OBJS := $(foreach src,$(SRCS),obj/release/$($(notdir $(src)):.cpp=.o))
DEBUG_OBJS := $(foreach src,$(SRCS),obj/debug/$($(notdir $(src)):.cpp=.o))
TEST_OBJS := $(foreach test_src,$(TEST_SRCS),obj/test/$($(notdir $(test_src)):.cpp=.o))
LIB_OBJS := $(foreach lib_src,$(LIB_SRCS),$(lib_src:/src/%.cpp=/obj/%.o))

DEPS := $(foreach src,$(SRCS),obj/$(src:.cpp=.d))
TEST_DEPS := $(foreach test_src,$(TEST_SRCS),obj/test/$(test_src:.cpp=.d))

TEST_BINS := $(foreach test_src,$(TEST_SRCS),obj/test/$(test_src:.cpp=))

LIBS_OBJ_DIR := $(foreach lib,$(wildcard lib/*/),$(patsubst %/,%,$(lib))/obj)

###########################################################

all: release

release: bin/release/monlang

debug: bin/debug/monlang

test: $(TEST_BINS)

clean:
	$(RM) obj lib/libs.a $(LIBS_OBJ_DIR)

mrproper:
	$(RM) obj lib/libs.a $(LIBS_OBJ_DIR) bin

###########################################################

bin/release/monlang: $(RELEASE_OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

bin/debug/monlang: $(DEBUG_OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(RELEASE_OBJS): %.o: src/$($(notdir %):.o=.cpp)
	$(CXX) -o $@ -c $< $(CXXFLAGS_RELEASE) $(DEPFLAGS)

$(DEBUG_OBJS): %.o: src/$($(notdir %):.o=.cpp)
	$(CXX) -o $@ -c $< $(CXXFLAGS_DEBUG) $(DEPFLAGS)

$(TEST_OBJS): %.o: src/test/$($(notdir %):.o=.cpp)
	$(CXX) -o $@ -c $< $(CXXFLAGS_TEST) $(DEPFLAGS)

lib/libs.a: $(LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(LIB_OBJS): %.cpp: $(patsubst obj/%,src/%,%).cpp
	$(CXX) -o $@ -c $< -I lib/$(patsubst lib/%/,%,$@)

###########################################################

.PHONY: all release debug test clean mrproper

# will create all necessary directories after the Makefile is parsed
$(shell mkdir -p obj/release obj/debug obj/test bin/release bin/debug bin/test $(LIBS_OBJ_DIR))
