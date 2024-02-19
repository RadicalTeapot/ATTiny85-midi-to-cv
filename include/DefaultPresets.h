#ifndef DefaultPresets_h
#define DefaultPresets_h

#include "Preset.h"

const Preset preset0 = {
    {
        0x01, // Note channels (1 and 2)
        0x01, // CC number 1 (CC1)
        0x02, // CC number 2 (CC2)
        0x03, // CC number 3 (CC3)
        0x04, // CC number 4 (CC4)
        0x00, // CC channels 1 (0)
        0x00, // CC channels 2 (0)
    },
    {
        0x23, // Note channels (3 and 4)
        0x05, // CC number 1 (CC5)
        0x06, // CC number 2 (CC6)
        0x07, // CC number 3 (CC7)
        0x08, // CC number 4 (CC8)
        0x00, // CC channels 1 (0)
        0x00, // CC channels 2 (0)
    }
};

const Preset preset1 = {
    {
        0x01, // Note channels (1 and 2)
        0x01, // CC number 1 (CC1)
        0x07, // CC number 2 (CC7)
        0x0B, // CC number 3 (CC11)
        0x4A, // CC number 4 (CC74)
        0x00, // CC channels 1 (0)
        0x00, // CC channels 2 (0)
    },
    {
        0x23, // Note channels (3 and 4)
        0x15, // CC number 1 (CC21)
        0x16, // CC number 2 (CC22)
        0x17, // CC number 3 (CC23)
        0x47, // CC number 4 (CC71)
        0x00, // CC channels 1 (0)
        0x00, // CC channels 2 (0)
    }
};

#endif // DefaultPresets_h
