# Root recursive Makefile
PROJECT_ROOT  := $(shell pwd)
CFGDIR        := $(PROJECT_ROOT)/buildcfg
HEXDIR        := $(PROJECT_ROOT)/bin
LIBROOT       := $(PROJECT_ROOT)/vendor
LIBCFG        := $(LIBROOT)/buildcfg
export PROJECT_ROOT
export CFGDIR
export HEXDIR
export LIBROOT
export LIBCFG
# propagate user flags
export CFLAGS
include $(CFGDIR)/verify_version.mk

ARTEFACTS := blink.hex

.DEFAULT_GOAL := all

# metatargets
.PHONY: all
all: hub motion_fusion examples

.PHONY: hub motion_fusion
hub motion_fusion: common_deps
	$(MAKE) -C src $@

.PHONY: examples
# recursive targets
examples: common_deps
	$(MAKE) -C examples

# dep targets
.PHONY: common_deps
common_deps: _check_lib_vers vendor | $(HEXDIR)

.PHONY: vendor
vendor:
	make -C $(LIBROOT)

# utility targets
.PHONY: compiledb
compiledb:
	rm -f compile_commands.json
	$(MAKE) --always-make --dry-run all > build.log
	compiledb < build.log
	rm -f build.log
.PHONY: clean
clean:
	rm -rf $(HEXDIR)
	$(call_recursive)
.PHONY: libclean
libclean:
	$(MAKE) -C $(LIBROOT) clean

# special make targets
$(HEXDIR):
	@mkdir -p $@

# utilities
define call_recursive
	$(MAKE) -C examples $@
	$(MAKE) -C src $@
endef
