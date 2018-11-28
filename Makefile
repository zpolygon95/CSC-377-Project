SHELL = /bin/bash

bin/simulate: simulate.c
	if [[ ! -d bin ]]; then mkdir bin; fi
	gcc simulate.c -o bin/simulate

.PHONY: clean
clean:
	rm -rf bin
