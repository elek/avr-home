TARGET = $(notdir $(CURDIR))
BUILD_DIR := ../build
# Change this to match your arduino installation directory
PORT = /dev/ttyUSB0
UPLOAD_RATE = 57600
AVRDUDE_PROGRAMMER = arduino
#AVRDUDE_PROGRAMMER=usbasp

include $(BUILD_DIR)/jeenode
#include $(BUILD_DIR)/arduino-pro-mini-33
AVR_TOOLS_PATH = /usr/bin
AVRDUDE_PATH =/usr/bin
AVR_LIB=/home/eszti/projects/avr2/libs

#include "$(CURDIR)/local.mk"
LIB_MODULES = uart/uart.c

C_MODULES = \
lib.c \


#Note that if your program has dependencies other than those
#already listed below, you will need to add them accordingly.

CXX_MODULES = \


MODULES = $(C_MODULES) $(CXX_MODULES)
SRC = $(C_MODULES)
CXXSRC = $(CXX_MODULES) 
FORMAT = ihex

# Name of this Makefile (used for "make depend").
MAKEFILE = Makefile

# Debugging format.
# Native formats for AVR-GCC's -g are stabs [default], or dwarf-2.
# AVR (extended) COFF requires stabs, plus an avr-objcopy run.
#DEBUG = stabs
DEBUG =

OPT = s

# Place -D or -U options here
CDEFS = -DF_CPU=$(F_CPU)L 
CXXDEFS = -DF_CPU=$(F_CPU)L

# Place -I options here
CINCS = -I$(AVR_LIB)
CXXINCS = 

# Compiler flag to set the C Standard level.
# c89   - "ANSI" C
# gnu89 - c89 plus GCC extensions
# c99   - ISO C99 standard (not yet fully implemented)
# gnu99 - c99 plus GCC extensions
CSTANDARD = -std=gnu99
CDEBUG = -g$(DEBUG)
#CWARN = -Wall -Wstrict-prototypes
#CWARN = -Wall   # show all warnings
CWARN = -w #suppress all warnings
####CTUNING = -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CTUNING = -ffunction-sections -fdata-sections
CXXTUNING = -fno-exceptions -ffunction-sections -fdata-sections
#CEXTRA = -Wa,-adhlns=$(<:.c=.lst)

CFLAGS = $(CDEBUG) -O$(OPT) $(CWARN) $(CTUNING) $(CDEFS) $(CINCS) $(CSTANDARD) $(CEXTRA)
CXXFLAGS = $(CDEBUG) -O$(OPT) $(CWARN) $(CXXTUNING) $(CDEFS) $(CINCS)
#ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-gstabs
LDFLAGS = -O$(OPT) -lm -Wl,--gc-sections


# Programming support using avrdude. Settings and variables.
AVRDUDE_PORT = $(PORT)
#AVRDUDE_WRITE_FLASH = -U flash:w:applet/main.hex

# Program settings
CC = $(AVR_TOOLS_PATH)/avr-gcc
CXX = $(AVR_TOOLS_PATH)/avr-g++
LD = $(AVR_TOOLS_PATH)/avr-gcc
OBJCOPY = $(AVR_TOOLS_PATH)/avr-objcopy
OBJDUMP = $(AVR_TOOLS_PATH)/avr-objdump
AR  = $(AVR_TOOLS_PATH)/avr-ar
SIZE = $(AVR_TOOLS_PATH)/avr-size
NM = $(AVR_TOOLS_PATH)/avr-nm
AVRDUDE = $(AVRDUDE_PATH)/avrdude
REMOVE = rm -rf
MV = mv -f

# Define all object files.
OBJ = $(SRC:.c=.o) $(CXXSRC:.cpp=.o) $(ASRC:.S=.o)
OBJ_MODULES = $(C_MODULES:.c=.o) $(CXX_MODULES:.cpp=.o)

OBJ_LIB_MODULES = $(LIB_MODULES:.c=.libo)

# Define all listing files.
LST = $(ASRC:.S=.lst) $(CXXSRC:.cpp=.lst) $(SRC:.c=.lst)

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = $(CFLAGS) -mmcu=$(MCU)
ALL_CXXFLAGS = $(CXXFLAGS) -mmcu=$(MCU)
ALL_ASFLAGS = -x assembler-with-cpp $(ASFLAGS) -mmcu=$(MCU)
ALL_LDFLAGS = $(LDFLAGS) -mmcu=$(MCU)

# Default target.
all: applet_files build sizeafter


hex: main.hex

%.upload : %.hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH) -U flash:w:applet/$<

sizebefore:
	@if [ -f applet/main.elf ]; then echo; echo $(MSG_SIZE_BEFORE); $(HEXSIZE); echo; fi

sizeafter:
	@if [ -f applet/main.elf ]; then echo; echo $(MSG_SIZE_AFTER); $(HEXSIZE); echo; fi


.SUFFIXES: .elf .hex .eep .lss .sym .axf

.elf.hex:
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< applet/$@

.elf.eep:
	$(OBJCOPY) -O $(FORMAT) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--no-change-warnings \
	--change-section-lma .eeprom=0 $< applet/$@

# Create extended listing file from ELF output file.
.elf.lss:
	$(OBJDUMP) -h -S $< > $@

# Create a symbol table from ELF output file.
.elf.sym:
	$(NM) -n $< > $@

%.elf: %.o applet/core.a 
	$(LD) $(ALL_LDFLAGS) -o $@ $< applet/core.a 

.c.axf:
	$(CC) -Wall -gdwarf-2 -Os\
        -mmcu=$(MCU) \
	-fno-inline-small-functions -ffunction-sections -fdata-sections \
        -Wl,--relax,--gc-sections \
        -Wl,--undefined=_mmcu,--section-start=.mmcu=0x910000 \
	$< -o $@ \

applet:
	mkdir -p applet

applet/core.a: $(OBJ_MODULES) applet
	@for i in $(OBJ_MODULES); do echo $(AR) rcs applet/core.a $$i; $(AR) rcs applet/core.a $$i; done


# Compile: create object files from C source files.
.c.libo:
	$(CC) -c $(ALL_CFLAGS) $(AVR_LIB)/$< -o applet/$@ 

# Compile: create object files from C source files.
.c.o:
	$(CC) -c $(ALL_CFLAGS) $< -o $@ 


# Compile: create assembler files from C source files.
.c.s:
	$(CC) -S $(ALL_CFLAGS) $< -o $@


# Assemble: create object files from assembler source files.
.S.o:
	$(CC) -c $(ALL_ASFLAGS) $< -o $@


# Automatic dependencies
%.d: %.c
	$(CC) -M $(ALL_CFLAGS) $< | sed "s;$(notdir $*).o:;$*.o $*.d:;" > $@

%.d: %.cpp
	$(CXX) -M $(ALL_CXXFLAGS) $< | sed "s;$(notdir $*).o:;$*.o $*.d:;" > $@


# Target: clean project.
clean:
	$(REMOVE) applet
	$(REMOVE) *.o

.PHONY:	all build elf hex eep lss sym program coff extcoff clean applet_files sizebefore sizeafter

#include $(SRC:.c=.d)
#include $(CXXSRC:.cpp=.d)
