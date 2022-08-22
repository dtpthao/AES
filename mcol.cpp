#include "mcol.h"

unsigned char modPolynom(int w) {
	int div = log2(w >> M_POLY);
	int res = w ^ (POLYNOM << div);
	if (res < POW2m_POLY) return (unsigned char)res;
	return modPolynom(res);
}

inline int coeff_mul(unsigned char uc, unsigned char coeff) {
    switch (coeff) {
    case  1:
        return uc;
    case 2:
        return uc << 1;
    case 3:
        return (uc << 1) ^ uc;
    case 9:  // 1001
        return (uc << 3) ^ uc;
    case 11: // 1011
        return (uc << 3) ^ (uc << 1) ^ uc;
    case 13: // 1101
        return (uc << 3) ^ (uc << 2) ^ uc;
    case 14: // 1110
        return (uc << 3) ^ (uc << 2) ^ (uc << 1);
    default:
        return 0;
    }
}

void MixColumn(unsigned char *state, const unsigned char matrix[4][4]) {

    int tmp[5];
    for (int stateCol = 0; stateCol < 4; stateCol++) {
        int stateIdx = stateCol << 2;

        for (int matrixRow = 0; matrixRow < 4; matrixRow++) {
            tmp[matrixRow] = coeff_mul(state[stateIdx], matrix[matrixRow][0]);
            tmp[matrixRow] ^= coeff_mul(state[stateIdx + 1], matrix[matrixRow][1]);
            tmp[matrixRow] ^= coeff_mul(state[stateIdx + 2], matrix[matrixRow][2]);
            tmp[matrixRow] ^= coeff_mul(state[stateIdx + 3], matrix[matrixRow][3]);
        }
        state[stateIdx] = modPolynom(tmp[0]);
        state[stateIdx + 1] = modPolynom(tmp[1]);
        state[stateIdx + 2] = modPolynom(tmp[2]);
        state[stateIdx + 3] = modPolynom(tmp[3]);
    }
}


void TestMixColumn() {
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
    MixColumn(state, MCOL_MATRIX);
    for(int i = 0; i < 16; i++) {
        printf("%02x ", state[i]);
    }
    printf("\n");
    
    MixColumn(state, INV_MCOL_MATRIX);
    for(int i = 0; i < 16; i++) {
        printf("%02x ", state[i]);
    }
}