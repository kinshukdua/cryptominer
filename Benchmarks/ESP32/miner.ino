#include "memzero.h"
extern "C"{
#include "sha2.h"
#include "sha2_opt.h"
};




int receive_block_data_test(unsigned char *block_header, unsigned char *target) {
    memset(block_header, 0x11, 76);
    memset(target,0x00,3);
    memset(target+3,0x00,4);
    memset(target+7,0x00,25);
    return 0;
}   

int mine_nonce(unsigned char *block_header, unsigned char *target, uint32_t *nonce_out, uint32_t max_nonce) {
    uint32_t nonce = 0;
    
    while(1) {
        
        // Append nonce
        memcpy(&block_header[76], &nonce, 4);
        //print_header();
//        Serial.print("Nonce: ");
//        Serial.println(nonce);

        // Increase nonce
        nonce++;
        if (nonce > max_nonce)
        {
            *nonce_out = nonce - 1;
            return 0;
        }
//        Serial.print("Hashing...\n");
        uint8_t hash_rev[32];
        uint8_t hash2[32];
        uint8_t hash[32];
        sha256(block_header, 80,hash2);
        sha256(hash2,32,hash_rev);
        //calc_sha_256(hash2, block_header, 80);
        //calc_sha_256(hash_rev, hash2, 32);
        for(int i=0; i < 32; i++) {
            hash[31-i] = hash_rev[i];
        }
        // Serial.print("");


        // Check hash...
        for(int i=0; i < 32; i++) {
            // Serial.print("\nHASH: ");
            //hexdump(hash,32);
            // Serial.print("\nTARGET: ");
            // hexdump(target,32);
            if(hash[i] < target[i] ) {
                Serial.print("Success!\n");
                *nonce_out = nonce - 1;
                return 0;
            } else if(hash[i] > target[i]) {
                break;
            }
        }
    
        
        // Check if we need to receive a new block.
        /*DO GET TO CHECK IF WE GOT NEW BLOCK*/
        if(0) {
            Serial.print("New block data.\n");
            return 1;
        }
    }
}

void setup() {
  // put your setup code here, to run once:
   Serial.begin(9600);
}

void loop() {
// put your main code here, to run repeatedly:
//print_header();
for (int i = 1;i < 32;i++) {
    uint32_t nonce_max = pow(2,i) - 1;
    unsigned char block_header[80];
    unsigned char target[32];
    //Serial.print("Waiting for data...\n");
    receive_block_data_test(block_header, target);
    uint32_t nonce = 0;
    int64_t t;
    t = esp_timer_get_time();
    int mine_result = mine_nonce(block_header, target, &nonce, nonce_max);
    t = esp_timer_get_time() - t;
    double time_taken = ((double)t)/1000000; // in seconds
    Serial.print(i);
    Serial.print("\t");
    Serial.print(time_taken,4);
    Serial.print("\t");
    double hashrate = (((double)nonce)/time_taken);
    Serial.println(hashrate,4);
    // Indicate success to host
    if(mine_result == 0) {
        char data[4];
        memcpy(data, &nonce, 4);
//        Serial.print("Block Mined\n");
//        Serial.print("NONCE: ");
//        Serial.println(nonce,5);
        //printf("WINNING NONCE: %d\n",nonce);
        //printf("YAYYYY!!!!");
        //return;
        //POST_Req(/*SUCCESS and send Nonce*/);
        /*send nonce as post/get request data*/
    }
}
}
