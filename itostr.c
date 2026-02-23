/* Like utoa and itoa but safe frobverflow */
#include "itostr.h"

char *utostr(char *buf, unsigned len, unsigned val, unsigned base)
{
	unsigned i = 0;
	unsigned v = val;
	do
	{
		v /= base;
		i++;
	} while(i < len - 1 && v);
	buf[i] = '\0';

	do
	{
		char c = (char)(val % base);
		val /= base;
		if(c >= 10) c += 'A' - 10;
		else c += '0';

		buf[--i] = c;
	}while(i > 0 && val);
	return buf;
}

char *itostr(char *buf, unsigned len, int val, unsigned base)
{
	char *start = buf;
	if(val < 0)
	{
		*buf++ = '-';
		val = -val;
	}
	utostr(buf, len - 1, (unsigned)val, base);
	return start;
}
