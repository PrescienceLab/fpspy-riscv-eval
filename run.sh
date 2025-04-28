#!/usr/bin/env bash

# make RUN_LEN bigger to process more FP ops per iteration
# make NUM_RUNS bigger to perform more runs (averaged together later)
RUN_LEN=1000
NUM_RUNS=10

for FPSPY in sig_brk.so ppe_brk.so ppe_estep.so; do
    echo $FPSPY
    LD_PRELOAD=./$FPSPY FPSPY_MODE=individual \
    ./scanner $RUN_LEN $NUM_RUNS "${FPSPY%.so}.csv" 2>/dev/null
done
