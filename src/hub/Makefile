###################################################
#            BEGIN MAKEFILE SETTINGS              #
###################################################
CFLAGS     +=
INCLUDES   +=
LDFLAGS    +=
LDLIBS     +=
LDLIBS_END +=

# Add all subdirectories here separated by the : char
vpath %.c .
vpath %.c sys
vpath %.c perif
# Project name (sets outputed bins)
PROJ_NAME := hub
# Project deps
# add all object dependencies here without the .o extension
PROJ_OBJS := main globals
PROJ_OBJS += clock handlers init
PROJ_OBJS += sercom uart

###################################################
#            BEGIN MAKEFILE INCLUDES              #
###################################################
include update_root.mk
include $(LIBCFG)/atsamd51j18.mk
include $(CFGDIR)/project.mk
