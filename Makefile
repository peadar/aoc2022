MAKES=$(wildcard [0-9]*/Makefile )
clean:
	for i in $(MAKES); do (cd $$(dirname $$i) && make clean ); done
all:
	for i in $(MAKES); do (cd $$(dirname $$i) && make all ); done
check:
	for i in $(MAKES); do (cd $$(dirname $$i) && make check ); done

