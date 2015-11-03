CFLAGS += -mavx -mfma -std=c99 -Wall -Werror -O0

all: main avxflops

main: main.c
	gcc $(CFLAGS) $< -o $@ -lm

avxflops: avxflops.c
	gcc -fopenmp $(CFLAGS) $< -o $@ -lm

clean:
	rm main
	rm avxflops

cleantxt:
	rm baseline_time.txt
	rm baseline_error.txt
	rm avxsimd_time.txt
	rm avxsimd_error.txt
	rm opt_time.txt
	rm opt_error.txt
	rm avxflops_time.txt
	rm avxflops_error.txt
