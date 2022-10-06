#Environment settings

CC = g++
CFLAGS = -g -Wall

#Variables

ASM = asm
STACK  = stack
STR = stringUtils
FILE = fileUtils
MAIN = main
PROC = proc
MMAKE = mymake

#Body

all : $(MAIN)

$(MAIN)     : $(MMAKE).o $(PROC).o  $(ASM).o $(STACK).o $(STR).o $(FILE).o  
	$(CC) $(CFLAGS) $^ -o $@   

$(ASM).o   : $(ASM).cpp
	$(CC) $(CFLAGS) $< -c 

$(STACK).o  : $(STACK).cpp
	$(CC) $(CFLAGS) $< -c	

$(FILE).o  : $(FILE).cpp
	$(CC) $(CFLAGS) $< -c

$(STR).o  : $(STR).cpp
	$(CC) $(CFLAGS) $< -c  

$(PROC).o : $(PROC).cpp
	$(CC) $(CFLAGS) $< -c

$(MMAKE).o : $(MMAKE).cpp
	$(CC) $(CFLAGS) $< -c

clean:
	rm -rf *.o *.save $(MAIN)
