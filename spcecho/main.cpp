/* 
    SPC Echo Adder for Super Nintendo(tm)/Super Famicom(tm) music files (.spc files).

    Allows user to add, change, or edit echo controls to .spc files. Initially designed
    to be used with eKid's XM2SNES as I love creating samples and sequencing in 
    Fasttracker II but had no way of adding or controlling the converted .spc file with
    XM2SNES. Now the songs I create for the SNES have the full echo spacial depth goodness
    that we all remember from our childhood games!

    This allows you to control the volume for stereo left echo, volume for stereo right
    echo, feedback level, echo speed, channel mute/enable, and echo buffer address for
    the Super Nintendo's audio processor: the SPC700. To keep this as user-friendly as possible, 
    stereo left echo, stereo right echo, and feedback values are inputted as percentage. An 
    input of 100 for stereo left will give you 100% volume (or input of -100 will give you 100% 
    volume of phase-inverted delay) for stereo left echo! Likewise, the echo time has a value
    range of 0-240ms. The SPC700 only can increment by 16ms, so any value not a multiple of
    16ms will be rounded to the nearest 16ms. The echo buffer has a range of 02h-FFh and gets
    multiplied by 100h for the actual address (eg. an input of C0h will put the buffer address
    starting at $C000).

    The echo buffer address is the tricky part. Both the music data buffer and echo buffer total
    must be under ~57kb. Every 16ms of echo takes up 2kb of the echo buffer and the echo buffer
    cannot exceed address $FFFFh. Too low of a value for the address and the echo buffer will cut 
    into the music data buffer and cause the SPC player to glitch out. Similarly, too high of a 
    value for the address with too high a value for the echo timing will cause a buffer overrun 
    and will also cause your SPC player to glitch out. You can set the buffer address manually,
    but I recommend allowing the program to auto-detect the best address location.

    Will hopefully add controls for the FIR filters for the echo in the SPC700 once I figure out 
    how they work. Changing the values in their DSP address is the easy part. The hard part is 
    actually making their adjustment user-friendly. My experiments with changing their values
    only seemed to cause random and sometimes chaotic changes to the echo. Hopefully with more
    research into FIR filters, I'll be able to come up with some user-friendly controls.

    Tools for creating music for the Super Nintendo is a woefully needed addition to the demoscene
    and homebrewing video game scene and I hope that this will be a useful addition to SNES 
    development. Once I understand more of the ins and outs of the SPC700, I hope to create even
    more tools for SNES music creating. Until then, please enjoy this SPC echo adder!

    Big thanks to eKid for their XM2SNES program and to both eKid and Anomie for their research
    into the SPC700 and work on the SPC700 Reference Guide.

    05/06/2023 by Finley Baker ( _astriid_ )
*/

#include "global.h"

#ifdef  _DEBUG
#define _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC_NEW
#include <crtdbg.h>
#endif

