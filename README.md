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
    and will also cause your SPC player to glitch out. I recommend starting with a value of C0h 
    to put the echo buffer at address $C000h with a short echo and the playing around from there. 
    If your SPC emulator has an ARAM viewer (such as Plogue's Chipsynth SFC) and you're using an
    XM2SNES converted .spc file or an .spc file without echo, you can put the file in the ARAM
    viewer before running it through this program in order to get a good sense of where your music
    data buffer ends.

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
    
# Usage
![image](https://user-images.githubusercontent.com/98296288/236651901-a60aae62-8402-4719-aa9e-026fa2d16f6e.png)

# Test Files
Three test files (located in the folder testfiles) are included:

rroll.xm is the Fasttracker II module before being converted with XM2SNES

rroll.spc is the converted .spc music file, converted with XM2SNES

rrollecho.spc is the same .spc file, but with echo added using spcecho

The spcecho commands used to add the echo controls to rroll.spc were:

![spcecho controls](https://user-images.githubusercontent.com/98296288/236652468-f4251050-9e00-4120-9be0-6c524a4b19e1.png)

All samples used were sourced or created by me, without ripping them from any games

# References
1. SPC700 Reference https://wiki.superfamicom.org/spc700-reference#dsp-983
2. SPC and RSN File Format https://wiki.superfamicom.org/spc-and-rsn-file-format
3. XM2SNES (by eKid, mirror by osoumen) https://github.com/osoumen/XMSNES
4. SPC Play https://github.com/dgrfactory/spcplay
5. Plogue Chipsynth SFC https://www.plogue.com/products/chipsynth-sfc.html

