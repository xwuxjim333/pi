CFLAGS += -mavx -std=c99 -Wall -Werror -O0

all: main

main: main.c
	gcc $(CFLAGS) $< -o $@ -lm

clean:
	rm main
	rm baseline_time.txt
	rm baseline_error.txt
	rm avxsimd_time.txt
	rm avxsimd_error.txt
	rm opt_time.txt
	rm opt_error.txt
