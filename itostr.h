#ifndef ITOSTR_H_
#define ITOSTR_H_

char *utostr(char *buf, unsigned len, unsigned val, unsigned base);
char *itostr(char *buf, unsigned len, int val, unsigned base);

#endif