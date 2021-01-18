// this script is used to convert pack mipi raw image to unpacked raw.
//cmd = ./a mipi_raw.raw 4032 3024 -> output will be mipi_unpacked_raw.raw
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char raw_array[4032 * 3024 * 2];	//4032*3024*2byte
short pixel_array[4032 * 3024 * 2]; //4032*3024*2byte
static const char *help = "help";
int main(int argc, char *argv[])
{
	char *file_name = argv[1];
	int width = atoi(argv[2]);
	int height = atoi(argv[3]);
	char out_file[256];
	int byte_index = 0;
	int pixel_index = 0;
	int in_size = 0;
	int out_size = 0;
	printf("Usage: <filename> <width> <height>\r\n");
	printf("file name: %s \r\n", file_name);
	printf("width: %d \r\n", width);
	printf("height: %d \r\n", height);
	FILE *raw_fb;
	raw_fb = fopen(file_name, "rb");
	if (raw_fb == NULL)
	{
		printf("open input file failed \r\n");
		return -1;
	}
	sprintf(out_file, "mipi_unpacked_%s", file_name);
	printf("open input file name: %s\r\n", out_file);
	FILE *pixel_fb = fopen(out_file, "wb");
	if (!pixel_fb)
	{
		printf("open output file failed \r\n");
		return -1;
	}
	printf("start convertion\r\n");
	in_size = width * height * 5 / 4;
	out_size = width * height * 2;
	fread(raw_array, 1, in_size, raw_fb);
	for (byte_index = 0, pixel_index = 0; byte_index < (in_size - 5);)
	{
		pixel_array[pixel_index + 0] = (((short)(raw_array[byte_index + 0])) << 2) & 0x03FC;
		pixel_array[pixel_index + 0] = pixel_array[pixel_index + 0] | (short)((raw_array[byte_index + 4] >> 0) & 0x0003);
		pixel_array[pixel_index + 1] = (((short)(raw_array[byte_index + 1])) << 2) & 0x03FC;
		pixel_array[pixel_index + 1] = pixel_array[pixel_index + 1] | (short)((raw_array[byte_index + 4] >> 2) & 0x0003);
		pixel_array[pixel_index + 2] = (((short)(raw_array[byte_index + 2])) << 2) & 0x03FC;
		pixel_array[pixel_index + 2] = pixel_array[pixel_index + 2] | (short)((raw_array[byte_index + 4] >> 4) & 0x0003);
		pixel_array[pixel_index + 3] = (((short)(raw_array[byte_index + 3])) << 2) & 0x03FC;
		pixel_array[pixel_index + 3] = pixel_array[pixel_index + 3] | (short)((raw_array[byte_index + 4] >> 6) & 0x0003);
		byte_index = byte_index + 5;
		pixel_index = pixel_index + 4;
	}
	fwrite(pixel_array, 1, out_size, pixel_fb);
	fclose(raw_fb);
	fclose(pixel_fb);
	printf("convert finished !!! \r\n");
	return 0;
}
