objects = main.o
output  = main
cc      = gcc
ld      = -lbcm2835 -lm

build:$(output)

$(output):$(objects)
	$(cc) -o $(output) $(objects) $(ld)

$(output).o:$(output).h
	$(cc) -c $(output).c

.PHONY:clean
clean:
	rm -rf $(output) $(output).o