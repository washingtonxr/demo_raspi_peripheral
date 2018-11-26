objects = main.o BMP085.o
output  = gy801
cc      = gcc
ld      = -lbcm2835 -lm

build:$(output)

$(output):$(objects)
	$(cc) -o $(output) $(objects) $(ld)

main.o:main.h
	$(cc) -c main.c

BMP085.o:BMP085.h
	$(cc) -c BMP085.c
    
.PHONY:clean
clean:
	rm -rf $(output) $(objects)
