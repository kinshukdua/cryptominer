#include "util.h"

void *_memset(void *s, int c,  unsigned int len)
{
    unsigned char* p=s;
    while(len--)
    {
        *p++ = (unsigned char)c;
    }
    return s;
}

void _memcpy(void *dest, void *src, size_t n)
{
   // Typecast src and dest addresses to (char *)
   char *csrc = (char *)src;
   char *cdest = (char *)dest;
  
   // Copy contents of src[] to dest[]
   for (int i=0; i<n; i++)
       cdest[i] = csrc[i];
}

// convert from big endian to little endian in place
void big2little (uint8_t * str_big_end)
{
    uint8_t temp;
    for (int i = 0; i < 32; i++)
    {
        temp = str_big_end[i];
        str_big_end[i] = str_big_end[31-i];
        str_big_end[31-i] = temp;
    }
}


// Lookup table for log2
const uint8_t tab32[32] = {
     0,  9,  1, 10, 13, 21,  2, 29,
    11, 14, 16, 18, 22, 25,  3, 30,
     8, 12, 20, 28, 15, 17, 24,  7,
    19, 27, 23,  6, 26,  5,  4, 31};

// fast log2 for uin32
int log2_ (uint32_t value)
{
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    return tab32[(uint32_t)(value*0x07C4ACDD) >> 27];
}

char * int2hexpad(uint32_t num)
{
	char * hex = malloc(sizeof(char)*16);
	sprintf(hex,"%08x",num);
	return hex;
}

char * int2hex(uint32_t num)
{
	char * hex = malloc(sizeof(char)*16);
	sprintf(hex,"%x",num);
	return hex;
}

//convert n bytes to one hex string
char * bytes2hex(unsigned char * bytes, int n)
{
    char * hex = malloc(sizeof(char)*(n*2+1));
    for(int i=0; i<n; i++)
    {
        sprintf(hex+i*2,"%02x",bytes[i]);
    }
    return hex;
}

int strlen_(char * str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        i++;
    }
    return i;
}

// takes in two charactcer (in big endian) and returns the hex value
unsigned char str2byte(char a, char b)
{
    unsigned char ret = 0;
    if (a >= '0' && a <= '9')
    {
        ret += (a - '0') * 16;
    }
    else if (a >= 'a' && a <= 'f')
    {
        ret += (a - 'a' + 10) * 16;
    }
    else if (a >= 'A' && a <= 'F')
    {
        ret += (a - 'A' + 10) * 16;
    }
    if (b >= '0' && b <= '9')
    {
        ret += (b - '0');
    }
    else if (b >= 'a' && b <= 'f')
    {
        ret += (b - 'a' + 10);
    }
    else if (b >= 'A' && b <= 'F')
    {
        ret += (b - 'A' + 10);
    }
    return ret;
}

struct darry str2bytes(char * str)
{
    int len = strlen_(str);
    if (len%2 == 1)
    {
        len++;
        char * x = (char *)calloc(len, sizeof(char));
        x[0] = '0';
        for (int i = 1;i<len;i++)
        {
            x[i] = str[i-1];
        }
        str = x;
    }
    unsigned char * data = (unsigned char *) calloc((len/2), sizeof(unsigned char));
    struct darry ret;
    for (int i = 0; i < len/2; i++)
    {
        data[i] = str2byte(str[2*i], str[2*i+1]);
    }
    ret.size = len/2;
    ret.data = data;
    return ret;
}



// convert a 32 bit hex string to a single uin32_t
uint32_t str2uint32 (char * str)
{
    uint32_t ret = 0;
    ret = (uint32_t) strtoll(str, NULL, 16);
    return ret;
    // uint32_t ret = 0;
    // uint16_t pow = 1;
    // for (int i = 3; i >= 0; i--)
    // {
    //     unsigned char byte = str2byte(str[i*2], str[i*2+1]);
    //     ret += byte * pow;
    //     pow = pow * 16;
    // }
    // return ret;
}
