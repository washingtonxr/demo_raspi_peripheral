module  = KX022
output  = main

cc      = gcc
ld      = -lbcm2835 -lm
objects = $(output).o $(module).o

build:$(output)

$(output):$(objects)
	$(cc) -o $(output) $(objects) $(ld)

$(output).o:$(output).h
	$(cc) -c $(output).c

$(module).o:$(module).h
	$(cc) -c $(module).c
    
.PHONY:clean
clean:
	rm -rf $(output) $(objects)
