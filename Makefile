MAKES=$(wildcard [0-9]*/Makefile )
.DEFAULT_GOAL = check

%:
	for i in $(MAKES); do make -C $$(dirname $$i) $@; done
