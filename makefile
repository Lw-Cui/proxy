objects = proxy.o rio.o aux.o

proxy.out: $(objects)
	cc -o proxy.out $(objects)
$(objects): rio.h aux.h

.PHONY: clean
clean:
	rm proxy.out $(objects)