**Notices:**This is a demostration drivers for Raspi3 B.
Create a project named  electronic compass.
**Author:**Washington Ruan

**E-mail:**washingtonxr@gmail.com

**Driver list:**

1. PRE:EE->1110 1110->77
1. ACC:A6->1010 0110->53
1. HMC:3C->1100 0011->1E
1. GYR:D2->1101 0010->69
1. M06150:..........->3C
1. KX022:...........->1e

For I2C driver like this:

> $ i2cdetect -y 1

1. 00:          -- -- -- -- -- -- -- -- -- -- -- -- -- 
1. 10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- 1e -- 
1. 20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
1. 30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
1. 40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
1. 50: -- -- -- 53 -- -- -- -- -- -- -- -- -- -- -- -- 
1. 60: -- -- -- -- -- -- -- -- -- 69 -- -- -- -- -- -- 
1. 70: -- -- -- -- -- -- -- 77


