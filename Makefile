CFLAGS=-D_GNU_SOURCE -O0 -g -Wall -I./

all:
	gcc $(CFLAGS) -c camera.c -o camera.o
	gcc $(CFLAGS) -c image.c -o image.o
	gcc $(CFLAGS) -c light.c -o light.o
	gcc $(CFLAGS) -c object.c -o object.o
	gcc $(CFLAGS) -c ray.c -o ray.o
	gcc $(CFLAGS) -c shading.c -o shading.o
	gcc $(CFLAGS) main.c -o main camera.o image.o light.o \
		object.o ray.o shading.c -lpng -lm

clean:
	rm -rf *.o main
