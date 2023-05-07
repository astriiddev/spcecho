#ifndef ADDRESSES_H
#define ADDRESSES_H

int percentToSign(char a, const std::string& percent);

int channelAddress(const std::string& channelVal);

int millisecondToInt(const std::string& speed);

int bufferAddress(const std::string& bufferHex);

#endif /*ADDRESSES_H*/