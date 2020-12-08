# Makefile for midifilter
# Created by Pieter Venter 5 Dec 2020

PROG = midifilter
CC = g++
CPPFLAGS = -g -Wall -I/usr/local/include/rtmidi
LDFLAGS = -lrtmidi
OBJS = midifilter.o

$(PROG) : $(OBJS)
	$(CC) -o $(PROG) $(OBJS) $(LDFLAGS)

midifilter.o :
	$(CC) $(LDFLAGS) $(CPPFLAGS) -c midifilter.cpp

clean:
	rm -f $(PROG) $(OBJS) 
