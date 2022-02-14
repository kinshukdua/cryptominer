
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "sha2.h"
#include <time.h>
#include <math.h>
void sb(unsigned char c) {
}

int receive_bytes(unsigned char *header, size_t len) {
    /*receive data*/
}



int receive_block_header(unsigned char *header) {
    /* Do GET Request for block header 76 long*/
    
    // printf("Done.");
    // hex_print(header, 10);
    return 0;
}



void hexdump(unsigned char *f, size_t len) {
    for(int i=0; i < len; i++) {
        unsigned char c = f[i];
        unsigned char c1 = c & 0x0F;
        unsigned char c2 = (c >> 4) & 0x0F;
        if(c2 < 10) {
            printf("%c", '0' + c2);
        } else {
            printf("%c", 'A' + (c2 - 10));
        }
        if(c1 < 10) {
            printf("%c", '0' + c1);
        } else {
            printf("%c", 'A' + (c1 - 10));
        }

    }
    puts("");
}
int receive_block_data_test(unsigned char *block_header, unsigned char *target) {
    memset(block_header, 0x11, 76);
    /*
    43829
    memset(target,0x00,2);
    memset(target+2,0xFF,4);
    memset(target+6,0x00,26);
    */
    
    memset(target,0x00,3);
    memset(target+3,0x00,4);
    memset(target+7,0x00,25);
    
    return 0;
}   

int mine_nonce(unsigned char *block_header, unsigned char *target, uint32_t *nonce_out,uint32_t max) {
    uint32_t nonce = 0;
    while(1) {
        
        // Append nonce
        memcpy(&block_header[76], &nonce, 4);
        //print_header();
        //printf("Nonce: %5d\n", nonce);

        // Increase nonce
        nonce++;
        if (nonce > max)
        {
            *nonce_out = nonce - 1;
            return 0;
        }
        //printf("Hashing...\n");
        uint8_t hash_rev[32];
        uint8_t hash2[32];
        uint8_t hash[32];
        calc_sha_256(hash2, block_header, 80);
        calc_sha_256(hash_rev, hash2, 32);
        for(int i=0; i < 32; i++) {
            hash[31-i] = hash_rev[i];
        }
        // printf("");


        // Check hash...
        for(int i=0; i < 32; i++) {
            //printf("\nHASH: ");
            //hexdump(hash,32);
            //printf("\nTARGET: ");
            //hexdump(target,32);
            if(hash[i] < target[i]) {
                //printf("Success!\n");
                *nonce_out = nonce - 1;
                return 0;
            } else if(hash[i] > target[i]) {
                break;
            }
        }
    
        
        // Check if we need to receive a new block.
        /*DO GET TO CHECK IF WE GOT NEW BLOCK*/
        if(0) {
            printf("New block data.\n");
            return 1;
        }
    }
}

void miner(void) {
    unsigned char block_header[80];
    unsigned char target[32];

    while(1) {
        //print_header();
        printf("Waiting for data...\n");
        FILE *fptr;
        fptr = fopen("results.txt", "a");
        receive_block_data_test(block_header, target);
        for (int i = 10;i < 32;i++) {
            uint32_t max = pow(2,i) - 1;
            uint32_t nonce = 0;
            clock_t t;
            t = clock();
            int mine_result = mine_nonce(block_header, target, &nonce,max);
            t = clock() - t;
            double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
            //printf("fun() took %f seconds to execute \n", time_taken);
            printf("Time taken: %.4fs\n",time_taken);
            printf("Hash Rate: %.4f H/s\n", (double)nonce/time_taken);
            fptr = fopen("results.txt", "a");
            fprintf(fptr, "%d\t%f\t%lf\n",i,time_taken,(double)nonce/time_taken);
            fclose(fptr);

            // Indicate success to host
            if(mine_result == 0) {
                char data[4];
                memcpy(data, &nonce, 4);
                printf("Block Mined\n");
                printf("NONCE: %d\n",nonce);
                //printf("WINNING NONCE: %d\n",nonce);
                //printf("YAYYYY!!!!");
                //return;
                //POST_Req(/*SUCCESS and send Nonce*/);
                /*send nonce as post/get request data*/
            }
        }
 
        return;
    }
}

int main(void)
{
    miner();
    return 0;
}
