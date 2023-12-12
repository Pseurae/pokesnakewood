MAKEFLAGS += --no-print-directory

TOOLDIRS := $(filter-out agbcc poryscript binutils Makefile, $(wildcard *))

.PHONY: all $(TOOLDIRS) poryscript

all: $(TOOLDIRS) poryscript

$(TOOLDIRS):
	@$(MAKE) -C $@

poryscript:
	@cd poryscript && go build
	@cd ../..

poryscript_clean:
	@cd poryscript && rm -rf poryscript poryscript.exe

clean: poryscript_clean
	@$(foreach tooldir,$(TOOLDIRS),$(MAKE) clean -C $(tooldir);)