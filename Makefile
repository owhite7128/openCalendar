all: openCalendar

openCalendar: gtkwrapper.o
	gcc -g gtkwrapper.o -o openCalendar `pkg-config --cflags --libs gtk4`

gtkwrapper.o: gtkwrapper.c
	gcc -c gtkwrapper.c -o gtkwrapper.o `pkg-config --cflags --libs gtk4`

clean:
	rm -rf *.o openCalendar