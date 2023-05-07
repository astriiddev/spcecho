#include "global.h"

int channelAddress(const std::string& channelVal)
{
    const uint8_t numofChannels = 8;

    uint8_t echoChannel;
    uint8_t *byteArray = new uint8_t[numofChannels];

    /* outputs error if not all 8 channels or if too many channels are inputted */
    if(channelVal.length() != numofChannels)
    {
        std::cerr << "\n!!!! Invalid number of channels! !!!!\n";
        usage();
    }

    /* creates array of bit values based on user input, acceptable inputs : o / O / 0 for OFF and x / X / 1 for ON */
    for(int i = 0; i < numofChannels; i++)
    {
        if( toupper(channelVal[i]) == 'X' || channelVal[i] == '1')
        {
            byteArray[i] = 0b01;
        } else if( toupper(channelVal[i]) == 'O' || channelVal[i] == '0' )
        {
            byteArray[i] = 0b00;
        } else 
        {
            /* outputs error if a character other than o/O/0 or x/X/1 is used*/
            byteArray[i] = 0;
            std::cerr << "\n!!!! Invalid channel input! !!!!\n";
            usage();
        }
    }

    /* channel enable is inputed as channel 1 through 8 for user readability, but actual byte is read by SPC700
        as channel 8 (MSB) through 1 (LSB) so a bitshift is needed to maintain user-friendliness */
    echoChannel = byteArray[7] << 7 | byteArray[6] << 6 | byteArray[5] << 5 | byteArray[4] << 4 | 
        byteArray[3] << 3 | byteArray[2] << 2 | byteArray[1] << 1 | byteArray[0];

    delete[] byteArray;
    byteArray = nullptr;

    return echoChannel;
}

int percentToSign(char a, const std::string& percent)
{
    std::string regAddress;
    int percentVal;
    const double toSign = 1.27;

    /* outputs corresponding percent value if invalid input is detected */
    switch (a)
    {
    case 'l':
        regAddress = "left channel volume";
        break;
    case 'r':
        regAddress = "right channel volume";
        break;
    case 'f':
        regAddress = "feedback amount";
        break;
    }

    /* if only digits are inputted, converts string to integer; if non-number is inputted 
        defaults integer to out of range value and outputs error */ 
    percent.find_first_not_of("-0123456789") == std::string::npos ? 
        percentVal = stol(percent) : percentVal = 127;
    
    /* outputs error if input is under -100% or over 100% */
    if(percentVal < -100 || percentVal > 100)
    {
        std::cerr << "\n!!!! Invalid value for " << regAddress << "! !!!!\n";
        usage();
    }

    /* converts percentage into signed integer */
    return int(round(percentVal * toSign));
}


int millisecondToInt(const std::string& echoSpeed)
{
    int timingVal;

    /* if only digits are inputted, converts string to integer; if non-number is inputted
        defaults integer to out of range value and outputs error */
    echoSpeed.find_first_not_of("0123456789") == std::string::npos ? 
        timingVal = stol(echoSpeed) : timingVal = 255;

    /* outputs error if input is under 0ms or over 240ms */
    if(timingVal < 0 || timingVal > 240)
    {
        std::cerr << "\n!!!! Invalid value for echo speed! !!!!\n";
        usage();
    }

    /* scales down time into 8-bit value, rounding to nearest bit */
    return int(round(float(timingVal) / 16));
}

int bufferAddress(const std::string& bufferHex)
{
    try
    {
        /* input 16-bit hex value for buffer address */
        int hexAddress = std::stoul(bufferHex, nullptr, 16);

        /* outputs error if input is outside of range 02h-FFh or if input is too long */
        if(bufferHex.length() > 2 || hexAddress > 0xFF || hexAddress < 2)
        {
            std::cerr << "\n!!!! Invalid buffer address! !!!!\n";
            usage();
        }

        return hexAddress;
    }
    catch(...)
    {
        /* outputs error if a non-hex value is inputted */
        std::cerr << "\n!!!! Invalid buffer address! !!!!\n";
        usage();
        return 0;
    }
}