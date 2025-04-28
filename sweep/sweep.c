#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

unsigned long rdcycle(void);

int main(int argc, char **argv) {
  assert(argc == 4);
  unsigned run_len = atoi(argv[1]);
  unsigned num_runs = atoi(argv[2]);

  unsigned long long results[21][num_runs];
  const double five = 5.0;
  const double zero = 0.0;
  double z;

  for (unsigned run = 0; run < num_runs; run++) {
    for (unsigned ratio = 1; ratio <= 20; ratio++) {
      unsigned long start, end;

      unsigned exceptionals = run_len / ratio;
      unsigned normals = run_len - exceptionals;

      start = rdcycle();
      for (unsigned i = 0; i < exceptionals; i++) {
        __asm__ __volatile__("fdiv.d %0, %1, %1\n\t" : "=f"(z) : "f"(zero) :);
      }
      for (unsigned i = 0; i < normals; i ++) {
        __asm__ __volatile__("fdiv.d %0, %1, %1\n\t" : "=f"(z) : "f"(five) :);
      }
      end = rdcycle();

      results[ratio][run] = end - start;
    }
  }

  FILE *time_file = fopen(argv[3], "w");
  assert(time_file != NULL);
  fprintf(time_file, "num_fp_insts,period,value\n");
  for (unsigned ratio = 1; ratio <= 20; ratio++) {
    unsigned long total = 0;
    for (unsigned run = 0; run < num_runs; run++) {
      total += results[ratio][run];
    }
    printf("%lu,", total / num_runs);
    fprintf(time_file, "%u,%u,%lu\n", run_len, ratio, total / num_runs);
  }
  printf("\n");
  fclose(time_file);
  return 0;
}

inline unsigned long rdcycle(void) {
  unsigned long rcycle;
  __asm__ __volatile__("rdcycle %0" : "=r"(rcycle));
  return rcycle;
}
