#!/usr/bin/env bash

function run_NAS {
	local fpspy_config="$1"
	local TIMING_DIR='timing'
	mkdir -p "$TIMING_DIR"
	for EXE in *.S.x; do
		echo "Running $EXE"
		( time ./$EXE ) 2> "$TIMING_DIR/$EXE.$fpspy_config.time"
		./$EXE
        done
}

unset FPSPY_MODE
unset FPSPY_AGGRESSIVE
unset FPSPY_EXCEPT_LIST

# Run all NAS benches without FPSpy to form a baseline
run_NAS 'baseline'

# Run with all 3 with everything EXCEPT precision
echo "Running NAS under FPSpy with rounding events turned OFF"
export FPSPY_MODE=individual
export FPSPY_AGGRESSIVE=yes
export FPSPY_EXCEPT_LIST=invalid,denorm,divide,overflow,underflow

LD_PRELOAD=/fpspy-signals/bin/riscv64/fpspy.so run_NAS 'signals'
LD_PRELOAD=/fpspy-fpeppe/bin/riscv64/fpspy.so run_NAS 'fpeppe'
LD_PRELOAD=/fpspy-estep/bin/riscv64/fpspy.so run_NAS 'estep'

# Run with all 3 configs with rounding (precision) turned on
echo "Running NAS under FPSpy with rounding events turned ON"
export FPSPY_MODE=individual
export FPSPY_AGGRESSIVE=yes
export FPSPY_EXCEPT_LIST=invalid,denorm,divide,overflow,underflow,precision

LD_PRELOAD=/fpspy-signals/bin/riscv64/fpspy.so run_NAS 'signals.precision'
LD_PRELOAD=/fpspy-fpeppe/bin/riscv64/fpspy.so run_NAS 'fpeppe.precision'
LD_PRELOAD=/fpspy-estep/bin/riscv64/fpspy.so run_NAS 'estep.precision'

unset FPSPY_MODE
unset FPSPY_AGGRESSIVE
unset FPSPY_EXCEPT_LIST
