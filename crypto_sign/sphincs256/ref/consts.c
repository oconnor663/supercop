#include <stdint.h>

const uint32_t hashc8x[64] = {
    0x61707865, 0x61707865, 0x61707865, 0x61707865, 0x61707865, 0x61707865, 0x61707865, 0x61707865,
    0x3320646e, 0x3320646e, 0x3320646e, 0x3320646e, 0x3320646e, 0x3320646e, 0x3320646e, 0x3320646e,
    0x79622d32, 0x79622d32, 0x79622d32, 0x79622d32, 0x79622d32, 0x79622d32, 0x79622d32, 0x79622d32,
    0x74206574, 0x74206574, 0x74206574, 0x74206574, 0x74206574, 0x74206574, 0x74206574, 0x74206574,
    0x3436206f, 0x3436206f, 0x3436206f, 0x3436206f, 0x3436206f, 0x3436206f, 0x3436206f, 0x3436206f,
    0x7479622d, 0x7479622d, 0x7479622d, 0x7479622d, 0x7479622d, 0x7479622d, 0x7479622d, 0x7479622d,
    0x74732065, 0x74732065, 0x74732065, 0x74732065, 0x74732065, 0x74732065, 0x74732065, 0x74732065,
    0x21657461, 0x21657461, 0x21657461, 0x21657461, 0x21657461, 0x21657461, 0x21657461, 0x21657461};

const unsigned char _rotate8[32]  = {3,0,1,2,7,4,5,6,11,8,9,10,15,12,13,14,
                               3,0,1,2,7,4,5,6,11,8,9,10,15,12,13,14};

const unsigned char _rotate16[32] = {2,3,0,1,6,7,4,5,10,11,8,9,14,15,12,13,
                               2,3,0,1,6,7,4,5,10,11,8,9,14,15,12,13};
