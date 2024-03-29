
/**************************************************

file: demo_tx.c
purpose: simple demo that transmits characters to
the serial port and print them on the screen,
exit the program by pressing Ctrl-C

compile with the command: gcc demo_tx.c rs232.c -Wall -Wextra -o2 -o test_tx

**************************************************/

#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#include <pthread.h>
#endif

#include "rs232.h"
#include "tap_util.h"

int cport_nr = 0;        /* /dev/ttyS0 (COM1 on windows) */
int bdrate = 9600;            /* 9600 baud */

#ifndef _WIN32
pthread_t  rx_thread_id;
#endif

unsigned char buf[4096];

char test_str1[] = "Resident Call \0";
char test_str2[] = "Bed 110\0";
unsigned char tcr = 1;
unsigned char auto_test_str[128] = {0};
 

int success_flag = 0;


void* do_rx_data(void *arg)
{
    int n;
    int i;
    
    while(1)
    {
        n = RS232_PollComport(cport_nr, buf, 4095);

        if(n > 0)
        {
            buf[n] = 0;   /* always put a "null" at the end of a string! */

            for(i=0; i < n; i++)
            {
                if(buf[i] < 32)  /* replace unreadable control-codes by dots */
                {
                    buf[i] = '.';
                }
            }

            printf("received %i bytes: %s\n", n, (char *)buf);
		if(strcasestr(buf, "211") != NULL)
			success_flag = 1;
        }

#ifdef _WIN32
        Sleep(100);
#else
        usleep(100000);  /* sleep for 100 milliSeconds */
#endif
    }
}


int dump_rs232_port_msg(int nr)
{

	if(nr == 0)
	{
		printf("tap port : com1 \n");
	}
	else if((nr >= 16) && (nr <= 20))
	{
		printf("tap port : USB%d \n", nr - 16);
	}
	else
	{
		printf("tap port : ERROR !! \n");
	}
	return 0;
}
int main(int argc, char *argv[])
{
	int usb_port = 0;
  char mode[]={'8','N','1',0};
    int j = 0;
	char line[256] = {0};
	int hsid = 0;
	unsigned char bytes_to_send[256] = {0};
	char hsid_str[64] = {0};

  	if(argc > 1)
        {
                int tmp = 1;
                while(tmp < argc)
                {
                        if(strcmp(argv[tmp], "-u") == 0)
                        {
                                if((strlen(argv[tmp+1]) > 0))
                                {
					usb_port = atoi(argv[tmp+1]);
					
					if((usb_port < 0) ||  (usb_port > 5))
                                        {
						printf(" input usb port error !!");
    						return(0);
					}
					cport_nr = usb_port + 16;
                                }
                                else
                                {
                                        printf(" input dlf file name error !!\n");
                                        return -1;
                                }
                        }
			else if(strcmp(argv[tmp], "-h") == 0)
                        {
                                printf(" ===== tap send usage ========================= \n");
                                printf(" RS232 port fixed COM1 \n");
                                printf(" -u : change to USB port (0 ~5) . example -u 3 for usb3 \n");
                                printf(" ==================================================== \n");
    				return(0);
                        }
                        tmp++;

		}

	}

	dump_rs232_port_msg(cport_nr);
  if(RS232_OpenComport(cport_nr, bdrate, mode))
  {
    printf("Can not open comport\n");

    return(0);
  }

#ifdef _WIN32
    HANDLE thread = CreateThread(NULL, 0, &do_rx_data, NULL, 0, NULL);
     if (thread) {
          printf("\ncan't create thread");
     }
     else{
         printf("\n Thread created successfully\n");
     }
#else
    int err = pthread_create(&rx_thread_id, NULL, &do_rx_data, NULL);
     if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
    else
            printf("\n Thread created successfully\n");
#endif  

	printf(" Input handset ID :") ;  
        if (fgets(line, sizeof(line), stdin)) 

	{
#if 0
		if (1 != sscanf(line, "%d", &hsid)) 
		{
				printf("HS ID %d not availed!! \n", hsid);	
				return -1;
   			/* i can be safely used */
		}
#else

		if (1 != sscanf(line, "%s", hsid_str)) 
		{
				printf("HS ID %d not availed!! \n", hsid);	
				return -1;
   			/* i can be safely used */
		}
#endif
	}

	printf(" Input message to send :") ;
	memset(line, 0, sizeof(line));
	if (fgets(line, sizeof(line), stdin)) 
	{

	}

#if 0
   unsigned int tap_len = tap_msg_gen(hsid, (unsigned char *)line ,bytes_to_send);
#else
   unsigned int tap_len = tap_msg_gen(hsid_str, (unsigned char *)line ,bytes_to_send);
#endif


   RS232_cputs(cport_nr, (const char *)bytes_to_send);
   printf(" TAP Raw data --> ");
   for(j = 0; j < tap_len; j++)
   {
       printf(" %02x", bytes_to_send[j]);
   }
   printf("\n");
   //printf("sent: %s\n", bytes_to_send);


	unsigned int ack_time_out = 0;

	while(1)
	{
#ifdef _WIN32
        Sleep(1000);  // 1 sec
#else
        usleep(1000000);  
#endif
	if(++ack_time_out > 5)
		break;

	if(success_flag)
		break;

	}

	if(success_flag == 0)
		printf(" Failed!!, Pager can't send your message !! \n");
  return(0);
}

