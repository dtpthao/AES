#ifndef _ROUND_KEY_H
#define _ROUND_KEY_H

const uint8_t RCON[10] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };

void AddRoundKey(uint8_t *state, uint8_t *rKey);

void RotWordAndSubBytes(uint8_t *rKey1, uint8_t *rKey2);

void RconAndXOR(uint8_t *rKey1, uint8_t *rKey2, uint8_t rcon);

void GenRoundKey(uint8_t *cipherKey, int keyLen, int rcon, uint8_t *rKeys);

#endif
