all: bin/openCalendar

bin/openCalendar: src/gtkwrapper.o
	gcc  -g src/gtkwrapper.o -o bin/openCalendar `pkg-config --cflags --libs gtk4`

src/gtkwrapper.o: src/gtkwrapper.c
	gcc  -c src/gtkwrapper.c -o src/gtkwrapper.o `pkg-config --cflags --libs gtk4`

clean:
	rm -rf src/*.o bin/openCalendar