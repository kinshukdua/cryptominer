#include <stdint.h>
void hexdump(unsigned char *f, size_t len);
int receive_block_data_test(unsigned char *block_header, unsigned char *target);
int mine_nonce(unsigned char *block_header, unsigned char *target, uint32_t *nonce_out,uint32_t max);
void miner(void);
