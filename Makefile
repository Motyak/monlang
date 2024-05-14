SHELL := /bin/bash
RM := rm -rf
CXXFLAGS := --std=c++23 -Wall -Wextra -Og -g -I include
DEPFLAGS = -MMD -MP -MF .deps/$(notdir $*.d)

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

###########################################################

all: $(OBJS)

clean:
	$(RM) $(OBJS) $(DEPS)

mrproper:
	$(RM) obj .deps


.PHONY: all clean mrproper

###########################################################

.SECONDEXPANSION:
$(OBJS): %.o: src/$$(notdir %.cpp)
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(DEPFLAGS)

-include $(DEPS)

###########################################################

# will create all necessary directories after the Makefile is parsed #
$(shell mkdir -p obj .deps)
