#include <stdio.h>
#include <stdint.h>
#include "sha2_opt.h"
#include "stratum.h" 

void get_target(unsigned char *target, uint32_t diff)
{
    int shift = log2_(diff);
    for (int i = 0; i < 4; i++)
    {
        target[i] = 0x00;
    }
    // max difficulty = 2**64
    // else the algorithm breaks
    for (int i = 11; i >= 4; i--)
    {
        target[i] = (((0xffff000000000000 >> shift) >> (8 * (11-i))) & 0XFF);
    }
    for (int i = 12; i < 32; i++)
    {
        target[i] = 0x00;
    }
    // for (int i = 0;i<32;i++)
    // {
    //     printf("%02x", target[i]);
    // }
}

void build_merkle_root(struct Job job, uint8_t root[32])
{
    // build coinbase_hash_bin 
    uint8_t coinbase_hash_bin2[32];
    uint8_t coinbase_hash_bin[32];
    // concatonate coinb1 + extranonce1 + extranonce2 + coinb2
    uint8_t coinbase_bin[job.coinb1.size + 4 + 4 + job.coinb2.size];
    _memcpy(coinbase_bin, job.coinb1.data, job.coinb1.size);
    _memcpy(coinbase_bin + job.coinb1.size, &job.extranonce.extranonce1, 4);
    _memcpy(coinbase_bin + job.coinb1.size + 4, &job.extranonce.extranonce2, 4);
    _memcpy(coinbase_bin + job.coinb1.size + 4 + 4, job.coinb2.data, job.coinb2.size);
    // build coinbase_hash_bin
    sha256(coinbase_bin, job.coinb1.size + 4 + 4 + job.coinb2.size, coinbase_hash_bin2);
    sha256(coinbase_hash_bin2, 32, coinbase_hash_bin);
    // calc_sha_256(coinbase_hash_bin2, coinbase_bin, job.coinb1.size + 4 + 4 + job.coinb2.size);
    // calc_sha_256(coinbase_hash_bin, coinbase_hash_bin2, 32);
    // find merkle_root
    uint8_t merkle_root2[64];
    uint8_t merkle_root[64];
    _memcpy(merkle_root, coinbase_hash_bin, 32);
    // loop over size of merkle_tree
    for (int i = 0; i < job.merkle_tree.size; i++)
    {
        // conctonate merkle_root and hashes[i]
        _memcpy(merkle_root+32, job.merkle_tree.hashes[i], 32);
        // calculate double sha256
        sha256(merkle_root2, 64, merkle_root);
        sha256(merkle_root, 32, merkle_root2);
        // calc_sha_256(merkle_root2, merkle_root, 64);
        // calc_sha_256(merkle_root, merkle_root2, 32);
    }
    

}



int receive_block_header(struct Job job, unsigned char *header) {
    uint8_t merkle_root[32];
    build_merkle_root(job,merkle_root);
    big2little(merkle_root);
    // concatenate version, prevhash, merkle_root, ntime, nbits to header
    _memcpy(header, &job.version, 4);
    _memcpy(header+4, job.prev_hash, 32);
    _memcpy(header+36, merkle_root, 32);
    _memcpy(header+68, &job.ntime, 4);
    _memcpy(header+72, &job.nbits, 4);


    return 0;
}


int receive_block_data_test(unsigned char *block_header, unsigned char *target) {
    _memset(block_header, 0x11, 76);
    _memset(target,0x00,2);
    _memset(target+2,0xFF,4);
    _memset(target+6,0x00,26);
    return 0;
}  

int mine_nonce(unsigned char *block_header, unsigned char *target, uint32_t *nonce_out) {
    uint32_t nonce = 0;
    while(1) {
        // Append nonce
        _memcpy(&block_header[76], &nonce, 4);
        // Increase nonce
        nonce++;
        uint8_t hash_rev[32];
        uint8_t hash2[32];
        uint8_t hash[32];
        sha256(block_header, 80, hash2);
        sha256(hash2, 32, hash_rev);
        // calc_sha_256(hash2, block_header, 80);
        // calc_sha_256(hash_rev, hash2, 32);
        for(int i=0; i < 32; i++) {
            hash[31-i] = hash_rev[i];
        }
        for(int i=0; i < 32; i++) {
            if(hash[i] < target[i]) {
                *nonce_out = nonce - 1;
                return 0;
            } else if(hash[i] > target[i]) {
                break;
            }
        }
    
        
        // Check if we need to receive a new block.
        /*DO GET TO CHECK IF WE GOT NEW BLOCK*/
        if(0) {
            return 1;
        }
    }
}

uint32_t miner(struct Job job) {
    unsigned char target[32];
    unsigned char block_header[80];
    while(1) {
        //get_target(target, job.difficulty);
        //receive_block_header(job, block_header);
        receive_block_data_test(block_header, target);
        uint32_t nonce = 0;
        clock_t t;
        int mine_result = mine_nonce(block_header, target, &nonce);
        if(mine_result == 0) {
            printf("Block Mined\n");
            printf("NONCE: %d\n",nonce);
            return nonce;
        }
        else {
            return -1;
        }
    }
}

void main(void)
{
    connect_to_server();
    struct Job job;
    job = subscribe();
    uint32_t winning_nonce;
    authorize("miner1","password");
    printf("Mining...\n");
    winning_nonce = miner(job);
    submit("miner1", job.job_id, job.extranonce.extranonce2 , job.ntime, winning_nonce);

}
