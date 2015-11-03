#!/bin/bash
for dt in 1 2 4 8 16 32 64 128 256 512
do
	./avxflops $dt
done
