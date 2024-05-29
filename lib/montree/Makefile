SHELL := /bin/bash
RM := rm -rf
CXXFLAGS := --std=c++23 -Wall -Wextra -Og -g -I .
DEPFLAGS = -MMD -MP -MF .deps/$(notdir $*.d)

###########################################################

SRC_NAMES := $(foreach file,$(wildcard src/*.cpp),$(file:src/%.cpp=%))
OBJS := $(SRC_NAMES:%=obj/%.o)
DEPS := $(SRC_NAMES:%=.deps/%.d)

###########################################################

all: build

build: $(OBJS)

clean:
	$(RM) $(OBJS) $(TEST_OBJS) $(DEPS) $(TEST_DEPS)

mrproper:
	$(RM) obj .deps


.PHONY: all build clean mrproper

###########################################################

$(OBJS): obj/%.o: src/%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(DEPFLAGS)

-include $(DEPS)

###########################################################

# will create all necessary directories after the Makefile is parsed #
$(shell mkdir -p obj .deps)
