CC = gcc

CFLAGS = -Wall
LDFLAGS = -lz

TARGET = Mallumo.exe
OBJS = mallumo.o swfreader.o binreader.o hexdump.o

$(TARGET) : $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS) $(CFLAGS)
	strip $(TARGET)

clean :
	rm -f $(TARGET) *.o *~

test : $(TARGET)
	.\\$(TARGET) testswf/test.swf > test_log.txt

mallumo.o : mallumo.c swfreader.h swftags.h hexdump.h
swfreader.o : swfreader.c swfreader.h binreader.h swftags.h
binreader.o : binreader.c binreader.h
hexdump.o : hexdump.c hexdump.h

