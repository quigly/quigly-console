#pragma once

#include "language_layer.h"
#include "strings.h"

typedef enum
{
	#define TOKEN(id) TOKEN_TYPE_##id,
	#include "tokens.inc"
	#undef TOKEN
} token_type_e;

typedef struct
{
	token_type_e type;
	const char* path;
	string_t source;
	i32 line;
	i32 col;
} token_t;

typedef struct
{
	token_t* tokens;
	size_t length;
	size_t capacity;
} token_list_t;

typedef struct
{
	const char* source_begin;
	const char* source_end;
	char* source;
	char c;
	i32 line;
	i32 col;
} lexer_t;

bool lexer_init(const char* path, lexer_t* lexer);
void lexer_advance(lexer_t* lexer);
char lexer_peek(lexer_t* lexer, i32 count);
void lexer_skip_whitespace(lexer_t* lexer);
bool lexer_next_token(const char* path, lexer_t* lexer, token_list_t* token_list);
token_t* token_list_alloc(token_list_t* list);
void lexer_read_identifier(lexer_t* lexer, token_t* token);
void lexer_read_number(lexer_t* lexer, token_t* token);
void lexer_read_char(lexer_t* lexer, token_t* token, char expect, token_type_e type);
