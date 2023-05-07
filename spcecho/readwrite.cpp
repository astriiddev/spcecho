#include "global.h"

char *fileBuffer;
long long fileLength;

/* SPC700 DSP registers begin at 10100h */
const unsigned int addressOffset = 0x10100;

void fileRead(const std::string& spcName)
{
    std::ifstream spcRead;
    if(spcRead)
    {   
        /* if no .spc extension is inputted, attempts to locate .spc file of input */
        spcRead.open(spcName.find(".spc") != std::string::npos ? spcName : 
            spcName + ".spc", std::ios::in | std::ios::binary);

        if (!spcRead)
        {
            std::cerr << "\n!!!! File not found! !!!!\n";
            exit(0);
        }
        spcRead.seekg(0, std::ios::end);
        fileLength = spcRead.tellg();

        /* copies input file into buffer */
        fileBuffer = new char[unsigned int(fileLength)];
        spcRead.seekg(0, std::ios::beg);
        spcRead.read(&fileBuffer[0], fileLength);

        spcRead.close();
    }

}

void valueSet(char v, int controlValue)
{
    switch(v)
    {
    case 'l':

        /* left channel echo volume address : 1012Ch */
        fileBuffer[addressOffset + 0x2C] = controlValue;
        break;

    case 'r':

        /* right channel echo volume address : 1013Ch */
        fileBuffer[addressOffset + 0x3C] = controlValue;
        break;

    case 'f':

        /* echo feedback level address : 1010Dh */
        fileBuffer[addressOffset + 0x0D] = controlValue;
        break;

    case 't':

        /* echo speed address : 1017Dh */
        fileBuffer[addressOffset + 0x7D] = controlValue;
        break;

    case 'c':

        /* channel echo enable/mute address : 1014Dh */
        fileBuffer[addressOffset + 0x4D] = controlValue;
        break;

    case 'a':

        /* echo buffer address : 1016Dh */
        fileBuffer[addressOffset + 0x6D] = controlValue;
        break;
    }
}

void fileWrite(const std::string& spcName)
{
    /* if no .spc extension is inputted, attempts to locate .spc file of input */
    bool hasExtension = (spcName.find(".spc") != std::string::npos);
    char overWrite;

    std::ofstream spcWrite;
    if (spcWrite)
    {
        /* checks if file exists before attempting to write */
        spcWrite.open(hasExtension ? spcName : spcName + ".spc", std::ios::in);

        if (spcWrite)
        {
            /* if file exists, choose to overwrite old file */
            hasExtension ? std::cout << '\n' << spcName << " already exists! Overwrite ? (y / n) " :
                std::cout << '\n' << spcName + ".spc" << " already exists! Overwrite ? (y / n) ";

            std::cin >> overWrite;
            spcWrite.close();

            if (toupper(overWrite) != 'Y')
            {
                /* if file is not overwritten, exits before file writing */
                hasExtension ? std::cerr << "\n!!!! " << spcName << " not saved! !!!!\n" :
                    std::cerr << "\n!!!! " << spcName + ".spc" << " not saved! !!!!\n";
                exit(0);

            }

        }

        /* informs user that echo values are saved */
        hasExtension ? std::cout << "\nWriting echo DSP registers for " << spcName << "....\n" << spcName << " saved!\n" :
            std::cout << "\nWriting echo DSP registers for " << spcName << ".spc....\n" << spcName + ".spc saved!\n";

        /* set bit at value 20h mutes echo. clears bit while leaving other bits as they were */
        fileBuffer[addressOffset + 0x6C] &= ~(0b01 << 5);

        spcWrite.open(spcName.find(".spc") != std::string::npos ? spcName :
            spcName + ".spc", std::ios::binary);

        if (!spcWrite)
        {
            std::cerr << "!!!! Cannot open file! !!!!" << "\n";
            exit(0);
        }

        spcWrite.write(fileBuffer, fileLength);

        spcWrite.close();

        if (!spcWrite.good())
        {
            std::cerr << "!!!! Error occurred at writing time! !!!!" << "\n";
            exit(0);
        }

    }

    delete[] fileBuffer;
    fileBuffer = nullptr;
}