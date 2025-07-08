#include "language_layer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define BIT_CLEAR(n, k) ((n) & (~(1 << ((k) - 1))))

i32 main(i32 argc, char** argv)
{
	if (argc <= 1)
	{
		printf("Missing path!\n");
		return 1;
	}

	const char* in_path = argv[1];

	i32 src_width;
	i32 src_height;
	i32 comp;
	u8* src = stbi_load(in_path, &src_width, &src_height, &comp, STBI_rgb_alpha);
	if (src_width != 128 || src_height != 128)
	{
		printf("Invalid image size! The image must be 128x128 pixels, got %ix%i\n", src_width, src_height);
		return 1;
	}
	if (src == NULL)
	{
		printf("Failed to open %s\n", in_path);
		return 1;
	}
	// printf("comp: %i\n", comp);

	u8* dst_pixels = calloc(1, 4096);
	assert(dst_pixels != NULL);

	u32 bit_offset = 0;
	
	if (comp == 4)
	{
		for (u32 pixel_index = 0; pixel_index < (128 * 128); pixel_index += 1)
		{
			const u32 src_color = ((u32*)src)[pixel_index];
			const u8* rgba = (u8*)&src_color;
			
			const u8 r = rgba[0];
			const u8 g = rgba[1];
			const u8 b = rgba[2];

			u8 value = 0;

			if (r == 0x00 && g == 0x00 && b == 0x00)
			{
				value = 0b00;
			}
			else if (r == 0x51 && g == 0x51 && b == 0x52)
			{
				value = 0b01;
			}
			else if (r == 0xad && g == 0xad && b == 0xad)
			{
				value = 0b10;
			}
			else if (r == 0xff && g == 0xff && b == 0xff)
			{
				value = 0b11;
			}
			else
			{
				printf("Invalid pixel color at index %u\n", pixel_index);
				return 1;
			}

			const u32 byte_index = bit_offset / 8;
			const u32 bit_index = bit_offset % 8;

			const u32 shift = 6 - (bit_index & ~1);

			dst_pixels[byte_index] &= ~(1 << bit_index);
			dst_pixels[byte_index] |= (value & 0x3) << bit_index;

			bit_offset += 2;
		}
	}
	else
	{
		printf("Unsupported pixel composition!\n");
		return 1;
	}

	/*
	bit_offset = 0;
	for (u32 i = 0; i < 4; i += 1)
	{
		u8 value = dst_pixels[i];

		for (u32 b = 0; b < 4; b += 1)
		{
			const u32 byte_index = bit_offset / 8;
			const u32 bit_index = bit_offset % 8;

			const u8 byte = dst_pixels[byte_index];
			const u8 shift = bit_index;
			const u8 pixel = (byte >> shift) & 0x3;

			printf("p[%u]: %u\n", bit_offset, pixel);
			
			bit_offset += 2;
		}
	}*/

	stbi_image_free(src);
	src = NULL;

	char out_path[512];
	strncpy(out_path, in_path, 512);
	const u32 out_path_len = strlen(out_path);
	char* dot = strchr(out_path, '.');
	dot[1] = 'b';
	dot[2] = 'i';
	dot[3] = 'n';
	dot[4] = 0;

	{
		FILE* fp = fopen(out_path, "wb");
		if (fp == NULL)
		{
			printf("Failed to open file %s\n", out_path);
			return 1;
		}

		fwrite(dst_pixels, 4096, 1, fp);

		fclose(fp);
	}

	free(dst_pixels);
	dst_pixels = NULL;

	return 0;
}
