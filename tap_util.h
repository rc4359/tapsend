#ifndef TAP_UTIL_H
#define TAP_UTIL_H


#define TAP_CHECKSUM_LEN    3

/* TAP BYTE DEFINE */
#define TAP_START_BYTE   0x02
#define TAP_STOP_BYTE    0x03
#define TAP_CR_BYTE      0x0D

int check_sum_gen(unsigned char *data, unsigned char len,
    unsigned char *chks);
#if 0
unsigned int tap_msg_gen(unsigned int hsid, unsigned char *msg, unsigned char *serial_buf);   
#else
unsigned int tap_msg_gen(char* hsid, unsigned char *msg, unsigned char *serial_buf);   
#endif
#endif
