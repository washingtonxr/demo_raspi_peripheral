part = BMM150
objects = main.o $(part).o
output  = gy801
cc      = gcc
ld      = -lbcm2835 -lm

build:$(output)

$(output):$(objects)
	$(cc) -o $(output) $(objects) $(ld)

main.o:main.h
	$(cc) -c main.c

$(part).o:$(part).h
	$(cc) -c $(part).c
    
.PHONY:clean
clean:
	rm -rf $(output) $(objects)
