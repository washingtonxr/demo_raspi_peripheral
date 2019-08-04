part	= LSM6DS3
objects = main.o $(part).o common.o
output  = $(part)
cc      = gcc
ld      = -lbcm2835 -lm

build:$(output)

$(output):$(objects)
	$(cc) -o $(output) $(objects) $(ld)

main.o:main.h
	$(cc) -c main.c

common.o:common.h
	$(cc) -c common.c

$(part).o:$(part).h
	$(cc) -c $(part).c
    
.PHONY:clean
clean:
	rm -rf $(output) $(objects)
