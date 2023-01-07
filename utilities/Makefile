
# Run 'make SYS=<target>'; or, set a SYS env.
# var. to build for another target system.
SYS ?= apple2

# For this one see https://applecommander.github.io/
AC ?= ac.jar

# Just the usual way to find out if we're
# using cmd.exe to execute make rules.
ifneq ($(shell echo),)
  CMD_EXE = 1
endif

ifdef CMD_EXE
  NULLDEV = nul:
  DEL = -del /f
  RMDIR = rmdir /s /q
else
  NULLDEV = /dev/null
  DEL = $(RM)
  RMDIR = $(RM) -r
endif

ifdef CC65_HOME
  AS = $(CC65_HOME)/bin/ca65
  CC = $(CC65_HOME)/bin/cc65
  CL = $(CC65_HOME)/bin/cl65
  LD = $(CC65_HOME)/bin/ld65
else
  AS := $(if $(wildcard ../../bin/ca65*),../../bin/ca65,ca65)
  CC := $(if $(wildcard ../../bin/cc65*),../../bin/cc65,cc65)
  CL := $(if $(wildcard ../../bin/cl65*),../../bin/cl65,cl65)
  LD := $(if $(wildcard ../../bin/ld65*),../../bin/ld65,ld65)
endif

EXELIST_apple2 = \
        uploadfont

ifneq ($(EXELIST_$(SYS)),)
all: $(EXELIST_$(SYS))
else
all: notavailable
endif

# empty target used to skip systems that will not work with any program in this dir
notavailable:
ifeq ($(MAKELEVEL),0)
	@echo "info: apple2 tests not available for" $(SYS)
else
# suppress the "nothing to be done for 'all' message
	@echo > $(NULLDEV)
endif

disk: font.dsk

font.dsk: uploadfont
	cp prodos/prodos.dsk $@
	java -jar $(AC) -n   $@ FONTDISK
	java -jar $(AC) -p   $@ BASIC.SYSTEM     SYS 0x2000  <prodos/basic.system
	java -jar $(AC) -p   $@ BITSY.BOOT       SYS 0x2000  <prodos/bitsy.boot
	java -jar $(AC) -p   $@ QUIT.SYSTEM      SYS 0x2000  <prodos/quit.system
	java -jar $(AC) -as  $@ UPLOADFONT                   <uploadfont
	java -jar $(AC) -p   $@ IIC.FONT         BIN 0x2000  <fonts/iicfont.bin
	java -jar $(AC) -p   $@ IIPLUS.FONT      BIN 0x2000  <fonts/iiplus.bin
	java -jar $(AC) -p   $@ LOWERCASE.FONT   BIN 0x2000  <fonts/lcase.bin
	java -jar $(AC) -p   $@ PIG.FONT         BIN 0x2000  <fonts/pigfont.bin
	java -jar $(AC) -p   $@ MOUSETEXT.FONT   BIN 0x2000  <fonts/iiemouse.bin
	java -jar $(AC) -p   $@ REACTIVE.FONT    BIN 0x2000  <fonts/reactive.bin
	java -jar $(AC) -bas $@ IICFONT.BAS                  <prodos/iicfont.bas
	java -jar $(AC) -bas $@ IIPLUS.BAS                   <prodos/iiplus.bas
	java -jar $(AC) -bas $@ LOWERCASE.BAS                <prodos/lowercase.bas
	java -jar $(AC) -bas $@ PIGFONT.BAS                  <prodos/pigfont.bas
	java -jar $(AC) -bas $@ MOUSETEXT.BAS                <prodos/mousetext.bas
	java -jar $(AC) -bas $@ REACTIVE.BAS                 <prodos/reactive.bas

uploadfont: uploadfont.c
	$(CL) -Oirs -t apple2 --start-addr 0x4000 -m uploadfont.map $^

clean:
	@$(DEL) font.dsk 2>$(NULLDEV)
	@$(DEL) uploadfont uploadfont.map 2>$(NULLDEV)
