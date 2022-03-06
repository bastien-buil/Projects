CC=gcc
CFLAGS= -g -Wall -Wextra -pedantic -c
CPPFLAGS=-I./include/ -DMUTEX $(shell pkg-config --cflags gtk+-3.0)
LDFLAGS=-lpthread -lrt
LDFLAGSWINDOW=-lpthread -lrt $(shell pkg-config --libs gtk+-3.0)

INCLUDE_PATH=./include/
SRC_PATH=./src/
OBJSPATH= ./build/obj/

CITIZEN = ./build/citizenManager
TIMER = ./build/timer
EPIDEMIC = ./build/epidemicSim
PRESS = ./build/pressAgency
WINDOW = ./build/window

EXEC= $(CITIZEN) $(TIMER) $(EPIDEMIC) $(PRESS) $(WINDOW)
TOBUILD = $(CITIZEN) $(EPIDEMIC) $(PRESS) $(WINDOW)
UTILS =  $(OBJSPATH)utils.o $(OBJSPATH)doctorUtils.o $(OBJSPATH)printInformations.o $(OBJSPATH)pcg_basic.o

CONSOLE_MODE=off
SILENCE_MODE=off

ifeq ($(CONSOLE_MODE),on)
    ifeq ($(SILENCE_MODE),on)
    	$(error Cant have two modes)
    else ifeq ($(SILENCE_MODE),off)
    	CPPFLAGS+=-DCONSOLE_MODE
    	TOBUILD = $(CITIZEN) $(TIMER) $(EPIDEMIC) $(PRESS)
    endif
else ifeq ($(SILENCE_MODE),on)
	CPPFLAGS+=-DSILENCE_MODE
	TOBUILD = $(CITIZEN) $(TIMER) $(EPIDEMIC) $(PRESS)
endif

.PHONY: all clean mrproper

all: $(TOBUILD)

$(EPIDEMIC): $(OBJSPATH)epidemicSim.o $(OBJSPATH)utils.o $(OBJSPATH)pcg_basic.o
	$(CC) $^ -o $@ $(LDFLAGS)

$(CITIZEN): $(OBJSPATH)citizenManager.o $(UTILS)
	$(CC) $^ -o $@ $(LDFLAGS)

$(TIMER): $(OBJSPATH)timer.o $(OBJSPATH)utils.o $(OBJSPATH)pcg_basic.o
	$(CC) $^ -o $@ $(LDFLAGS)

$(PRESS): $(OBJSPATH)pressAgency.o $(OBJSPATH)utils.o $(OBJSPATH)pcg_basic.o
	$(CC) $^ -o $@ $(LDFLAGS)

$(WINDOW): $(OBJSPATH)mainWindow.o $(OBJSPATH)windowConnector.o $(OBJSPATH)utils.o $(OBJSPATH)pcg_basic.o
	$(CC) $^ -o $@ $(LDFLAGSWINDOW)

$(OBJSPATH)utils.o: $(SRC_PATH)utils.c $(INCLUDE_PATH)utils.h $(INCLUDE_PATH)pcg_basic.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@

$(OBJSPATH)epidemic.o: $(SRC_PATH)epidemic.c $(INCLUDE_PATH)define.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@

$(OBJSPATH)%.o: $(SRC_PATH)%.c $(INCLUDE_PATH)%.h
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJSPATH)*.o

mrproper: clean
	rm -f $(EXEC)


#$(EXEC): $(OBJS)
#	$(CC) $^ -o $@ $(LDFLAGS)
