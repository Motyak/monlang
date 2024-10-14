#!/bin/bash

# exec 9> .trace.txt
# export BASH_XTRACEFD=9
# set -o xtrace

BIN="bin/test/all.elf"
j=${1:-$(nproc)} # nb of jobs
seed=$(shuf -i 1-999999999 -n1)

for i in $(seq 0 $((j - 1))); do
    if [ $i -eq 0 ]; then
        echo "$BIN --order rand --rng-seed $seed --shard-count $j --shard-index $i"
        "$BIN" --order rand --rng-seed $seed --shard-count $j --shard-index $i & exit_code=$?
        continue
    fi

    ## `script` allows to save/restore colored output ##
    cmd="echo $BIN --order rand --rng-seed $seed --shard-count $j --shard-index $i"
    cmd="${cmd}; $BIN --order rand --rng-seed $seed --shard-count $j --shard-index $i"
    script -q -e -c "$cmd" /dev/null &> ${seed}${i}.out &
done

for job in $(jobs -p); do
    wait -n $job; job_exit_code=$?
    exit_code=$((exit_code || job_exit_code))
done

for f in $(2>/dev/null ls ${seed}* | sort -n); do
    cat $f
    rm $f
done

[ ${exit_code:-0} -eq 1 ] && {
    echo "Job(s) has failed, check above output"
    exit 1
}
