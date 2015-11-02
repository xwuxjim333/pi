#!/bin/bash
for method in 0 1 2
do
	for dt in 1 2 4 8 16 32 64 128 256 512
	do
		./main $method $dt
	done
done
