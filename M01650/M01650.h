#ifndef M01650_H
#define M01650_H
#define SlaveAddress    0x3C
#define Boundrate       1000000

#define Config_Addr     0x00
#define Data_Addr       0x40
#define Page_Full_No    16
#define Column_Full_No  64

extern void Clean_Screen();
extern void Init_M01650();
extern int Write_Sceen(unsigned char *data);
extern void Start_bcm2835();
extern void Stop_bcm2835();
#endif
