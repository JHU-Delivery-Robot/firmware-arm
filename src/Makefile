include update_root.mk

.PHONY: all
all: gps hub motion_fusion

.PHONY: gps 
gps:
	$(MAKE) -C $@
.PHONY: hub
hub:
	$(MAKE) -C $@
.PHONY: motion_fusion
motion_fusion:
	$(MAKE) -C $@

.PHONY: clean compiledb
clean compiledb:
	$(MAKE) -C gps $@
	$(MAKE) -C hub $@
	$(MAKE) -C motion_fusion $@
