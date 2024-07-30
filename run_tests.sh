#!/bin/sh

### HAPPY SCENARIOS ############################################

happy_scenarios_results="0/3 base happy scenarios => KO"

# run base happy scenarios
{
    bin/test/all.elf '[base]' \
        && happy_scenarios_results="1/3 base happy scenarios => OK"
} && \

# run remaining happy scenarios
{
    bin/test/all.elf '~[base]~[int]' \
        && happy_scenarios_results="2/3 all happy scenarios => OK"
} && \

# run integration tests
{
    bin/test/all.elf '[int]' \
        && happy_scenarios_results="3/3 integration tests => OK"
}

### NEGATIVE SCENARIOS #########################################

negative_scenarios_results="0/2 base negative scenarios => KO"

# run negative base scenarios
{
    bin/test/all.elf '[err][base]' \
        && negative_scenarios_results="1/2 base negative scenarios => OK"
} && \

# run remaining negative scenarios
{
    bin/test/all.elf '[err]~[base]' \
        && negative_scenarios_results="2/2 all negative scenarios => OK"
}

### RESULTS ###################################################

echo
echo "${happy_scenarios_results}"
echo "${negative_scenarios_results}"
