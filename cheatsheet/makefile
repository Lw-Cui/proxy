SRC = ${wildcard *.c}
TAR = ${patsubst %.c, %.out, $(SRC)}

all: $(TAR)

$(TAR): %.out: %.c
	cc $^ -o $@ -g

clean:
	rm $(TAR)

.PHONY:all clean