#include <stdint.h>
#include <bitcoin_miner.h>
#include "sha2_opt.h"
int mine_nonce_opt(unsigned char *block_header, unsigned char *target, uint32_t *nonce_out,uint32_t max);
void miner_opt(void);
