MCU=attiny85

FORMAT=ihex

TARGET=main
OBJDIR=obj

OPT=s
CFLAGS = -Wall -g

# Define all source files.
SRC=$(call rwildcard,,*.c)

# Define all object files.
OBJ=$(addprefix $(OBJDIR)/,$(SRC:%.c=%.o))

# Define header directories.
VPATH=$(call uniq,$(dir $(SRC)))
EXTRAINCDIRS=$(call uniq,$(dir $(call rwildcard,,*.h)))


CSTANDARD= -std=gnu99

# C Options

CFLAGS = -g$(DEBUG)
CFLAGS += -O$(OPT)
CFLAGS += -funsigned-char
CFLAGS += -funsigned-bitfields
CFLAGS += -fpack-struct
CFLAGS += -fshort-enums
CFLAGS += -pedantic
CFLAGS += -Wall 
CFLAGS += -Wextra 
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wmissing-prototypes
CFLAGS += -fno-unit-at-a-time
CFLAGS += -Wundef
CFLAGS += -Wunreachable-code
CFLAGS += -Wsign-compare
CFLAGS += -Wa,-adhlns=$(<:%.c=$(OBJDIR)/%.lst)
CFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))
CFLAGS += $(CSTANDARD)

# Library Options

MATH_LIB=-lm

# Linker Options

LDFLAGS = -Wl,-Map=$(TARGET).map,--cref
LDFLAGS += $(patsubst %,-L%,$(EXTRALIBDIRS))
LDFLAGS += $(PRINTF_LIB) $(SCANF_LIB) $(MATH_LIB)

# Define programs and commands.
SHELL = sh
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
AR = avr-ar rcs
AVRDUDE = avrdude
REMOVE = rm -f
REMOVEDIR = rm -rf
NM = avr-nm
COPY = cp
BEAR = bear
MAKE = make

# Compiler flags to generate dependency files.
GENDEPFLAGS = -MMD -MP -MF .dep/$(@F).d

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS) $(GENDEPFLAGS)

# Default target.
all: gccversion sizebefore build sizeafter 

# Change the build target to build a HEX file or a library.
build: elf hex eep lss sym
#build: lib

elf: $(TARGET).elf
hex: $(TARGET).hex
eep: $(TARGET).eep
lss: $(TARGET).lss
sym: $(TARGET).sym
LIBNAME=lib$(TARGET).a
lib: $(LIBNAME)

# Display size of file.
HEXSIZE = $(SIZE) --target=$(FORMAT) $(TARGET).hex
ELFSIZE = $(SIZE) --mcu=$(MCU) --format=avr $(TARGET).elf

sizebefore:
	@if test -f $(TARGET).elf; then echo; echo $(MSG_SIZE_BEFORE); $(ELFSIZE); \
	2>/dev/null; echo; fi

sizeafter:
	@if test -f $(TARGET).elf; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE); \
	2>/dev/null; echo; fi

# Display compiler version information.
gccversion :
	@$(CC) --version

# Program the device.  
program: $(TARGET).hex $(TARGET).eep
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH) $(AVRDUDE_WRITE_EEPROM)

coff: $(TARGET).elf
	$(COFFCONVERT) -O coff-avr $< $(TARGET).cof

extcoff: $(TARGET).elf
	$(COFFCONVERT) -O coff-ext-avr $< $(TARGET).cof

# Create final output files (.hex, .eep) from ELF output file.
%.hex: %.elf
	$(OBJCOPY) -O $(FORMAT) -R .eeprom -R .fuse -R .lock -R .signature $< $@

%.eep: %.elf
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 --no-change-warnings -O $(FORMAT) $< $@ || exit 0

# Create extended listing file from ELF output file.
%.lss: %.elf
	$(OBJDUMP) -h -S -z $< > $@

# Create a symbol table from ELF output file.
%.sym: %.elf
	$(NM) -n $< > $@

# Create library from object files.
.SECONDARY : $(TARGET).a
.PRECIOUS : $(OBJ)
%.a: $(OBJ)
	$(AR) $@ $(OBJ)

# Link: create ELF output file from object files.
.SECONDEXPANSION:
.SECONDARY : $(TARGET).elf
.PRECIOUS : $$(OBJ)
%.elf: $$(OBJ)
	$(CC) $(ALL_CFLAGS) $^ --output $@ $(LDFLAGS)

# Compile: create object files from C source files.
$(OBJDIR)/%.o : %.c
	@mkdir -p $(dir $@)
	$(CC) -c $(ALL_CFLAGS) $< -o $@
	
# Compile: create assembler files from C source files.
%.s : %.c
	$(CC) -S $(ALL_CFLAGS) $< -o $@

# Create preprocessed source for use in sending a bug report.
%.i : %.c
	$(CC) -E -mmcu=$(MCU) -I. $(CFLAGS) $< -o $@

# Helpful debugging target to show what is going on
show-dirs:
	@echo $(SRC)
	@echo $(OBJ)
	@echo $(EXTRAINCDIRS)

# Target: clean project.
clean: begin clean_list end

clean_list :
	@echo
	@echo $(MSG_CLEANING)
	$(REMOVE) $(TARGET).hex
	$(REMOVE) $(TARGET).eep
	$(REMOVE) $(TARGET).cof
	$(REMOVE) $(TARGET).elf
	$(REMOVE) $(TARGET).map
	$(REMOVE) $(TARGET).sym
	$(REMOVE) $(TARGET).lss
	$(REMOVE) $(OBJ)
	$(REMOVE) $(OBJ:%.o=%.lst)
	$(REMOVE) $(SRC:.c=.s)
	$(REMOVE) $(SRC:.c=.d)
	$(REMOVE) $(SRC:.c=.i)
	$(REMOVEDIR) .dep
	$(REMOVEDIR) $(OBJDIR)

# Include the dependency files.
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

compile_commands.json:
	$(BEAR) $(MAKE)

# Listing of phony targets.
.PHONY : all begin finish end sizebefore sizeafter gccversion \
build elf hex eep lss sym coff extcoff \
clean clean_list program debug gdb-config show-dirs

# Make does not offer a recursive wildcard function, so here's one:
define rwildcard=
	$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
endef

define uniq=
	$(if $1,$(firstword $1) $(call uniq,$(filter-out $(firstword $1),$1)))
endef

