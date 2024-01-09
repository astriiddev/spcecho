#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "readwrite.h"

/* SPC700 DSP registers begin at 10100h */
#define ADDRESS_OFFSET 0x10100

static char *fileBuffer = NULL;
static size_t fileLength = 0;

typedef enum EchoControls
{
    LEFT_VOL  = 0,
    RIGHT_VOL = 1,
    FEEDBACK  = 2,
    ECHO_SPD  = 3,
    ECHO_CHNL = 4,
    ECHO_ADDR = 5

} echo_controls_t;

typedef struct SpcAddress
{
    int  address;
    unsigned char value;
    char overwrite;

} spcaddress_t;

static spcaddress_t spcAddresses[6] =
{
    { ADDRESS_OFFSET + 0x2C, 0, 0 },    /* left channel echo volume address  : 1012Ch */
    { ADDRESS_OFFSET + 0x3C, 0, 0 },    /* right channel echo volume address : 1013Ch */
    { ADDRESS_OFFSET + 0x0D, 0, 0 },    /* echo feedback level address : 1010Dh */
    { ADDRESS_OFFSET + 0x7D, 0, 0 },    /* echo speed address : 1017Dh */
    { ADDRESS_OFFSET + 0x4D, 0, 0 },    /* channel echo enable/mute address : 1014Dh */
    { ADDRESS_OFFSET + 0x6D, 0, 0 },    /* echo buffer address : 1016Dh */
};

static int readAddresses(void)
{
    int i = 0;
    if(fileLength < ADDRESS_OFFSET + 0x80) return 0;

    for(; i < 6; i++)
    {
        if(spcAddresses[i].overwrite) continue;
        spcAddresses[i].value = fileBuffer[spcAddresses[i].address];
    }

    return 1;
}

static int writeAddresses(void)
{
    int i = 0;
    if(fileLength < ADDRESS_OFFSET + 0x80) return 0;

    for(; i < 6; i++)
    {
        if(!spcAddresses[i].overwrite) continue;
        fileBuffer[spcAddresses[i].address] = spcAddresses[i].value;
    }

    return 1;
}

int fileRead(const char* spcName)
{
    FILE* spcRead;
    char filepath[128];

    if(strstr(spcName, ".spc") == NULL) sprintf(filepath, "%s.spc", spcName);
    else sprintf(filepath, "%s", spcName);

    if((spcRead = fopen(filepath, "rb")) == NULL)
    {
        printf("Unable to open file!\nFile in use or not found!\n");
        return 0;
    }
    
    fseek(spcRead, 0, SEEK_END);
    fileLength = ftell(spcRead);
    rewind(spcRead);

    fileBuffer = malloc(fileLength * sizeof *fileBuffer);

    if(fileBuffer == NULL)
    {
        printf("Unable to allocate memory for file!\n");
        fclose(spcRead);
        return 0;
    }

    if(fread(fileBuffer, 1, fileLength, spcRead) < fileLength)
    {
        printf("Error while reading input file!\n");

        freeBuffer();
        fclose(spcRead);

        return 0;
    }

    fclose(spcRead);
    

    if(!readAddresses())
    {
        printf("Error while reading SPC addresses!\n");
        freeBuffer();

        return 0;
    }

    return 1;
}

static int getResponse(const char* msg)
{
	char proceed = '\0', ignore = '\0';

	printf("%s", msg);

    proceed = (char)toupper(getchar());

	/* ignores all other input other than y/Y and n/N
	 * -- necessary for recursion as to ignore '\n' -- */
	if (proceed == '\n')
		ignore = proceed;

	while (ignore != '\n')
		ignore = getchar();
	
	if (proceed == 'Y') return 1;
	else if (proceed == 'N') return 0;
	else return getResponse(msg);
}

static int underflowCheck(void)
{
        if(!getResponse("\nRaise echo buffer address? (y / n) "))  
        {
            printf("\nEcho buffer underflow can overwrite music data and cause unexpected and/or unwanted glitches in SPC playback.");
            return (getResponse("\nProceed anyway ? (y / n) "));
        }

        return 1;

}

