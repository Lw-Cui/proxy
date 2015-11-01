objects = proxy.o rio.o aux.o

proxy: $(objects)
	cc -o proxy $(objects)
$(objects): rio.h aux.h

.PHONY: clean
clean:
	rm proxy $(objects)