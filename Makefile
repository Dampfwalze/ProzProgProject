# Include directorys
COMMON_INCLUDES = -I"src/header"
WIN_INCLUDES = -I"dependencies\SDL2-WIN\include"
UNIX_INCLUDES =

# Linker options
COMMON_LFLAGS =
WIN_LFLAGS =-L"dependencies\SDL2-WIN\lib" -lmingw32 -lSDL2main -lSDL2 -lopengl32
UNIX_LFLAGS =

# Makefile settings
APPNAME = myapp
SRCDIR := src/source
OBJDIR := obj
BINDIR := bin

# Compiler settings
CC=gcc
CFLAGS:=-std=c11 -Wall -fdiagnostics-color=always
LDFLAGS =

# Plattform specific commands
ifeq ($(OS),Windows_NT) 
    MK="./winmkdir.bat"
	RM="./winrmdir.bat"
	CP=copy
else
    MK=mkdir -p
	RM=rm -r -f
	CP=cp
endif


rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard,$(SRCDIR),*.c)
OBJ = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))
OBJDIRS = $(sort $(dir $(OBJ)))

.PHONY: all
all: win-debug

####### win debug #######
.PHONY: win-debug
win-debug:
	@mingw32-make $(APPNAME) "OBJDIR=$(OBJDIR)/win/debug" "BINDIR=$(BINDIR)/win/debug" "CFLAGS=$(CFLAGS) $(COMMON_INCLUDES) $(WIN_INCLUDES) -g" "LDFLAGS=$(LDFLAGS) $(COMMON_LFLAGS) $(WIN_LFLAGS)"
	$(CP) $(subst /,\,"dependencies/SDL2-WIN/bin/SDL2.dll" "$(BINDIR)/win/debug")

###### win release ######
.PHONY: win-release
win-release:
	@mingw32-make $(APPNAME) "OBJDIR=$(OBJDIR)/win/release" "BINDIR=$(BINDIR)/win/release" "CFLAGS=$(CFLAGS) $(COMMON_INCLUDES) $(WIN_INCLUDES)" "LDFLAGS=$(LDFLAGS) $(COMMON_LFLAGS) $(WIN_LFLAGS)"
	$(CP) $(subst /,\,"dependencies/SDL2-WIN/bin/SDL2.dll" "$(BINDIR)/win/release")

###### unix debug #######
.PHONY: unix-debug
unix-debug:
	@make $(APPNAME) "OBJDIR=$(OBJDIR)/unix/debug" "BINDIR=$(BINDIR)/unix/debug" "CFLAGS=$(CFLAGS) $(COMMON_INCLUDES) $(UNIX_INCLUDES) -g" "LDFLAGS=$(LDFLAGS) $(COMMON_LFLAGS) $(UNIX_LFLAGS)"

##### unix release ######
.PHONY: unix-release
unix-release:
	@make $(APPNAME) "OBJDIR=$(OBJDIR)/unix/release" "BINDIR=$(BINDIR)/unix/release" "CFLAGS=$(CFLAGS) $(COMMON_INCLUDES) $(UNIX_INCLUDES)" "LDFLAGS=$(LDFLAGS) $(COMMON_LFLAGS) $(UNIX_LFLAGS)"

$(APPNAME): makedirs | $(OBJ)
	@$(MK) $(BINDIR)
	gcc $(CFLAGS) -o $(BINDIR)/$@ $| $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) $(LDFLAGS) $< -MM -MF $(patsubst %.o,%.d,$@) -MT $@
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

-include $(patsubst %.o,%.d,$(OBJ))

.PHONY: makedirs
makedirs:
	@$(MK) $(OBJDIRS)

.PHONY: clean
clean:
	$(RM) $(OBJDIR) $(BINDIR)
