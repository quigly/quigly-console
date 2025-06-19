#pragma once

#include "language_layer.h"
#include <string.h>

typedef struct
{
	char* src;
	size_t len;
} string_t;

#define string_literal(str) (string_t){ str, strlen(str) }

static inline string_t string_from(char* src)
{
	return (string_t){ src, strlen(src) };
}

static inline bool is_alpha(u8 ch)
{
    return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
}

static inline bool is_number(u8 ch)
{
    return ch >= '0' && ch <= '9';
}

static inline bool is_alpha_numeric(u8 ch)
{
    return is_alpha(ch) || is_number(ch);
}

static inline bool string_equals(string_t str1, string_t str2)
{
	if (str1.len != str2.len)
	{
		return false;
	}

	for (size_t i = 0; i < str1.len; i += 1)
	{
		if (str1.src[i] != str2.src[i])
		{
			return false;
		}
	}

	return true;
}
