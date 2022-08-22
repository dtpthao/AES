#include "srow.hpp"

void ShiftRow(unsigned char *state) {

    unsigned char tmp[4];
    int gap = 0, idx;
    for(int i = 1; i < 4; i++) {
        gap += 4;
        idx = i + gap;
        tmp[0] = state[idx];
        tmp[1] = state[(idx + 4) % 16];
        tmp[2] = state[(idx + 8) % 16];
        tmp[3] = state[(idx + 12) % 16];

        state[i] = tmp[0];
        state[i + 4] = tmp[1];
        state[i + 8] = tmp[2];
        state[i + 12] = tmp[3];
    }
}

void InvShiftRow(unsigned char *state) {

    unsigned char tmp[4];
    int gap = 16, idx;
    for(int i = 1; i < 4; i++) {
        gap -= 4;
        idx = i + gap;
        tmp[0] = state[idx];
        tmp[1] = state[(idx + 4) % 16];
        tmp[2] = state[(idx + 8) % 16];
        tmp[3] = state[(idx + 12) % 16];

        state[i] = tmp[0];
        state[i + 4] = tmp[1];
        state[i + 8] = tmp[2];
        state[i + 12] = tmp[3];
    }
}

void TestShiftRow() {
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

    ShiftRow(state);
    for(int i = 0; i < 16; i++) {
        printf("%02x ", state[i]);
    }
    printf("\n");

    InvShiftRow(state);
    for(int i = 0; i < 16; i++) {
        printf("%02x ", state[i]);
    }
    printf("\n");
}