# spcecho

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

05/06/2023 by Finley Baker ( \_astriid\_ )

    
# Usage
    Usage: spcecho inputname.spc [options] [outputname.spc]
    [NOTE: if outputname is not entered, inputname will be used]
    
    Options:

    -l | left echo volume    | percentage of left echo volume between -100 and 100
    
    -r | right echo volume   | percentage of right echo volume between -100 and 100
    
    -f | feedback            | percentage of feedback between -100 and 100
    
    -t | echo timing in ms   | 0-240ms (will be rounded to nearest 16ms increment)
                               [values: 16 | 32 | 48 | 64 | 80 | 96 | 112 | 128
                                        144 | 160 | 176 | 192 | 208 | 224 | 240 ]
                               [NOTE: echo buffer uses 2kb for every 16ms. Music data
                               AND echo buffer must not exceed 64kb]
    
    -c | channel             | turns echo on or off for channels 1-8. Use X to turn on
                               echo for channel and O to turn off echo for channel.
                               [eg: XOXOXOOO will turn echo on for 1, 3, and 5
                               but keep echo off for 2, 4, 6, 7, and 8]
    
    -a | echo buffer address | address in hex from 02-FF for echo buffer
                               [NOTE: not using this option will automatically set the
                               echo buffer to the address block immediately after the
                               SPC's audio data. Recommended to not use this option
                               unless you absolutely want to specify the buffer address]
# Test Files
Three test files (located in the folder testfiles) are included:

rroll.xm is the Fasttracker II module before being converted with XM2SNES

rroll.spc is the converted .spc music file, converted with XM2SNES

rrollecho.spc is the same .spc file, but with echo added using spcecho

The spcecho commands used to add the echo controls to rroll.spc were:

    spcecho rroll.spc -l 38 -r 38 -f -50 -c ooxoxoxx -a d7 -t 80 rrollecho.spc
    
All samples used were sourced or created by me, without ripping them from any games

# Resources Used
1. SPC700 Reference https://wiki.superfamicom.org/spc700-reference#dsp-983
2. SPC and RSN File Format https://wiki.superfamicom.org/spc-and-rsn-file-format
3. XM2SNES (by eKid, mirror by osoumen) https://github.com/osoumen/XMSNES
4. SPC Play https://github.com/dgrfactory/spcplay
5. Plogue Chipsynth SFC https://www.plogue.com/products/chipsynth-sfc.html

