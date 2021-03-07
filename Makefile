# Very simple makefile for PC88-C.
#  Please use official GNU make installer 3.81!
#  i.e. do not use the one distributed with SGDK etc.

CC=sdcc
CFLAGS=-Isrc
PY=python3
DEL=rm

## USED SECTORS ON DISC ##
USEDSEC=0x5f
# If this number isn't correct, 
# the app won't load right!
# Make sure it's big enough!

## PROJECT FOLDER ##
PROJECT=examples/helloworld
# This can also be explicit on the commandline i.e.
# $ make PROJECT=myproj

## MEMORY LOCATIONS ##
STACK=0x80
DATA=0x100
CODE=0x4000
# Stack should stay in ZP.
# This is due to VRAM being in C000~.

88FLAGS=-mz80 --stack-loc $(STACK) --data-loc $(DATA) --code-loc $(CODE) --fomit-frame-pointer --no-std-crt0 

## DISC FILE NAME ##
APPNAME=app.d88

## EMULATOR EXECUTABLE ##
EMUEXE=C:\Users\Bent\Downloads\m88\m88x5.exe 

default: $(PROJECT)
	$(PY) tools/maked88.py $(APPNAME)
	$(PY) tools/hexer.py src/ipl.bin 0x2f $(USEDSEC)
	$(CC) $(88FLAGS) $(CFLAGS) $(PROJECT)/main.c
	$(PY) tools/fixboot.py src/ipl.bin 
	$(PY) tools/hex2bin.py main.ihx main.bin
	$(PY) tools/maked88.py $(APPNAME) src/ipl.bin 0 0 1
	$(PY) tools/maked88.py $(APPNAME) main.bin 0 0 2	
	$(DEL) *.ihx
	$(DEL) *.lk
	$(DEL) *.lst
	$(DEL) *.map
	$(DEL) *.noi
	$(DEL) *.rel
	$(DEL) *.sym
	$(EMUEXE)