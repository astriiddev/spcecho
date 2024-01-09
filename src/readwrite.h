#ifndef READ_WRITE_H
#define READ_WRITE_H

void freeBuffer(void);
int echoAddress();
int fileRead(const char* spcName);
int fileWrite(const char* spcName);
void valueSet(char v, int controlValue);

#endif /*READ_WRITE_H*/
