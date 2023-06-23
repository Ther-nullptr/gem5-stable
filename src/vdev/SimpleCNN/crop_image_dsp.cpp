#include "crop_image_dsp.h"

int CropImageDSP::run()
{
    int cyclesUsed = 0;
    finished = false;

    if (counter == 0)
    {
        cyclesUsed = dspcore.op_init();
    }
    else if (counter == 1)
    {
        cyclesUsed = dspcore.op_input();
    }
    else if (counter == 2)
	{
		cyclesUsed = dspcore.op_calculate("original_image", "clipped_image", Range(0, 50), Range(0, 28));
	}
    else if (counter == 3)
    {
        cyclesUsed = dspcore.op_output();
        finished = true;
    }

    return cyclesUsed * tickPerCycle;
}