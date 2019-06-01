m       = HMC5883L
objects = main.o $(m).o
output  = gy801
cc      = gcc
ld      = -lbcm2835 -lm

build:$(output)

$(output):$(objects)
	$(cc) -o $(output) $(objects) $(ld)

main.o:main.h
	$(cc) -c main.c

$(m).o:$(m).h
	$(cc) -c $(m).c
    
.PHONY:clean
clean:
	rm -rf $(output) $(objects)
