
all: mp751_POFF

mp751_POFF: mp751_POFF.o hid-libusb.o
	gcc mp751_POFF.o hid-libusb.o -o mp751_POFF -lusb-1.0 -lrt -lpthread

mp751_POFF.o: mp751_POFF.c
	gcc -c mp751_POFF.c
	
hid-libusb.o: hid-libusb.c 
	gcc -I/usr/include/libusb-1.0 -I. -c hid-libusb.c

clean:
	rm -f hid-libusb.o mp751_POFF.o mp751_POFF

.PHONY: clean