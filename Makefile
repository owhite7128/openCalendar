SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

all: bin/openCalendar

bin/openCalendar: $(OBJ)
	gcc -g $^ -o bin/openCalendar `pkg-config --cflags --libs gtk4`

src/%.o: src/%.c
	gcc -c $< -o $@ `pkg-config --cflags --libs gtk4`

f_build:
	make
	rm -rf src/*.o

clean:
	rm -rf src/*.o bin/openCalendar