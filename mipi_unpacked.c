#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char raw_array[4032*3024*2]; //4032*3024*2byte
short pixel_array[4032*3024*2]; //4032*3024*2byte
static const char *format_array[5] = {"RAW8", "RAW10", "RAW12", "RAW14", "RAW16"};
static const char *help = "help";
int main(int argc, char *argv[])
{
char *file_name = argv[1];
char *format = argv[2];
int width = atoi(argv[3]);
int height = atoi(argv[4]);
char out_file[256];
int format_index;
int byte_index = 0;
int pixel_index = 0;
int in_size = 0;
int out_size = 0;
printf("Usage: <filename> <format: RAW8/RAW10/RAW12/RAW14/RAW16> <width> <height>\r\n");
printf("file name: %s \r\n", file_name);
printf("format: %s \r\n", format);
printf("width: %d \r\n", width);
printf("height: %d \r\n", height);
//file_name = /home/chengyuw/Desktop/Diane_ISP/2021/0107/1x/Camera/test.raw
FILE * raw_fb;
raw_fb = fopen(file_name,"rb");
if (raw_fb!=NULL)
{
	printf("open input file faile \r\n");
	//return -1;
}
sprintf(out_file, "out_%s_%s", format, file_name);
printf("open input file name: %s\r\n", out_file);
FILE *pixel_fb = fopen(out_file, "wb");
if(!pixel_fb)
{
printf("open output file faile \r\n");
return -1;
}
for(int i=0; i<5; i++)
{
if(0 == strcmp(format_array[i], format))
{
format_index = i;
break;
}
format_index = 5;
}
printf("start convertion format_index: %d \r\n", format_index);
switch(format_index)
{
case 0:
in_size = width*height;
out_size = width*height;
break;
case 1:
in_size = width*height*5/4;
out_size = width*height*2;
break;
case 2:
in_size = width*height*12/8;
out_size = width*height*2;
break;
case 3:
in_size = width*height*14/8;
out_size = width*height*2;
break;
case 4:
in_size = width*height*2;
out_size = width*height*2;
break;
default:
printf("format is unsupport !!! \r\n");
break;
}
fread(raw_array, 1, in_size, raw_fb);
switch(format_index)
{
case 0:
// RAW8 don't need convert
break;
case 1:
// RAW10
for(byte_index=0, pixel_index=0; byte_index < (in_size-5); )
{
pixel_array[pixel_index+0] = (((short)(raw_array[byte_index+0]))<<2) & 0x03FC;
pixel_array[pixel_index+0] = pixel_array[pixel_index+0] | (short)((raw_array[byte_index+4]>>0) & 0x0003);
pixel_array[pixel_index+1] = (((short)(raw_array[byte_index+1]))<<2) & 0x03FC;
pixel_array[pixel_index+1] = pixel_array[pixel_index+1] | (short)((raw_array[byte_index+4]>>2) & 0x0003);
pixel_array[pixel_index+2] = (((short)(raw_array[byte_index+2]))<<2) & 0x03FC;
pixel_array[pixel_index+2] = pixel_array[pixel_index+2] | (short)((raw_array[byte_index+4]>>4) & 0x0003);
pixel_array[pixel_index+3] = (((short)(raw_array[byte_index+3]))<<2) & 0x03FC;
pixel_array[pixel_index+3] = pixel_array[pixel_index+3] | (short)((raw_array[byte_index+4]>>6) & 0x0003);
byte_index = byte_index + 5;
pixel_index = pixel_index + 4;
}
break;
case 2:
// RAW12
for(byte_index=0, pixel_index=0; byte_index < (in_size-3); )
{
pixel_array[pixel_index+0] = (((short)(raw_array[byte_index+0]))<<4) & 0x0FF0;
pixel_array[pixel_index+0] = pixel_array[pixel_index+0] | (short)((raw_array[byte_index+2]>>0) & 0x000F);
pixel_array[pixel_index+1] = (((short)(raw_array[byte_index+1]))<<4) & 0x0FF0;
pixel_array[pixel_index+1] = pixel_array[pixel_index+1] | (short)((raw_array[byte_index+2]>>4) & 0x000F);
byte_index = byte_index + 3;
pixel_index = pixel_index + 2;
}
break;
case 3:
//RAW14
for(byte_index=0, pixel_index=0; byte_index < (in_size-7); )
{
pixel_array[pixel_index+0] = (((short)(raw_array[byte_index+0]))<<6) & 0x3FC0;
pixel_array[pixel_index+0] = pixel_array[pixel_index+0] | (short)((raw_array[byte_index+4]>>0) & 0x003F);
pixel_array[pixel_index+1] = (((short)(raw_array[byte_index+1]))<<6) & 0x3FC0;
pixel_array[pixel_index+1] = pixel_array[pixel_index+1] | (short)((raw_array[byte_index+4]>>6) & 0x0003);
pixel_array[pixel_index+1] = pixel_array[pixel_index+1] | (short)((raw_array[byte_index+5]>>0) & 0x000F);
pixel_array[pixel_index+2] = (((short)(raw_array[byte_index+2]))<<6) & 0x3FC0;
pixel_array[pixel_index+2] = pixel_array[pixel_index+2] | (short)((raw_array[byte_index+5]>>4) & 0x000F);
pixel_array[pixel_index+2] = pixel_array[pixel_index+2] | (short)((raw_array[byte_index+6]>>0) & 0x0003);
pixel_array[pixel_index+3] = (((short)(raw_array[byte_index+3]))<<6) & 0x3FC0;
pixel_array[pixel_index+3] = pixel_array[pixel_index+3] | (short)((raw_array[byte_index+6]>>2) & 0x003F);
byte_index = byte_index + 7;
pixel_index = pixel_index + 4;
}
break;
case 4:
// RAW16 don't need convert
break;
default:
printf("formt is unsupport !!! \r\n");
break;
}
fwrite(pixel_array, 1, out_size, pixel_fb);
fclose(raw_fb);
fclose(pixel_fb);
printf("convert finished !!! \r\n");
return 0;
}
