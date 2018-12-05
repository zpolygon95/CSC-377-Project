SHELL = /bin/bash

bin/simulate: simulate.cpp
	if [[ ! -d bin ]]; then mkdir bin; fi
	g++ simulate.cpp -o bin/simulate

.PHONY: clean
clean:
	rm -rf bin

.PHONY: run
run: bin/simulate
	cd programs; ../bin/simulate file_0
