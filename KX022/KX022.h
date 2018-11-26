#ifndef KX022_H
#define KX022_H
#define SlaveAddress    0x1e
#define Boundrate       100000
// This register is used to verify circuit functionality. It always is 0x55h while OK
#define COTR_reg        0x0C
#define txbuf_len       10

struct WriteReg1{
    unsigned char reg_addr;
    unsigned char data;
};

extern void Init_KX022();
extern void Start_bcm2835();
extern void Stop_bcm2835();
extern unsigned char kx022_read_regs(unsigned char register_address, unsigned char *destination, \
                                unsigned char number_of_bytes);
extern unsigned char kx022_write_regs(const unsigned char *register_address, const unsigned char *value, \
                                unsigned char number_of_bytes);

#endif

