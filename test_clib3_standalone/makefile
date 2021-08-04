CC = gcc
CFLAGS = -Wall -I src
LDFLAGS = -pthread -lcunit
TARGET = test_clib3_standalone

OBJ = src/main.o  \
	../clib3/src/timer.o \
	src/test_timer.o \
	../clib3/src/linked_list.o \
	src/test_linked_list.o \
	../clib3/src/tcpmsg.o \
	../clib3/src/tcpmsg_client.o \
	../clib3/src/tcpmsg_server.o \
	src/test_tcpmsg.o \
	../clib3/src/utils.o \
	src/test_utils.o


all: $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

clean:
	rm -f src/*.o
	rm -f ../clib3/src/*.o
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
