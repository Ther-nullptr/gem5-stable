#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/mman.h>
#include <stdint.h>
#include <assert.h>
#include "../../../util/m5/m5op.h"

#include "peripheral.h"
#include "output_label.h"

#define BATCH 100

#define SIZE_CONV2D_KERNEL (5 * 5 * 1 * 32)
#define SIZE_CONV2D_BIAS 32

#define SIZE_CONV2D1_KERNEL (5 * 5 * 32 * 64)
#define SIZE_CONV2D1_BIAS 64

#define SIZE_DENSE_KERNEL (3136 * 512)
#define SIZE_DENSE_BIAS 512

#define SIZE_DENSE1_KERNEL (512 * 10)
#define SIZE_DENSE1_BIAS 10


int main()
{
	/*****************************/
	// m5_dumpreset_stats(0, 0);
	/*****************************/

	printf("Program Start.\n");

	volatile uint8_t *data = (uint8_t *)mmap(PERI_ADDR[0], 10 * 1024 * 1024, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	printf("Peripherals Registered.\n");

	periInit(data, 0, 1); // initialize the aichip
	printf("aichip Inited.\n");

    volatile uint8_t *data_dsp = (uint8_t *)mmap(PERI_ADDR[11], 1024 * 1024, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	printf("Peripherals Registered.\n");

	periInit(data_dsp, 0, 1); // initialize the aichip
	printf("dsp Inited.\n");

	FILE *fmodel = NULL;
	fmodel = fopen("data/ModelData", "rb");
	if (NULL == fmodel)
	{
		printf("Error Read: data/ModelData.\n");
		return -1;
	}

	size_t vdev_offset = 2 + 1024 * 1024 * 2;

	float *conv2d_kernel = (float *)malloc(SIZE_CONV2D_KERNEL * sizeof(float));
	fread(conv2d_kernel, sizeof(float), SIZE_CONV2D_KERNEL, fmodel);
	periWrite(data, vdev_offset, conv2d_kernel, SIZE_CONV2D_KERNEL * sizeof(float));
	free(conv2d_kernel);
	vdev_offset += SIZE_CONV2D_KERNEL * sizeof(float);
	printf("conv2d_kernel Writen to Vdev.\n");

	float *conv2d_bias = (float *)malloc(SIZE_CONV2D_BIAS * sizeof(float));
	fread(conv2d_bias, sizeof(float), SIZE_CONV2D_BIAS, fmodel);
	periWrite(data, vdev_offset, conv2d_bias, SIZE_CONV2D_BIAS * sizeof(float));
	free(conv2d_bias);
	vdev_offset += SIZE_CONV2D_BIAS * sizeof(float);
	printf("conv2d_bias Writen to Vdev.\n");

	float *conv2d1_kernel = (float *)malloc(SIZE_CONV2D1_KERNEL * sizeof(float));
	fread(conv2d1_kernel, sizeof(float), SIZE_CONV2D1_KERNEL, fmodel);
	periWrite(data, vdev_offset, conv2d1_kernel, SIZE_CONV2D1_KERNEL * sizeof(float));
	free(conv2d1_kernel);
	vdev_offset += SIZE_CONV2D1_KERNEL * sizeof(float);
	printf("conv2d1_kernel Writen to Vdev.\n");

	float *conv2d1_bias = (float *)malloc(SIZE_CONV2D1_BIAS * sizeof(float));
	fread(conv2d1_bias, sizeof(float), SIZE_CONV2D1_BIAS, fmodel);
	periWrite(data, vdev_offset, conv2d1_bias, SIZE_CONV2D1_BIAS * sizeof(float));
	free(conv2d1_bias);
	vdev_offset += SIZE_CONV2D1_BIAS * sizeof(float);
	printf("conv2d1_bias Writen to Vdev.\n");

	float *dense_kernel = (float *)malloc(SIZE_DENSE_KERNEL * sizeof(float));
	fread(dense_kernel, sizeof(float), SIZE_DENSE_KERNEL, fmodel);
	periWrite(data, vdev_offset, dense_kernel, SIZE_DENSE_KERNEL * sizeof(float));
	free(dense_kernel);
	vdev_offset += SIZE_DENSE_KERNEL * sizeof(float);
	printf("dense_kernel Writen to Vdev.\n");

	float *dense_bias = (float *)malloc(SIZE_DENSE_BIAS * sizeof(float));
	fread(dense_bias, sizeof(float), SIZE_DENSE_BIAS, fmodel);
	periWrite(data, vdev_offset, dense_bias, SIZE_DENSE_BIAS * sizeof(float));
	free(dense_bias);
	vdev_offset += SIZE_DENSE_BIAS * sizeof(float);
	printf("dense_bias Writen to Vdev.\n");

	float *dense1_kernel = (float *)malloc(SIZE_DENSE1_KERNEL * sizeof(float));
	fread(dense1_kernel, sizeof(float), SIZE_DENSE1_KERNEL, fmodel);
	periWrite(data, vdev_offset, dense1_kernel, SIZE_DENSE1_KERNEL * sizeof(float));
	free(dense1_kernel);
	vdev_offset += SIZE_DENSE1_KERNEL * sizeof(float);
	printf("dense1_kernel Writen to Vdev.\n");

	float *dense1_bias = (float *)malloc(SIZE_DENSE1_BIAS * sizeof(float));
	fread(dense1_bias, sizeof(float), SIZE_DENSE1_BIAS, fmodel);
	periWrite(data, vdev_offset, dense1_bias, SIZE_DENSE1_BIAS * sizeof(float));
	free(dense1_bias);
	vdev_offset += SIZE_DENSE1_BIAS * sizeof(float);
	printf("dense1_bias Writen to Vdev.\n");
	
	fclose(fmodel);

	FILE *fdata = NULL;
	fdata = fopen("data/mnist.dat", "rb");
	if (NULL == fdata)
	{
		printf("Error Read: data/mnist.dat.\n");
		return -1;
	}

	/*****************************/
	// m5_dumpreset_stats(0, 0);
	/*****************************/

	uint8_t input[50][50];
	uint8_t img[28][28];
	uint8_t finalresult[BATCH];
	for (int count = 0; count < BATCH; count++)
	{
		fread(input, sizeof(input), 1, fdata); //read input img
		/************* DSP Operation Begin ***************/
		// crop input 50x50 img into 28x28
        m5_reset_stats(0, 0);
        periWrite(data_dsp, 2, input, sizeof(input));
        for (int j = 1; j <= 3; j++)
        {
            periInit(data_dsp, j, 1);
        }
        assert(periIsFinished(data_dsp));
        periRead(data_dsp, 2 + 1024 * 512, img, sizeof(img));
        m5_dump_stats(0, 0);
		// crop_img(input, img);
		/************* DSP Operation End *****************/

		periWrite(data, 2 + 1024 * 1024, img, sizeof(img));
		for (int j = 1; j <= 10; j++)
		{
			periInit(data, j, 1);
		}
		assert(periIsFinished(data));
		periRead(data, 2, finalresult + count, sizeof(unsigned char));
		printf("Got result: %d\n", finalresult[count]);
	}
	fclose(fdata);
	periLogout(0);
    periLogout(11);

	/*****************************/
	// m5_dumpreset_stats(0, 0);
	/*****************************/

	int correct = 0;
	for (int i = 0; i < BATCH; i++)
	{
		if (finalresult[i] == target_list[i])
		{
			correct++;
		}
	}
	printf("accu: %f\n", (double)correct / (double)BATCH);

	return 0;
}
