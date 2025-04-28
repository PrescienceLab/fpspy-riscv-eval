# RISC-V FPE & PPE Microbenchmarks

These microbenchmarks test our modifications to RISC-V that add Floating-Point Exceptions (FPEs) and Precise Pipelined Exceptions (PPEs).

There are two microbenchmarks in this repository:
  1. A pathological worst-case where every iteration of a loop causes a floating-point exception.
     This serves to find the amount of time required to make the processor change its privilege and jump to the handler code.
  2. A sweeper program, which sweeps the period between floating-point exceptions.
     This helps show the diminishing costs of floating-point exceptions (FPEs) as the frequency of exceptional FP instructions decreases.

Both of these are C programs with no external dependencies other than libc.
