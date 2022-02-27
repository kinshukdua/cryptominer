#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void *_memset(void *s, int c,  unsigned int len);

void _memcpy(void *dest, void *src, size_t n);

void big2little (uint8_t * str_big_end);

int log2_ (uint32_t value);

struct darry
{
    int size;
    unsigned char * data;
};

struct merkletree
{
    int size;
    unsigned char ** hashes;
};

struct Extranonce {
    uint32_t extranonce1;
    struct darry extranonce2;
};


struct Job {
    int valid;
    struct Extranonce extranonce;
    char * job_id;
    unsigned char prev_hash[32];
    struct darry coinb1;
    struct darry coinb2;
    struct merkletree merkle_tree;
    uint32_t version;
    uint32_t nbits;
    uint32_t ntime;
    uint32_t difficulty;
    int clean;
};

struct Result {
    int success;
    char * error;
};

char * int2hexpad(uint32_t num);
char * int2hex(uint32_t num);
char * bytes2hex(unsigned char * bytes, int n);
int strlen_(char * str);
unsigned char str2byte(char a, char b);
struct darry str2bytes(char * str);
uint32_t str2uint32 (char * str);