SRC = ${wildcard *.c}
OBJ = ${patsubst %.c, %.out, $(SRC)}

all: $(OBJ)

$(OBJ): %.out : %.c
	cc $^ -o $@ 

clean:
	rm $(OBJ)

.PHONY:all clean