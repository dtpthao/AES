#include "aes.hpp"


uint8_t* AESBlockCipher(uint8_t *src, uint8_t *cipherKey, int keyByteSize) {

    int rcon, rounds;
    switch (keyByteSize) {
    case KEY_128:
        rcon = 10;
        rounds = 10;
        break;
    case KEY_192:
        rcon = 8;
        rounds = 12;
        break;
    case KEY_256:
        rcon = 7;
        rounds = 14;
        break;
    default:
        throw std::invalid_argument( "invalid key length" );
    }

    uint8_t *dst = (uint8_t *) malloc(BLOCK_SIZE);
    memcpy(dst, src, BLOCK_SIZE);
    uint8_t *rKeys = (uint8_t *) malloc(rounds * BLOCK_SIZE);
    GenRoundKey(cipherKey, keyByteSize, rcon, rKeys);

    int keyIdx = 0;
    AddRoundKey(dst, &rKeys[0]);
    for (int i = 1; i < rounds; i++) {
        keyIdx = i * BLOCK_SIZE;
        SubBytes(dst, SBOX);
        ShiftRow(dst);
        MixColumn(dst, MCOL_MATRIX);
        AddRoundKey(dst, &rKeys[keyIdx]);
    }
    SubBytes(dst, SBOX);
    ShiftRow(dst);
    AddRoundKey(dst, &rKeys[rounds * BLOCK_SIZE]);

    return dst;
}

uint8_t* AESBlockDecipher(uint8_t *src, uint8_t *cipherKey, int keyByteSize) {

    int rcon, rounds;
    switch (keyByteSize) {
    case KEY_128:
        rcon = 10;
        rounds = 10;
        break;
    case KEY_192:
        rcon = 8;
        rounds = 12;
        break;
    case KEY_256:
        rcon = 7;
        rounds = 14;
        break;
    default:
        throw std::invalid_argument( "invalid key length" );
    }

    uint8_t *dst = (uint8_t *) malloc(BLOCK_SIZE);
    memcpy(dst, src, BLOCK_SIZE);
    uint8_t *rKeys = (uint8_t *) malloc(rounds * BLOCK_SIZE);
    GenRoundKey(cipherKey, keyByteSize, rcon, rKeys);

    int keyIdx;
    AddRoundKey(dst, &rKeys[rounds * BLOCK_SIZE]);
    for (int i = rounds - 1; i > 0; i--) {
        keyIdx = i * BLOCK_SIZE;
        InvShiftRow(dst);
        SubBytes(dst, INV_SBOX);
        AddRoundKey(dst, &rKeys[keyIdx]);
        MixColumn(dst, INV_MCOL_MATRIX);
    }

    InvShiftRow(dst);
    SubBytes(dst, INV_SBOX);
    AddRoundKey(dst, &rKeys[0]);

    return dst;
}

//////////////////////////////////////////////////////////////

// @return length of ciphertext
int EncryptAESCBC(uint8_t *src, int plainSize, uint8_t *IV, uint8_t *cipherKey, int keyByteSize, uint8_t *dst) {

    int numOfBlocks = getNumOfBlocks(plainSize);

    uint8_t *state = (uint8_t *) malloc(BLOCK_SIZE);
    uint8_t *tmpIV = (uint8_t *) malloc(BLOCK_SIZE);
    memcpy(tmpIV, IV, BLOCK_SIZE);

    int cipherTextSize = ZeroPadding(src, plainSize);
    int idx = 0;
    for(int i = 0; i < numOfBlocks; i++) {
        idx = i * BLOCK_SIZE;
        state = XORBlock(&src[idx], tmpIV);
        tmpIV = AESBlockCipher(state, cipherKey, keyByteSize);
        memcpy(&dst[idx], tmpIV, BLOCK_SIZE);
    }

    return cipherTextSize;
}

