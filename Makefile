
#
# MAKEFILE
# following hiltmon.com/blog/2013/07/03/a-simple-c-plus-plus-project-structure/
#

# Compiler
CC := g++

# Directories
SRCDIR := src
BUILDDIR := build
TARGETDIR := bin

# Targets
EXECUTABLE := RunPlayground
TARGET := $(TARGETDIR)/$(EXECUTABLE)

# Code Lists
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

# Shared Compiler Flags
CFLAGS := -g -Wall -std=c++11
LIB := -L lib
INC := -I include

$(TARGET): $(OBJECTS)
	@mkdir -p $(TARGETDIR)
	@echo " Linking..."
	@echo " $(CC) $^ $@'.o' -o $(TARGET) $(LIB)"; $(CC) $^ $@'.o' -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

# Different run modes
#
RunPlayground: $(OBJECTS)
	$(CC) $(CFLAGS) run/RunPlayground.cpp $(INC) $(LIB) $^ -o bin/RunPlayground
#
RunDecoderThresholdCalc: $(OBJECTS)
	$(CC) $(CFLAGS) run/RunDecoderThresholdCalc.cpp $(INC) $(LIB) $^ -o bin/RunDecoderThresholdCalc
#
RunThermalCohTimeCalc: $(OBJECTS)
	$(CC) $(CFLAGS) run/RunThermalCohTimeCalc.cpp $(INC) $(LIB) $^ -o bin/RunThermalCohTimeCalc
#
RunThermalDecoderSuccessDecayCalc: $(OBJECTS)
	$(CC) $(CFLAGS) run/RunThermalDecoderSuccessDecayCalc.cpp $(INC) $(LIB) $^ -o bin/RunThermalDecoderSuccessDecayCalc

.PHONY: clean
