#ifndef _AES_H
#define _AES_H

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include "rkey.hpp"
#include "sbox.hpp"
#include "mcol.h"
#include "srow.hpp"
using namespace std;

#define BLOCK_SIZE 16
#define KEY_128 16
#define KEY_192 24
#define KEY_256 32

int ZeroPadding(uint8_t *plaintxt, int plainSz);
int RevertZeroPadding(uint8_t *decipherTxt, int decipherTxtSize);

uint8_t* AESBlockCipher(uint8_t *src, uint8_t *cipherKey, int keyByteSize);
uint8_t* AESBlockDecipher(uint8_t *src, uint8_t *cipherKey, int keyByteSize);

int EncryptAESCBC(uint8_t *src, int plainSize, uint8_t *IV, uint8_t *cipherKey, int keyByteSize, uint8_t *dst);
int DecryptAESCBC(uint8_t *src, int cipherSize, uint8_t *IV, uint8_t *cipherKey, int keyByteSize, uint8_t *dst);


#endif // !_AES_H