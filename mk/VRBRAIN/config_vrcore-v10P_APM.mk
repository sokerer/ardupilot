#
# Makefile for the VRCORE 1.0 APM configuration
#
include $(SKETCHBOOK)/mk/VRBRAIN/vrbrain_common.mk

#
# Use the configuration's ROMFS.
#
ROMFS_ROOT	 = $(SKETCHBOOK)/mk/VRBRAIN/ROMFS_VRBRAIN_APM















MODULES		+= drivers/mpu9250



















MODULES		+= drivers/boards/vrcore-v10P





































































































































