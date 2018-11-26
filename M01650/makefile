objects = main.o M01650.o
output  = main
cc      = gcc
ld      = -lbcm2835 -lm

build:$(output)

$(output):$(objects)
	$(cc) -o $(output) $(objects) $(ld)

main.o:main.h
	$(cc) -c main.c

M01650.o:M01650.h
	$(cc) -c M01650.c
    
.PHONY:clean
clean:
	rm -rf $(output) $(objects)
