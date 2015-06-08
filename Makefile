# Any of action will be transmitted to Makefile in src dir
all clean:
	$(MAKE) -C src $@ 
