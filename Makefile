
all: 
	$(MAKE) -C belgolib
	$(MAKE) -C makelist
	$(MAKE) -C netmap

clean:
	$(MAKE) clean -C belgolib
	$(MAKE) clean -C makelist
	$(MAKE) clean -C netmap
