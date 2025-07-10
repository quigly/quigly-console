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
	if (src_width % 8 != 0)
	{
		printf("Width must be divisible by 8!\n");
		return 1;
	}
	if (src_height % 8 != 0)
	{
		printf("Height must be divisible by 8!\n");
		return 1;
	}
	if (src == NULL)
	{
		printf("Failed to open %s\n", in_path);
		return 1;
	}
	
	const size_t total_bits = src_width * src_height * 2;
	const size_t total_bytes = total_bits / 8;

	u8* dst_pixels = calloc(1, total_bytes);
	assert(dst_pixels != NULL);

	u32 bit_offset = 0;
	
	if (comp == 4)
	{
		const u32 sprites_wide = src_width / 8;
		const u32 sprites_tall = src_height / 8;

		for (u32 sprite_y = 0; sprite_y < sprites_tall; sprite_y += 1)
		{
			for (u32 sprite_x = 0; sprite_x < sprites_wide; sprite_x += 1)
			{
				for (u32 oy = 0; oy < 8; oy += 1)
				{
					for (u32 ox = 0; ox < 8; ox += 1)
					{
						const u32 pixel_index = ((sprite_x * 8) + ox) + ((sprite_y * 8) + oy) * src_width;

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
			}
		}
	}
	else
	{
		printf("Unsupported pixel composition!\n");
		return 1;
	}

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

		const size_t bytes_written = fwrite(dst_pixels, 1, total_bytes, fp);
		printf("Wrote %lu bytes\n", bytes_written);

		fclose(fp);
	}

	free(dst_pixels);
	dst_pixels = NULL;

	return 0;
}
