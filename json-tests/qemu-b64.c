#include "qemu-b64.h"

/*
** encodeblock
**
** encode 3 8-bit binary bytes as 4 '6-bit' characters
*/
static void encodeblock( unsigned char *in, unsigned char *out, int len )
{
    out[0] = (unsigned char) cb64[ (int)(in[0] >> 2) ];
    out[1] = (unsigned char) cb64[ (int)(((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4)) ];
    out[2] = (unsigned char) (len > 1 ? cb64[ (int)(((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6)) ] : '=');
    out[3] = (unsigned char) (len > 2 ? cb64[ (int)(in[2] & 0x3f) ] : '=');
}

/*
** encode
**
** base64 encode a stream adding padding and line breaks as per spec.
*/

int encode(unsigned char *ins, unsigned char *outs)
{
	unsigned char in[3];
	unsigned char out[4];
	int i = 0;	
	char *tmp = outs;
	
	*in = (unsigned char) 0;
	*out = (unsigned char) 0;
	while(*ins) {
			i = i % 3;
			in[i++] = (unsigned char) *ins++; in[i] = '\0';
			//putchar(in[i]);
		if(i == 3 || !*ins)
		{
			encodeblock(in, out, i);
		//	printf("\nin: %s;  out: %s\n", in, out);
			strncpy(tmp, out, strlen(out)); tmp += strlen(out);
		}
	}
}
