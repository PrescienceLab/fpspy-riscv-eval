CC=riscv64-unknown-linux-gnu-gcc
CFLAGS = -O2 -Wall -fno-strict-aliasing -g -std=gnu99
LDFLAGS = -O2 -Wall -fno-strict-aliasing -std=gnu99

################################################################################

all: scanner scanner1 fpspy-perf

scanner: sweep.o
	$(CC) $(LDFLAGS) -o $@ $^

scanner%: sweep%.o
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -f scanner sweep.o
