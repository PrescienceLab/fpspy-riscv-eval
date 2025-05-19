#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned long read_uscratch(void);
unsigned long rdcycle(void);
void nodrama(void);
void underflow(void);
void divzero(void);
void nanny(void);
void denorm(void);
void overflow(void);
void inexact(void);
void use(double x);

const unsigned long long NANO_PER_SEC = 1000000000ULL;
const unsigned long long NANO_PER_MICRO = 1000ULL;

int main(int argc, char **argv) {
  assert(argc == 4);
  unsigned run_len = atoi(argv[1]);
  unsigned num_runs = atoi(argv[2]);

  unsigned long long results[21][num_runs];
  const double five = 5.0;
  const double zero = 0.0;
  const double x = (double)0x7fefffffffffffffULL;
  const double y = (double)0x001fffffffffffffULL;
  double z;

  for (unsigned run = 0; run < num_runs; run++) {
    for (unsigned ratio = 2; ratio <= 20; ratio++) {
      unsigned long start, end;

      start = rdcycle();
      for (unsigned i = 0; i < run_len; i++) {
        unsigned counter = 0;
        if ((i & ratio) == 0) {
	  // Cause an FPE
          __asm__ __volatile__("fdiv.d %0, %1, %1\n\t"
                               : "=f"(z)
                               : "f"(zero)
                               :);
        } else {
	  // Do an FP operation that can NEVER cause an FPE
          __asm__ __volatile__("fdiv.d %0, %1, %1\n\t" : "=f"(z) : "f"(five) :);
        }
        use(z);
      }
      end = rdcycle();

      results[ratio][run] = end - start;
    }
  }

  FILE *time_file = fopen(argv[3], "w");
  assert(time_file != NULL);
  for (unsigned ratio = 2; ratio <= 20; ratio++) {
    unsigned long total = 0;
    for (unsigned run = 0; run < num_runs; run++) {
      total += results[ratio][run];
    }
    printf("%lu,", total / num_runs);
    fprintf(time_file, "%lu,", total / num_runs);
  }
  printf("\n");
  fprintf(time_file, "\n");
  fclose(time_file);
  return 0;
}

inline unsigned long read_uscratch(void) {
  unsigned long uscratch;
  __asm__ __volatile__("csrr %0, 0x840" : "=r"(uscratch));
  return uscratch;
}

inline unsigned long rdcycle(void) {
  unsigned long rcycle;
  __asm__ __volatile__("rdcycle %0" : "=r"(rcycle));
  return rcycle;
}

void nodrama(void) {
  volatile double x, y, z;
  x = 0x5ULL;
  y = 0x5ULL;
  z = x - y;
  use(z);
}

void underflow(void) {
  volatile double x, y, z;
  unsigned long val;
  val = 0x0010000000000001ULL;
  x = *(double *)&val;
  y = x;
  z = x * y;
  use(z);
}

void overflow(void) {
  volatile double x, y, z;
  unsigned long val;
  val = 0x7fefffffffffffffULL;
  x = *(double *)&val;
  y = 4.0;
  z = x * y;
  use(z);
}

void inexact(void) {
  volatile double x, y, z;
  x = 0x7fefffffffffffffULL;
  y = 0x001fffffffffffffULL;
  z = x - y;
  use(z);
}

void divzero(void) {
  volatile double x, y, z;
  x = 99.0;
  y = 0.0;
  z = x / y;
  use(z);
}

void nanny(void) {
  volatile double x, y, z;
  x = 0.0;
  y = 0.0;
  z = x / y;
  use(z);
}

void denorm(void) {
  volatile double x, y, z;
  unsigned long val;
  val = 0x000fffffffffffffULL;
  x = *(double *)&val;
  y = 4.0;
  z = x / y;
  use(z);
}

void use(double x) {}
