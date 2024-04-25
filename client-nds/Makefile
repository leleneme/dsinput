#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

# export TARGET := $(shell basename $(CURDIR))
export TARGET := dsinput
export TOPDIR := $(CURDIR)

# GMAE_ICON is the image used to create the game icon, leave blank to use default rule

# TODO: fix rom icon
# GAME_ICON := ../data/icon.bmp

# specify a directory which contains the nitro filesystem
# this is relative to the Makefile
NITRO_FILES :=

# These set the information text in the nds file
GAME_TITLE     := DS(i)nput
GAME_SUBTITLE1 := DSi as a controller
GAME_SUBTITLE2 := by leleneme

include $(DEVKITARM)/ds_rules

.PHONY: checkarm7 checkarm9 clean

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
all: checkarm7 checkarm9 $(TARGET).nds

#---------------------------------------------------------------------------------
checkarm7:
	$(MAKE) -C arm7

#---------------------------------------------------------------------------------
checkarm9:
	$(MAKE) -C arm9

#---------------------------------------------------------------------------------
$(TARGET).nds : $(NITRO_FILES) arm7/$(TARGET).elf arm9/$(TARGET).elf
	ndstool	-c $(TARGET).nds -7 arm7/$(TARGET).elf -9 arm9/$(TARGET).elf \
	-b $(GAME_ICON) "$(GAME_TITLE);$(GAME_SUBTITLE1);$(GAME_SUBTITLE2)" \
	$(_ADDFILES)
	rm -f arm9/dsinput.elf
	rm -f arm7/dsinput.elf

#---------------------------------------------------------------------------------
arm7/$(TARGET).elf:
	$(MAKE) -C arm7

#---------------------------------------------------------------------------------
arm9/$(TARGET).elf:
	$(MAKE) -C arm9

#---------------------------------------------------------------------------------
clean:
	$(MAKE) -C arm9 clean
	$(MAKE) -C arm7 clean
	rm -f $(TARGET).nds
