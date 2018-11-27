#include <stdio.h>
#include <string.h>
#include <math.h>       /* For mathmatics lib. */
#include <stdlib.h>     /* For exit. */
#include <unistd.h>     /* For Getopt. */
#include <stdbool.h>
#include <errno.h>
#include "main.h"
#include "M01650.h"

static unsigned char P_Mode;

int parse_int(char option, char *arg_str, int *data)
{
    int rc;

    rc = sscanf(optarg, "%d", data);
    if (rc < 0) {
        perror("Unable to parse argument");
        return rc;
    } else if (rc != 1) {
        fprintf(stderr, "Error: Unable to parse argument '-%c %s' as an "
                "integer.\n", option, arg_str);
        return -EINVAL;
    }

    return 0;
}

static void print_usage(bool help)
{
    FILE* stream = (help) ? stdout : stderr;
    //double default_size;
    
    fprintf(stream, "Usage:\n\tmain [-v] [-p <Print Mode>] "
            "[-h] "
            "\n");
    if (!help) {
        return;
    }
    fprintf(stream,"----------------------------------\n");
    //default_size = BYTE_TO_MB(DEFAULT_TRANSFER_SIZE);
    fprintf(stream, "\t-h:\tShow help.\n");
    fprintf(stream, "\t-p:\tPrint Mode is %d.\n", P_Mode);
    fprintf(stream, "\t-v:\tShow Version:%s.\n", SW_Ver);
    fprintf(stream,"----------------------------------\n");                                  
    return;
}

static int parse_args(int argc, char **argv)
{
    //double double_arg;
    int int_arg;
    int option;
    printf("Argcs:%d\n",argc);
    while ((option = getopt(argc, argv, "hp:v")) != (char)-1)
    {   
        //printf("option = %d.\n",option);
        switch (option)
        {
            case 'h':
                print_usage(true);
                exit(0);
                break;
            case 'p':
                if (parse_int(option, optarg, &int_arg) < 0) {
                    print_usage(false);
                    return -EINVAL;
                }

                P_Mode = (unsigned char)int_arg;
                printf("Print type:%d\n", P_Mode);
                return 0;            
            case 'v':
                printf("Version:%s\n", SW_Ver);
                exit(0);
            default:
                print_usage(false);
                return -EINVAL;
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    float temperature;
    long presure;
    float altitude;
    int i = 0, j = 0;
    int ret;
    
    printf("Hello world.\n");
    
    if (parse_args(argc, argv) < 0) {
        printf("Parse Args Error.\n");
        return 0;
    }
    
    Start_bcm2835();

    Init_M01650();

    printf("The end Init_M01650.\n");
    if(P_Mode>0){
        
        for(i=0;i<1;i++){
            Clean_Screen();
            //sleep(1);
            Write_Sceen(&qcode5[0]);
            //sleep(1);
            //printf("Pic = %d.\n",i);
        }
    }
    Stop_bcm2835();
    printf("The end.\n");
    return 0;
}
















