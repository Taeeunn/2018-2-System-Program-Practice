/*-----------------------------------------------------------
 *
 * SWE2007: Software Experiment 2 (FALL 2018)
 *
 * Skeleton code for PA#1
 * 
 * September 17, 2018.
 * Computer Network Lab, Sungkyunkwan University
 *
 * Student Id   : 2017313008
 * Student Name : Kim Tae Eun
 *
 *-----------------------------------------------------------
 */

CC		= gcc
CFLAGS	= -g -O -Wall
LDFLAGS	=
RM		= rm

CSRCS	= main.c indexBuilder.c indexPrinter.c
TARGET	= indexBP
OBJECTS	= $(CSRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
		$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
		$(CC) $(CFLAGS) -c $< -o $@

clean:
		$(RM) -f $(OBJECTS) $(TARGET) *~
