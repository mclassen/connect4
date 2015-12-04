## -*- Makefile -*-
##
## User: classenm
## Time: Nov 23, 2015 11:31:20 AM
## Makefile created by Oracle Solaris Studio.
##
## This file is generated automatically.
##


#### Compiler and tool definitions shared by all build targets #####
CCC = g++
CXX = g++
BASICOPTS = -g -m64
CCFLAGS = $(BASICOPTS)
CXXFLAGS = $(BASICOPTS)
CCADMIN = 


# Define the target directories.
TARGETDIR_connect4=GNU-amd64-Windows
TARGETDIR_debug=GNU-amd64-Windows


all: $(TARGETDIR_connect4)/connect4 $(TARGETDIR_debug)/debug

## Target: connect4
OBJS_connect4 =  \
	$(TARGETDIR_connect4)/board.o \
	$(TARGETDIR_connect4)/command.o \
	$(TARGETDIR_connect4)/hash.o \
	$(TARGETDIR_connect4)/main.o \
	$(TARGETDIR_connect4)/search.o
SYSLIBS_connect4 = -static -lm 
USERLIBS_connect4 = $(SYSLIBS_connect4) 
DEPLIBS_connect4 =  
LDLIBS_connect4 = $(USERLIBS_connect4)

CPPFLAGS_connect4 = -O3 -m64

# Link or archive
$(TARGETDIR_connect4)/connect4: $(TARGETDIR_connect4) $(OBJS_connect4) $(DEPLIBS_connect4)
	$(LINK.cc) $(CCFLAGS_connect4) $(CPPFLAGS_connect4) -o $@ $(OBJS_connect4) $(LDLIBS_connect4)


# Compile source files into .o files
$(TARGETDIR_connect4)/board.o: $(TARGETDIR_connect4) board.cpp
	$(COMPILE.cc) $(CCFLAGS_connect4) $(CPPFLAGS_connect4) -o $@ board.cpp

$(TARGETDIR_connect4)/command.o: $(TARGETDIR_connect4) command.cpp
	$(COMPILE.cc) $(CCFLAGS_connect4) $(CPPFLAGS_connect4) -o $@ command.cpp

$(TARGETDIR_connect4)/hash.o: $(TARGETDIR_connect4) hash.cpp
	$(COMPILE.cc) $(CCFLAGS_connect4) $(CPPFLAGS_connect4) -o $@ hash.cpp

$(TARGETDIR_connect4)/main.o: $(TARGETDIR_connect4) main.cpp
	$(COMPILE.cc) $(CCFLAGS_connect4) $(CPPFLAGS_connect4) -o $@ main.cpp

$(TARGETDIR_connect4)/search.o: $(TARGETDIR_connect4) search.cpp
	$(COMPILE.cc) $(CCFLAGS_connect4) $(CPPFLAGS_connect4) -o $@ search.cpp



## Target: debug
OBJS_debug =  \
	$(TARGETDIR_debug)/board.o \
	$(TARGETDIR_debug)/command.o \
	$(TARGETDIR_debug)/hash.o \
	$(TARGETDIR_debug)/main.o \
	$(TARGETDIR_debug)/search.o
SYSLIBS_debug = -static -lm 
USERLIBS_debug = $(SYSLIBS_debug) 
DEPLIBS_debug =  
LDLIBS_debug = $(USERLIBS_debug)


# Link or archive
$(TARGETDIR_debug)/debug: $(TARGETDIR_debug) $(OBJS_debug) $(DEPLIBS_debug)
	$(LINK.cc) $(CCFLAGS_debug) $(CPPFLAGS_debug) -o $@ $(OBJS_debug) $(LDLIBS_debug)


# Compile source files into .o files
$(TARGETDIR_debug)/board.o: $(TARGETDIR_debug) board.cpp
	$(COMPILE.cc) $(CCFLAGS_debug) $(CPPFLAGS_debug) -o $@ board.cpp

$(TARGETDIR_debug)/command.o: $(TARGETDIR_debug) command.cpp
	$(COMPILE.cc) $(CCFLAGS_debug) $(CPPFLAGS_debug) -o $@ command.cpp

$(TARGETDIR_debug)/hash.o: $(TARGETDIR_debug) hash.cpp
	$(COMPILE.cc) $(CCFLAGS_debug) $(CPPFLAGS_debug) -o $@ hash.cpp

$(TARGETDIR_debug)/main.o: $(TARGETDIR_debug) main.cpp
	$(COMPILE.cc) $(CCFLAGS_debug) $(CPPFLAGS_debug) -o $@ main.cpp

$(TARGETDIR_debug)/search.o: $(TARGETDIR_debug) search.cpp
	$(COMPILE.cc) $(CCFLAGS_debug) $(CPPFLAGS_debug) -o $@ search.cpp



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR_connect4)/connect4 \
		$(TARGETDIR_connect4)/board.o \
		$(TARGETDIR_connect4)/command.o \
		$(TARGETDIR_connect4)/hash.o \
		$(TARGETDIR_connect4)/main.o \
		$(TARGETDIR_connect4)/search.o \
		$(TARGETDIR_debug)/debug \
		$(TARGETDIR_debug)/board.o \
		$(TARGETDIR_debug)/command.o \
		$(TARGETDIR_debug)/hash.o \
		$(TARGETDIR_debug)/main.o \
		$(TARGETDIR_debug)/search.o
	$(CCADMIN)
	rm -f -r $(TARGETDIR_connect4)
	rm -f -r $(TARGETDIR_debug)


# Create the target directory (if needed)
$(TARGETDIR_connect4):
	mkdir -p $(TARGETDIR_connect4)
$(TARGETDIR_debug):
	mkdir -p $(TARGETDIR_debug)


# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-amd64-Windows

