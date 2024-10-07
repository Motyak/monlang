#!/bin/sh

### HAPPY SCENARIOS ############################################

happy_scenarios_results="0/4 base happy scenarios => KO"

# run base happy scenarios
{
    bin/test/all.elf '[base]~[err]' \
        && happy_scenarios_results="1/4 base happy scenarios => OK"
} && \

# run remaining happy scenarios
{
    bin/test/all.elf '~[base]~[int]~[err]' \
        && happy_scenarios_results="2/4 all happy scenarios => OK"
} && \

# run "entity-word" integration tests
{
    bin/test/all.elf '[int]~[bigbang]' \
        && happy_scenarios_results="3/4 integration tests => OK"
} && \

# run "big bang" integration tests
{
    bin/test/all.elf '[bigbang]' \
        && happy_scenarios_results="4/4 system tests => OK"
}

### NEGATIVE SCENARIOS #########################################

negative_scenarios_results="0/2 base negative scenarios => KO"

# run negative base scenarios
{
    bin/test/all.elf '[base][err]' \
        && negative_scenarios_results="1/2 base negative scenarios => OK"
} && \

# run remaining negative scenarios
{
    bin/test/all.elf '~[base][err]' \
        && negative_scenarios_results="2/2 all negative scenarios => OK"
}

### RESULTS ###################################################

echo
echo "${happy_scenarios_results}"
echo "${negative_scenarios_results}"
