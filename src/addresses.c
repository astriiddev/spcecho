#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "addresses.h"

int channelAddress(const char* channelVal, int* val)
{
    int echoChannel = 0, i = 0;

    /* outputs error if not all 8 channels or if too many channels are inputted */
    if(strlen(channelVal) != 8)
    {
        printf("\n!!!! Invalid number of channels! !!!!\n");
        return 0;
    }

    for(; i < 8; i++)
    {
        char bitVal = (char) toupper(channelVal[i]);

        if(bitVal == 'X' || bitVal == '1')
	    {
	        echoChannel |= (1 << i);
	    }
        else if(bitVal != 'O' && bitVal != '0' )
	    {
            printf("\n!!!! Invalid channel input! !!!!\n");
            return 0;
	    }
    }

    *val = echoChannel;

    return 1;
}

int percentToSign(const char* percent, int *val)
{
    char *err;
    float percentVal = strtof(percent, &err);
    
    if(percentVal < -100.f) return 0;
    if(percentVal >  100.f) return 0;
    if(strcmp(err, "\0") != 0) return 0;

    *val = percentVal < 0 ? (int) (percentVal * 1.28f) : (int) (percentVal * 1.27f);

    return 1;
}

int millisecondToInt(const char *echoSpeed, int *val)
{
    char *err;
    unsigned long int timingVal = strtoul(echoSpeed, &err, 10);

    if(timingVal > 240)  return 0;
    if(strcmp(err, "\0") != 0) return 0;

    /* scales down time into 8-bit value, rounding to nearest bit */
    *val = (int) ((timingVal + 8) >> 4);

    return 1;
}

int bufferAddress(const char* bufferHex, int *val)
{
    char *err;
    unsigned int hexAddress = strtoul(bufferHex, &err, 16);

    if(hexAddress < 0x02)  return 0;
    if(hexAddress > 0xFF)  return 0;
    if(strcmp(err, "\0") != 0) return 0;

    *val = (int) hexAddress;
    
    return 1;
}
