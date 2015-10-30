CFLAGS += -mavx -std=c99 -Wall -Werror -O0

main: main.c
	gcc $(CFLAGS) $< -o $@ -lm

clean:
	rm main
	rm baseline_time.txt
	rm baseline_error.txt
