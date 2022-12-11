MAKES=$(wildcard [0-9]*/Makefile )

check:
	for i in $(MAKES); do (cd $$(dirname $$i) && make check ); done

clean:
	for i in $(MAKES); do (cd $$(dirname $$i) && make clean ); done
all:
	for i in $(MAKES); do (cd $$(dirname $$i) && make all ); done
