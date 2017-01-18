#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#include <string.h>
#endif
#include <stdio.h>
 
#include "typedefine.h"
#include "tap_util.h"



int check_sum_gen(unsigned char *data, unsigned char len,
    unsigned char *chks)
{
        unsigned char bFramCheckSum[TAP_CHECKSUM_LEN];
        int i = 0;
        unsigned char *p_data_buf = NULL;
        unsigned long lnSumofFram=0;
        
        
        p_data_buf = data;
        
        for(i=0; i < len; i++)
        {
            lnSumofFram += *p_data_buf;
            p_data_buf++;
        }
    
        for(i=0; i<TAP_CHECKSUM_LEN; i++)
        {
            bFramCheckSum[i]=(BYTE)((lnSumofFram&(0x000f<<(i*4)))>>(i*4));
            bFramCheckSum[i]+=0x30;
        }
        *chks = bFramCheckSum[2];
        chks++;
        *chks = bFramCheckSum[1];
        chks++;
        *chks = bFramCheckSum[0];
       
      //  printf("check sun = [%02x] [%02x] [%02x]", bFramCheckSum[0], bFramCheckSum[1], bFramCheckSum[2]);
        
        
        return 0;
}

unsigned int tap_msg_gen(unsigned int hsid, unsigned char *msg, unsigned char *serial_buf)
{
    unsigned char buf[256] = {0};
    char hsid_str[32] = {0};
    unsigned int i = 0;
    unsigned char chk_sum[4] = {0x0d, 0x0d, 0x0d, 0x0d};
    buf[i] = TAP_START_BYTE;
    i++;
    
    sprintf(hsid_str, "%d", hsid);
    
    strcpy((char *)&buf[i], hsid_str);
    i += strlen(hsid_str);
    
    buf[i] = TAP_CR_BYTE;
    i++;
    
    strcpy((char *)&buf[i], (const char *)msg);
    i += strlen((const char *)msg);
    
    buf[i] = TAP_CR_BYTE;
    i++;
    
    buf[i] = TAP_STOP_BYTE;
    i++;
    
    check_sum_gen((unsigned char *)buf, i, chk_sum);
    memcpy(&buf[i], chk_sum, sizeof(chk_sum));
    i+= sizeof(chk_sum);
    memcpy(serial_buf, buf, i + 1);
    
    return i;
    
}