int DecryptAESCBC(uint8_t *src, int cipherSize, uint8_t *IV, uint8_t *cipherKey, int keyByteSize, uint8_t *dst) {

    int numOfBlocks = getNumOfBlocks(cipherSize);
    
    uint8_t *state = (uint8_t *) malloc(BLOCK_SIZE);
    uint8_t *tmpIV = (uint8_t *) malloc(BLOCK_SIZE);
    memcpy(tmpIV, IV, BLOCK_SIZE);
    uint8_t *output = (uint8_t *) malloc(BLOCK_SIZE);

    int idx = 0;
    for(int i = 0; i < numOfBlocks; i++) {
        idx = i * BLOCK_SIZE;
        state = AESBlockDecipher(&src[idx], cipherKey, keyByteSize);
        output = XORBlock(state, tmpIV);
        memcpy(tmpIV, &src[idx], BLOCK_SIZE);
        memcpy(&dst[idx], output, BLOCK_SIZE);
    }

    return RevertZeroPadding(dst, cipherSize);
}

//////////////////////////////////////////////////////////////

int ZeroPadding(uint8_t *plaintxt, int plainSize) {

    int numOfBlocks = getNumOfBlocks(plainSize);
    int padLen = BLOCK_SIZE - (plainSize % BLOCK_SIZE);

    if (padLen < BLOCK_SIZE) {
        for (int i = 0; i < padLen - 1; i++) {
            plaintxt[plainSize + i] = 0;
        }
        plaintxt[plainSize + padLen - 1] = (uint8_t) padLen;
    }

    return numOfBlocks * BLOCK_SIZE;
}

// @return correct plaintext length
int RevertZeroPadding(uint8_t *decipherText, int decipherSize) {

    int padLen = decipherText[decipherSize - 1];
    if (padLen < BLOCK_SIZE) {
        int startIndex = decipherSize - padLen;
        for (int i = decipherSize - 2; i > startIndex; i--) {
            if (decipherText[i] != 0) return decipherSize;
        }
        return decipherSize - padLen;
    }
    return decipherSize;
}

//////////////////////////////////////////////////////////////

void TestAES() {
    uint8_t input[16] = {
        0x32, 0x43, 0xf6, 0xa8,
        0x88, 0x5a, 0x30, 0x8d,
        0x31, 0x31, 0x98, 0xa2,
        0xe0, 0x37, 0x07, 0x34
    };

    uint8_t cipherKey[16] = {
        0x2b, 0x7e, 0x15, 0x16,
        0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88,
        0x09, 0xcf, 0x4f, 0x3c
    };

    uint8_t *output;
    uint8_t *output2;

    printf("Plaintext:    ");
    for(int i = 0; i < 16; i++) {
        printf("%02x ", input[i]);
    }
    printf("\n");

    output = AESBlockCipher(input, cipherKey, 16);
    printf("Ciphertext:   ");
    for(int i = 0; i < 16; i++) {
        printf("%02x ", output[i]);
    }
    printf("\n");

    printf("Deciphertext: ");
    output2 = AESBlockDecipher(output, cipherKey, 16);
        for(int i = 0; i < 16; i++) {
        printf("%02x ", output2[i]);
    }
    printf("\n");

}

void TestCBC() {
    uint8_t plain[32] = {
        0x32, 0x43, 0xf6, 0xa8,
        0x88, 0x5a, 0x30, 0x8d,
        0x31, 0x31, 0x98, 0xa2,
        0xe0, 0x37, 0x07, 0x34,
        0x47, 0x37, 0x94, 0xed,
        0x40, 0xd4, 0xe4, 0xa5,
        0xa3, 0x70, 0x3a, 0xa6,
        // 0x4c, 0x9f, 0x42, 0xbc
    };

    uint8_t IV[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

    uint8_t cipherKey[16] = {
        0x2b, 0x7e, 0x15, 0x16,
        0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88,
        0x09, 0xcf, 0x4f, 0x3c
    };

    uint8_t *output = (uint8_t *) malloc(32);
    uint8_t *output2 = (uint8_t *) malloc(32);

    printf("Plaintext:    ");
    for(int i = 0; i < 28; i++) {
        printf("%02x ", plain[i]);
    }
    printf("\n");

    int cipherLen = EncryptAESCBC(plain, 28, IV, cipherKey, KEY_128, output);

    printf("Ciphertext:   ");
    for(int i = 0; i < cipherLen; i++) {
        printf("%02x ", output[i]);
    }
    printf("\n");

    int plainLen = DecryptAESCBC(output, cipherLen, IV, cipherKey, KEY_128, output2);
    printf("Deciphertext: ");
    for(int i = 0; i < plainLen; i++) {
        printf("%02x ", output2[i]);
    }
    printf("\n");
}