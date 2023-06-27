#include <cmath>
#include <memory>
#include <cassert>
#include <cstring>
#include <algorithm>

#include "dsp_core.h"

int DSPCore::op_init(void)
{
    float* mem1 = memhub.getMemory("original_image").getPtr();
    float* mem2 = memhub.getMemory("clipped_image").getPtr();

    memhub.addSharedMemory("original_image", mem1, 50, 50);
    mem1 += 50 * 50;
    memhub.addSharedMemory("clipped_image", mem2, 28, 28);
    mem2 += 28 * 28;

    return 1;
}

int DSPCore::op_input()
{
    return 1;
}

int DSPCore::op_output()
{
    return 1;
}

int DSPCore::op_calculate(std::string inMemName, std::string outMemName, Range inRange, Range outRange)
{
    for (int i = 0; i < 5; i++)
    {
        uint8_t* in = (uint8_t*)memhub.getMemory(inMemName).getPtr();
        uint8_t* out = (uint8_t*)memhub.getMemory(outMemName).getPtr();

        // first, find the center of the input image
        double weighted_x_sum = 0;
        double weighted_y_sum = 0;
        double weight_sum = 0;
        for (int x = inRange.begin; x < inRange.end; x++)
        {
            for (int y = inRange.begin; y < inRange.end; y++)
            {
                weighted_x_sum += x * in[x * inRange.end + y];
                weighted_y_sum += y * in[x * inRange.end + y];
                weight_sum += in[x * inRange.end + y];
            }
        }
        double x_center = weighted_x_sum / weight_sum;
        double y_center = weighted_y_sum / weight_sum;

        // calculate the offset to move the center to (13.5, 13.5)
        int x_offset = lround(x_center - 13.5);
        int y_offset = lround(y_center - 13.5);

        // crop the image
        for (int x = outRange.begin; x < outRange.end; x++)
        {
            for (int y = outRange.begin; y < outRange.end; y++)
            {
                out[x * outRange.end + y] = in[(x + x_offset) * inRange.end + y + y_offset];
            }
        }
    }

    return 1;
}