void usage()
{

    std::cout << "\n"

        "SPC Echo Adder v1.3 by _astriid_\n\n"
        "Adds or changes echo controls for .spc SNES music files. Designed \n"
        "primarily for use with XM2SNES converted .spc files to add echo to\n"
        "SNES music made with Fasttracker II .xm files and converted to .spc.\n"
        "NOTE: will work with most .spc files, however some .spc files use\n"
        "indirect addressing to control the SNES's echo and may not work\n"
        "with this echo adder.\n\n"
        "Recommended to try .spc file in a dedicated SPC700 emulator player \n"
        "such as SPC Play or Chipsynth SFC before attempting playback on a \n"
        "real Super Nintendo(tm)/Super Famicom(tm).\n\n"
        "Usage: spcecho inputname.spc [options] [outputname.spc]\n"
        "[NOTE: if outputname is not entered, inputname will be used]\n\n"
        "Options:\n\n"
        "-l | left echo volume\t | percentage of left echo volume between -100 and 100\n\n"
        "-r | right echo volume\t | percentage of right echo volume between -100 and 100\n\n"
        "-f | feedback\t\t | percentage of feedback between -100 and 100\n\n"
        "-t | echo timing in ms\t | 0-240ms (will be rounded to nearest 16ms increment)\n"
        "\t\t\t   [values: 16 | 32 | 48 | 64 | 80 | 96 | 112 | 128\n\t\t\t\t    144 | 160 | 176 | 192 | 208 | 224 | 240 ]\n"
        "\t\t\t   [NOTE: echo buffer uses 2kb for every 16ms. Music data \n\t\t\t   AND echo buffer must not exceed 64kb]\n\n"
        "-c | channel\t\t | turns echo on or off for channels 1-8. Use X to turn on\n\t\t\t   echo for channel and O to turn off echo for channel.\n"
        "\t\t\t   [eg: XOXOXOOO will turn echo on for 1, 3, and 5\n\t\t\t   but keep echo off for 2, 4, 6, 7, and 8]\n\n"
        "-a | echo buffer address | address in hex from 02-FF for echo buffer\n"
        "\t\t\t   [NOTE: not using this option will automatically set the\n"
        "\t\t\t   echo buffer to the address block immediately after the\n\t\t\t   SPC's audio data. Recommended to not use this option\n"
        "\t\t\t   unless you absolutely want to specify the buffer address]\n\n";

    exit(0);
}

int main(int argc, char* argv[])
{
    if(argc <= 1)
        usage();
    
    std::string writeName = argv[1];
    fileRead(writeName);

    bool customAddress = false;


    for (int i = 1; i < argc; ++i, ++argv)
    {
        /* outputs error if invalid argument is made */
        if (argv[0][0] == '-' && isalpha(argv[0][1]) &&
          !(argv[0][1] == 'l' || argv[0][1] == 'r' || argv[0][1] == 'f' ||
            argv[0][1] == 't' || argv[0][1] == 'c' || argv[0][1] == 'a'))
        {
            std::cerr << "\n!!!! Cannot read options! !!!!\n";
            usage();
        }
        
        if(*argv[0] == '-' && argv[1] != NULL)
        {
            switch((argv[0][1]))
            {

                /* left channel echo volume, from - 100 % to 100 % */
                case 'l':
                    valueSet(argv[0][1], percentToSign(argv[0][1], argv[1]));
                    break;

                /* right channel echo volume, from - 100 % to 100 % */
                case 'r':
                    
                    valueSet(argv[0][1], percentToSign(argv[0][1], argv[1]));
                    break;
                
                /* echo feedback level, from - 100 % to 100 % */
                case 'f':
                    
                    valueSet(argv[0][1], percentToSign(argv[0][1], argv[1]));
                    break;

                /* echo timing, from 0 - 240ms in 16ms increments, using 2kb of buffer per 16ms */
                case 't':

                    valueSet(argv[0][1], millisecondToInt(argv[1]));
                    break;

                /* echo channel enable for audo channels 1 through 8 */
                case 'c':

                    valueSet(argv[0][1], channelAddress(argv[1]));
                    break;

                /* echo buffer address from, value multiplied by 100h for actuall SPC RAM address used.
                    input range is 02h-FFh for buffer addres of $0200h-$FF00h, but recommended high address
                    as to not interfere with music data buffer. C0h seems to be a decent starting point. */
                case 'a':
                    customAddress = true;
                    valueSet(argv[0][1], bufferAddress(argv[1]));
                    break;
            }

        }
        
    }

    if (!customAddress)
        echoAddress();

    // if no output file name is declared, use input file name //
    if(argv[-1][0] == '-' && !isdigit(argv[-1][1]))
        fileWrite(writeName);

    // write output file name //
    if(argv[-1][0] != '-' || isdigit(argv[-1][1]))
    {
        writeName = argv[0];
        fileWrite(writeName);
    }
    _CrtDumpMemoryLeaks();

    return 0;

}