#######################################################
##Utils
##
CP 		= cp -f
RM		= rm -rf

#######################################################
##Toolchain Var
##
CROSS_COMPILE=
AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CPP		= $(CROSS_COMPILE)g++
AR		= $(CROSS_COMPILE)ar
STRIP		= $(CROSS_COMPILE)strip
#######################################################
CUR_DIR = $(shell pwd)
INCFLAGS += -I./include
BASE_CFLAGS = -DLINUX -Wall -g -Wmissing-prototypes -D_GNU_SOURCE 
LIBFLAGS = -lpthread
TARGET = order_service
OBJS += order_main.o
OBJS += order_socket.o


####### Build rules
%.o: %.c
	$(CC) -c $(BASE_CFLAGS) $(INCFLAGS) $(CFLAGS) -o $@ $<
%.o: %.cpp
	$(CPP) -c $(BASE_CFLAGS) $(INCFLAGS) $(CFLAGS) -o $@ $<
	
$(TARGET):$(OBJS) 
#	$(CC) -static -o $(TARGET) $(OBJS) $(LIBFLAGS) 
	$(CC) -o $(TARGET) $(OBJS) $(LIBFLAGS) 

clean :
	rm -f $(TARGET) $(OBJS)
	@rm -rf .depend
	
.depend: $(wildcard *.h) $(wildcard *.c)
	@$(CC) $(INCFLAGS) $(CFLAGS) -MM *.c > .depend

-include .depend


