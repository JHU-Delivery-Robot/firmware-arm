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
# Project name (sets outputted bins)
PROJ_NAME := hub
# Project deps
# add all object dependencies here without the .o extension
PROJ_OBJS := main globals handlers init

###################################################
#            BEGIN MAKEFILE INCLUDES              #
###################################################
include update_root.mk
include $(LIBCFG)/atsamd51j18.mk
include $(CFGDIR)/project.mk
