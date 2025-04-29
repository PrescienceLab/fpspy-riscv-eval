#!/usr/bin/env bash

echo "Running pathological benchmark with signal-based FPE"
exception_to_signal_handler-signal-sigfpe > signals.csv

echo "Running pathological benchmark with PPE FPE"
exception_to_signal_handler-pipelined-sigfpe > fpeppe.csv

echo "Running pathological benchmark with ESTEP"
estep-pipelined > estep.csv

echo "Results in CSV files in PWD"
