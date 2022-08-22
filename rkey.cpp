#include "rkey.hpp"

void AddRoundKey(uint8_t *state, uint8_t *rKey) {
    
    for (int i = 0; i < BLOCK_SIZE; i++) {
        state[i] = state[i] ^ rKey[i];
    }
}

void RotWordAndSubBytes(uint8_t *rKey1, uint8_t *rKey2, int keySize) {
    
    rKey2[0] = SBOX[rKey1[keySize - 3]];
    rKey2[1] = SBOX[rKey1[keySize - 2]];
    rKey2[2] = SBOX[rKey1[keySize - 1]];
    rKey2[3] = SBOX[rKey1[keySize - 4]];
}

void RconAndXOR(uint8_t *rKey1, uint8_t *rKey2, uint8_t rcon, int keySize) {

    rKey2[0] ^= rKey1[0] ^ rcon;
    rKey2[1] ^= rKey1[1];
    rKey2[2] ^= rKey1[2];
    rKey2[3] ^= rKey1[3];
    
    for (int i = 4; i < keySize; i++) {
        rKey2[i] = rKey2[i - 4] ^ rKey1[i];
    }
}

void GenRoundKey(uint8_t *cipherKey, int keySize, int rcon, uint8_t *rKeys) {
    
    for (int i = 0; i < keySize; i++) {
        rKeys[i] = cipherKey[i];
    }

    int idx;
    for (int i = 0; i < rcon; i++) {
        idx = i * keySize;
        RotWordAndSubBytes(&rKeys[idx], &rKeys[idx + keySize], keySize);
        RconAndXOR(&rKeys[idx], &rKeys[idx + keySize], RCON[i], keySize);
    }
}
