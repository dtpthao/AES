#include "sbox.hpp"

void SubBytes(unsigned char *state, const unsigned char sbox[256]) {
    
    for(int i = 0; i < 16; i++) {
        state[i] = sbox[state[i]];
    }
}

void TestSubBytes() {

    unsigned char state[16] = {
        0xd4, 0xbf, 0x5d, 0x30,
        0xe0, 0xb4, 0x52, 0xae,
        0xb8, 0x41, 0x11, 0xf1,
        0x1e, 0x27, 0x98, 0xe5
    };

    for(int i = 0; i < 16; i++) {
        printf("%02x ", state[i]);
    }
    printf("\n");

    SubBytes(state, SBOX);
    for(int i = 0; i < 16; i++) {
        printf("%02x ", state[i]);
    }
    printf("\n");

    SubBytes(state, INV_SBOX);
    for(int i = 0; i < 16; i++) {
        printf("%02x ", state[i]);
    }
    printf("\n");

}