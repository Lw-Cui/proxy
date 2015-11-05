objects = proxy.o rio.o aux.o
flag = -g -c

proxy.out: $(objects)
	cc -o proxy.out $(objects)
	
$(objects): %.o: %.c
	cc $^ -o $@ $(flag)

.PHONY: clean
clean:
	rm proxy.out $(objects)