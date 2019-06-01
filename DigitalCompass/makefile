m       = compass
output  = eCompass
objects = $(output).o adxl345.o hmc5883l.o

cc      = gcc
ld      = -lbcm2835 -lm

build:$(output)

$(output): $(objects)
	$(cc) -o $(output) $(objects) $(ld)

$(output).o:
	$(cc) -c $(output).c $(output).h

adxl345.o:
	$(cc) -c adxl345.c adxl345.h

hmc5883l.o:
	$(cc) -c hmc5883l.c hmc5883l.h

.PHONY:clean
clean:
	rm -rf $(output) $(objects)
# End of this file.