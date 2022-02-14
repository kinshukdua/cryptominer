
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "sha2_opt.h"
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspsdk.h>
#include <pspctrl.h>
#define printf pspDebugScreenPrintf
#include <bitcoin_miner.h>
#include <math.h>

int mine_nonce_opt(unsigned char *block_header, unsigned char *target, uint32_t *nonce_out,uint32_t max) {
    uint32_t nonce = 0;
    while(1) {
        
        // Append nonce
        memcpy(&block_header[76], &nonce, 4);
        //print_header();
        
        //printf("Nonce: %5d\n", nonce);

        // Increase nonce
        nonce++;

        //printf("Hashing...\n");
        //pspDebugScreenClear();
        uint8_t hash_rev[32];
        uint8_t hash2[32];
        uint8_t hash[32];
        sha256(block_header, 80,hash2);
        sha256(hash2,32,hash_rev);
        for(int i=0; i < 32; i++) {
            hash[31-i] = hash_rev[i];
        }
        // printf("");


        
        // Check hash...
        for(int i=0; i < 32; i++) {
            /*
            printf("\nHASH: ");
            hexdump(hash,32);
            printf("\nTARGET: ");
            hexdump(target,32);
            */
            if(hash[i] < target[i]) {
                //pspDebugScreenSetTextColor(0x00FF00);
                //pspDebugScreenClear();
                //printf("Success!\n");
                *nonce_out = nonce - 1;
                return 0;
            } else if(hash[i] > target[i]) {
                break;
            }

        }
    
        //GET_Req("http://httpbin.org","/bytes/2");
        // Check if we need to receive a new block.
        /*DO GET TO CHECK IF WE GOT NEW BLOCK*/
        if(0) {
            printf("New block data.\n");
            return 1;
        }
        if (nonce>max)
        {
            //pspDebugScreenSetTextColor(0x00FF00);
            //pspDebugScreenClear();
            *nonce_out = nonce - 1;
            return 0;
        }
    }
}

void miner_opt(void) {
    unsigned char block_header[80];
    unsigned char target[32];

    while(1) {
        //print_header();
        struct SceCtrlData ctrlData;
        printf("Waiting for data...\n");
        receive_block_data_test(block_header, target);
        printf("Press X to start mining\nPress O to exit\n");
        while (1)
        {
            sceCtrlReadBufferPositive(&ctrlData,1);
            if(ctrlData.Buttons&PSP_CTRL_CROSS)
            {
                printf("Starting...");
                break;
            }
            if(ctrlData.Buttons&PSP_CTRL_CIRCLE)
            {
                sceKernelExitGame();
            }
            
        }
        for (int i = 1;i < 21;i++) {
            uint32_t max = pow(2,i) - 1;
            uint32_t nonce = 0;
            u64 tick0, tick1;
            u32 ticks_per_sec = sceRtcGetTickResolution();
            sceRtcGetCurrentTick(&tick0);
            int mine_result = mine_nonce_opt(block_header, target, &nonce, max);
            sceRtcGetCurrentTick(&tick1);
            double time_taken =  (double)(tick1 - tick0)/ticks_per_sec;
            printf("Time taken: %.4fs\n",time_taken);
            printf("Hash Rate: %.4f H/s\n", (double)nonce/time_taken);
            // Indicate success to host
            if(mine_result == 0) {
                char data[4];
                memcpy(data, &nonce, 4);
                //printf("Block Mined\n");
                //printf("NONCE: %d\n",nonce);
                //return;
                //POST_Req(/*SUCCESS and send Nonce*/);
                /*send nonce as post/get request data*/
            }
        }
        break;
    }
}
