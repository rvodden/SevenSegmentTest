MCU=attiny85

FORMAT=ihex

OBJDIR=target
SRCDIR=src
TARGET=$(OBJDIR)/main

OPT=s
CFLAGS = -Wall -g

# Define all source files.
SRC=$(call rwildcard, $(SRCDIR), *.c)

# Define all object files.
OBJ=$(addprefix $(OBJDIR)/,$(SRC:%.c=%.o))

# Define header directories.
VPATH=$(call uniq,$(dir $(SRC)))
EXTRAINCDIRS=$(call uniq,$(dir $(call rwildcard,$(SRCDIR),*.h)))

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
SHELL = bash
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
HEAD = head
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
	@echo -n "Using "
	@$(CC) --version | $(HEAD) -1

# Program the device.  
program: $(TARGET).hex $(TARGET).eep
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH) $(AVRDUDE_WRITE_EEPROM)

coff: $(TARGET).elf
	$(COFFCONVERT) -O coff-avr $< $(TARGET).cof

extcoff: $(TARGET).elf
	$(COFFCONVERT) -O coff-ext-avr $< $(TARGET).cof

# Create final output files (.hex, .eep) from ELF output file.
%.hex: %.elf
	@echo -n Create final out file \($@\) from ELF file \($^\)...
	@$(OBJCOPY) -O $(FORMAT) -R .eeprom -R .fuse -R .lock -R .signature $< $@
	@echo done.

%.eep: %.elf
	@echo -n Create final output file \($@\) from ELF file \($^\)...
	@-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 --no-change-warnings -O $(FORMAT) $< $@ || exit 0
	@echo done.

# Create extended listing file from ELF output file.
%.lss: %.elf
	@echo -n Creating extended listing file \($@\) from ELF output file \($^\).
	@$(OBJDUMP) -h -S -z $< > $@
	@echo done.

# Create a symbol table from ELF output file.
%.sym: %.elf
	@echo -n Creating a symbol table \($@\) from ELF output \($^\)...
	@$(NM) -n $< > $@
	@echo done.

# Create library from object files.
.SECONDARY : $(TARGET).a
.PRECIOUS : $(OBJ)
%.a: $(OBJ)
	@echo -n Creating library ($@) from object files...
	@$(AR) $@ $(OBJ)
	@echo done.

# Link: create ELF output file from object files.
.SECONDEXPANSION:
.SECONDARY : $(TARGET).elf
.PRECIOUS : $$(OBJ)
%.elf: $$(OBJ)
	@echo -n Creating $@ from object files...
	@$(CC) $(ALL_CFLAGS) $^ --output $@ $(LDFLAGS)
	@echo done.

# Compile: create object files from C source files.
$(OBJDIR)/%.o : %.c
	@echo -n Compiling $^ to object file...
	@mkdir -p $(dir $@)
	@$(CC) -c $(ALL_CFLAGS) $< -o $@
	@echo done.
	
# Compile: create assembler files from C source files.
%.s : %.c
	@echo -n Compiling $@ to assembly file.
	@$(CC) -S $(ALL_CFLAGS) $< -o $@
	@echo done.

# Create preprocessed source for use in sending a bug report.
%.i : %.c
	$(CC) -E -mmcu=$(MCU) -I. $(CFLAGS) $< -o $@

# Helpful debugging target to show what is going on
show-dirs:
	@echo "SRC:"
	@echo $(SRC)
	@echo "OBJ:"
	@echo $(OBJ)
	@echo "INCLUDES:"
	@echo $(EXTRAINCDIRS)

# Target: clean project.
clean: begin clean_list end

clean_list :
	@echo -n "Cleaning..."
	@$(REMOVE) $(TARGET).hex
	@$(REMOVE) $(TARGET).eep
	@$(REMOVE) $(TARGET).cof
	@$(REMOVE) $(TARGET).elf
	@$(REMOVE) $(TARGET).map
	@$(REMOVE) $(TARGET).sym
	@$(REMOVE) $(TARGET).lss
	@$(REMOVE) $(OBJ)
	@$(REMOVE) $(OBJ:%.o=%.lst)
	@$(REMOVE) $(SRC:.c=.s)
	@$(REMOVE) $(SRC:.c=.d)
	@$(REMOVE) $(SRC:.c=.i)
	@$(REMOVEDIR) .dep
	@$(REMOVEDIR) $(OBJDIR)
	@echo "...done."

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
	$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))
endef

define uniq=
	$(if $1,$(firstword $1) $(call uniq,$(filter-out $(firstword $1),$1)))
endef

