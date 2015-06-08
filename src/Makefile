INCLUDES=$(shell pkg-config --cflags libusb-1.0)
LIBS=$(shell pkg-config --libs libusb-1.0) -lrt -lpthread
SOURCES=hid-libusb.c mp751ctl.c
OBJECTS=$(SOURCES:.c=.o)
TARGET=../mp751ctl

CFLAGS=-Wall $(INCLUDES) $(LIBS)

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) 

.PHONY: clean