int echoAddress(void)
{
    int i = 0xFFFF;
    unsigned char echoAddy = 0;

    if(fileBuffer == NULL)
    {
        printf("Unexpected error with allocated memory!\n");
        return 0;
    } 
    
    for (; i > 0; i--)
    {
        if (fileBuffer[i] == (char) 0x00) continue;
        if (fileBuffer[i] == (char) 0xFF) continue;

        echoAddy = ((i & ~0x00FF) >> 8) + 1;

        break;
    }

    if(spcAddresses[ECHO_ADDR].overwrite)
    {
        if(echoAddy > spcAddresses[ECHO_ADDR].value)
        {
            printf("\nCAUTION: buffer underflow detected! Consider raising buffer address to: 0x%02X00\n", echoAddy);

            if(underflowCheck()) 
                spcAddresses[ECHO_ADDR].value = echoAddy;
            else 
                return 0;
        }
    }
    else
    {
        printf("\nEnd of audio data found at address: 0x%04X\n", i);

        printf("\nViable echo buffer address at: 0x%02X00\n", echoAddy);

        spcAddresses[ECHO_ADDR].value = echoAddy;
        spcAddresses[ECHO_ADDR].overwrite = 1;
    }

    return 1;
}

void valueSet(char v, int controlValue)
{
    switch(v)
    {
    case 'l':

        spcAddresses[LEFT_VOL].value = controlValue;
        spcAddresses[LEFT_VOL].overwrite = 1;
        break;

    case 'r':

        spcAddresses[RIGHT_VOL].value = controlValue;
        spcAddresses[RIGHT_VOL].overwrite = 1;
        
        break;

    case 'f':

        spcAddresses[FEEDBACK].value = controlValue;
        spcAddresses[FEEDBACK].overwrite = 1;
        
        break;

    case 't':

        spcAddresses[ECHO_SPD].value = controlValue;
        spcAddresses[ECHO_SPD].overwrite = 1;

        break;

    case 'c':

        spcAddresses[ECHO_CHNL].value = controlValue;
        spcAddresses[ECHO_CHNL].overwrite = 1;
        
        break;

    case 'a':

        spcAddresses[ECHO_ADDR].value = controlValue;
        spcAddresses[ECHO_ADDR].overwrite = 1;
        
        break;
    }
}

static int overflowCheck(void)
{
    /* Gives option to trunicate speed to fit buffer size, proceed with buffer overflow, or exit */
    int  truncatedSpeed = (0xFF - spcAddresses[ECHO_ADDR].value) >> 3;

    printf("\nCAUTION: buffer overflow detected! Consider lowering echo speed to: %dms", (truncatedSpeed << 4));
    
    if (getResponse("\nLower echo speed ? (y / n) "))
    {
        spcAddresses[ECHO_SPD].value = truncatedSpeed;
        return 1;
    }

    printf("\nEcho buffer overflow can cause unexpected and/or unwanted glitches in SPC playback.");
    return (getResponse("\nProceed anyway ? (y / n) "));
}

int fileWrite(const char* spcName)
{
    FILE* spcWrite;
    char filepath[128];

    if(fileBuffer == NULL) return 0;
    /* Checks for buffer overflow. Every 16 ms takes 2kb of buffer so echo buffer address + echo speed must be under end of SPC data */
    if ((((unsigned short)spcAddresses[ECHO_SPD].value << 11) + ((unsigned short)spcAddresses[ECHO_ADDR].value << 8)) > 0xFFFF)
        {
            if (!overflowCheck()) return 0;
        }

    if(strstr(spcName, ".spc") == NULL) sprintf(filepath, "%s.spc", spcName);
    else sprintf(filepath, "%s", spcName);

    if((spcWrite = fopen(filepath, "rb")) != NULL)
    {
        fclose(spcWrite);

        if(!getResponse("\nFile already exists! Overwrite file? (y / n) "))
        {
            freeBuffer();
            return 0;
        }
    }

    fclose(spcWrite);
    
    if(!writeAddresses())
    {
        printf("Error while writing SPC addresses!\n");
        freeBuffer();

        return 0;
    }

    /* set bit at value 20h mutes echo. clears bit while leaving other bits as they were */
    fileBuffer[ADDRESS_OFFSET + 0x6C] &= ~(1 << 5);

    if((spcWrite = fopen(filepath, "wb")) == NULL)
    {
        fclose(spcWrite);
        freeBuffer();

        printf("Unable to write %s!\n", filepath);
        return 0;
    }

    printf("\nWriting echo DSP registers for %s... saved!\n", filepath);

    if(fwrite(fileBuffer, 1, fileLength, spcWrite) < fileLength)
    {
        printf("Error while writing output file!\n");

        freeBuffer();
        fclose(spcWrite);

        return 0;
    }

    freeBuffer();

    return 1;
}

void freeBuffer(void)
{
    if(fileBuffer != NULL)
    {
        free(fileBuffer);
        fileBuffer = NULL;
    }

    if(fileBuffer != NULL) 
        printf("Error while freeing memory!\n");
}