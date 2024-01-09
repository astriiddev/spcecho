#ifndef ADDRESSES_H
#define ADDRESSES_H

int percentToSign(const char *percent, int *val);

int channelAddress(const char *channelVal, int *val);

int millisecondToInt(const char *echoSpeed, int *val);

int bufferAddress(const char *bufferHex, int *val);

#endif /*ADDRESSES_H*/
