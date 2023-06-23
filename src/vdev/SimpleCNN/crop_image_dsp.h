#pragma once
#include "dsp_core.h"

class CropImageDSP
{
public:
    int tickPerCycle = 1;

    int run();

    void* mem1;
    void* mem2;

    bool finished = false;
    unsigned char counter = 0;

    CropImageDSP()
    {
        dspcore.memhub.addMemory("original_image", 50 * 50);
		dspcore.memhub.addMemory("clipped_image", 28 * 28);

        mem1 = dspcore.memhub.getMemory("original_image").getPtr();
        mem2 = dspcore.memhub.getMemory("clipped_image").getPtr();
    }

private:
    DSPCore dspcore;
};