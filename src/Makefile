include update_root.mk

.PHONY: all
all: hub motion_fusion

.PHONY: hub
hub:
	$(MAKE) -C $@
.PHONY: motion_fusion
motion_fusion:
	$(MAKE) -C $@

.PHONY: clean compiledb
clean compiledb:
	$(MAKE) -C hub $@
	$(MAKE) -C motion_fusion $@
