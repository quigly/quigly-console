#include "lexer.h"

bool lexer_init(const char* path, lexer_t* lexer)
{
	assert(path != NULL);
	assert(lexer != NULL);

	memset(lexer, 0, sizeof(lexer_t));

	{
		FILE* fp = fopen(path, "r");
		if (fp == NULL)
		{
			printf("Failed to open file %s\n", path);
			return false;
		}

		fseek(fp, 0, SEEK_END);
		const size_t size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		lexer->source = malloc(size);
		assert(lexer->source != NULL);

		fread(lexer->source, size, 1, fp);

		fclose(fp);

		lexer->source_begin = lexer->source;
		lexer->source_end = lexer->source + size;
	}

	lexer->line = 1;
	lexer->col = 1;
	lexer->c = *lexer->source;

	// printf("%s\n", lexer->source_begin);

	return true;
}

void lexer_advance(lexer_t* lexer)
{
	assert(lexer != NULL);

	if (lexer->source >= lexer->source_end)
	{
		lexer->c = 0;
		return;
	}

	lexer->source += 1;
	lexer->c = *lexer->source;

	if (lexer->c == '\n')
	{
		lexer->line += 1;
		lexer->col = 1;
	}
	else
	{
		lexer->col += 1;
	}
}

char lexer_peek(lexer_t* lexer, i32 count)
{
	assert(lexer != NULL);
	
	char* new_source = lexer->source + count;

	if (new_source < lexer->source_begin ||
		new_source >= lexer->source_end)
	{
		return 0;
	}

	return *new_source;
}

void lexer_skip_whitespace(lexer_t* lexer)
{
	assert(lexer != NULL);
	
	while (lexer->c == ' ' || lexer->c == '\t')
	{
		lexer_advance(lexer);
	}
}

bool lexer_next_token(const char* path, lexer_t* lexer, token_list_t* token_list)
{
	assert(path != NULL);
	assert(lexer != NULL);
	
	lexer_skip_whitespace(lexer);

	if (lexer->c == 0 || lexer->source >= lexer->source_end)
	{
		return false;
	}

	// TODO: check for #include statements and recursively tokenize included files

	token_t* token = token_list_alloc(token_list);
	assert(token != NULL);
	token->type = TOKEN_TYPE_UNKNOWN;
	token->path = path;
	token->line = lexer->line;
	token->col = lexer->col;
	token->source.src = lexer->source;

	switch (lexer->c)
	{
		case 'a' ... 'z': case 'A' ... 'Z': case '_': lexer_read_identifier(lexer, token); break;
		case '0' ... '9': lexer_read_number(lexer, token); break;
        case '!': lexer_read_char(lexer, token, '!', TOKEN_TYPE_EXCLAMATION); break;
        case '@': lexer_read_char(lexer, token, '@', TOKEN_TYPE_AT); break;
        case '#': lexer_read_char(lexer, token, '#', TOKEN_TYPE_HASH); break;
        case '$': lexer_read_char(lexer, token, '$', TOKEN_TYPE_DOLLAR); break;
        case '%': lexer_read_char(lexer, token, '%', TOKEN_TYPE_PERCENT); break;
        case '^': lexer_read_char(lexer, token, '^', TOKEN_TYPE_CARROT); break;
        case '&': lexer_read_char(lexer, token, '&', TOKEN_TYPE_AND); break;
        case '*': lexer_read_char(lexer, token, '*', TOKEN_TYPE_ASTERISK); break;
        case '(': lexer_read_char(lexer, token, '(', TOKEN_TYPE_PAREN_LEFT); break;
        case ')': lexer_read_char(lexer, token, ')', TOKEN_TYPE_PAREN_RIGHT); break;
        case '=': lexer_read_char(lexer, token, '=', TOKEN_TYPE_EQUALS); break;
        case '-': lexer_read_char(lexer, token, '-', TOKEN_TYPE_MINUS); break;
        case '+': lexer_read_char(lexer, token, '+', TOKEN_TYPE_PLUS); break;
        case '{': lexer_read_char(lexer, token, '{', TOKEN_TYPE_CURLY_BRACKET_LEFT); break;
        case '}': lexer_read_char(lexer, token, '}', TOKEN_TYPE_CURLY_BRACKET_RIGHT); break;
        case '[': lexer_read_char(lexer, token, '{', TOKEN_TYPE_SQUARE_BRACKET_LEFT); break;
        case ']': lexer_read_char(lexer, token, '}', TOKEN_TYPE_SQUARE_BRACKET_RIGHT); break;
        case ':': lexer_read_char(lexer, token, ':', TOKEN_TYPE_COLON); break;
        case ';': lexer_read_char(lexer, token, ';', TOKEN_TYPE_SEMICOLON); break;
        case ',': lexer_read_char(lexer, token, ',', TOKEN_TYPE_COMMA); break;
        case '.': lexer_read_char(lexer, token, '.', TOKEN_TYPE_PERIOD); break;
        case '<': lexer_read_char(lexer, token, '<', TOKEN_TYPE_LESS_THAN); break;
        case '>': lexer_read_char(lexer, token, '>', TOKEN_TYPE_GREATER_THAN); break;
        case '/': lexer_read_char(lexer, token, '/', TOKEN_TYPE_SLASH); break;
        case '?': lexer_read_char(lexer, token, '?', TOKEN_TYPE_QUESTION); break;
        case '`': lexer_read_char(lexer, token, '`', TOKEN_TYPE_GRAVE); break;
        case '~': lexer_read_char(lexer, token, '~', TOKEN_TYPE_SQUIGLE); break;
        case '|': lexer_read_char(lexer, token, '|', TOKEN_TYPE_PIPE); break;
        case '\n': lexer_read_char(lexer, token, '\n', TOKEN_TYPE_END_LINE); break;

		default:
			printf("Unknown token '%c' in %s at %i:%i\n", lexer->c, path, lexer->line, lexer->col);
			exit(1);
	}

	static const char* token_names[] =
	{
		#define TOKEN(id) #id,
		#include "tokens.inc"
		#undef TOKEN
	};

	// printf("%s %i:%i [%i]%.*s\n", token_names[token->type], token->line, token->col, (i32)token->source.len, (i32)token->source.len, token->source.src);

	return true;
}

token_t* token_list_alloc(token_list_t* list)
{
	assert(list != NULL);

	if (list->tokens == NULL)
	{
		list->capacity = 2;
		list->length = 1;
		list->tokens = malloc(sizeof(token_t) * list->capacity);
		assert(list->tokens != NULL);

		return list->tokens;
	}
	
	if (list->length >= list->capacity)
	{
		list->capacity *= 2;
		list->tokens = realloc(list->tokens, list->capacity * sizeof(token_t));
		assert(list->tokens != NULL);
	}

	token_t* token = &list->tokens[list->length];
	memset(token, 0, sizeof(token_t));
	list->length += 1;
	
	return token;
}

void lexer_read_identifier(lexer_t* lexer, token_t* token)
{
	assert(lexer != NULL);
	assert(token != NULL);

	token->type = TOKEN_TYPE_IDENTIFIER;
	token->source.len = 0;

	while (is_alpha_numeric(lexer->c) || lexer->c == '_')
	{
		lexer_advance(lexer);
		token->source.len += 1;
	}

	assert(token->source.len > 0);

	if (lexer->c == ':')
	{
		token->type = TOKEN_TYPE_LABEL_DECL;
		lexer_advance(lexer);
	}
}

void lexer_read_number(lexer_t* lexer, token_t* token)
{
	assert(lexer != NULL);
	assert(token != NULL);

	token->type = TOKEN_TYPE_INTEGER;
	token->source.len = 0;

	while (is_number(lexer->c))
	{
		lexer_advance(lexer);
		token->source.len += 1;
	}
}

void lexer_read_char(lexer_t* lexer, token_t* token, char expect, token_type_e type)
{
	assert(lexer != NULL);
	assert(token != NULL);
	assert(type != TOKEN_TYPE_UNKNOWN);
	
	assert(lexer->c == expect);

	token->type = type;
	token->source.len = 1;

	lexer_advance(lexer);

	// skip multi line breaks
	if (expect == '\n')
	{
		while (lexer->c == '\n')
		{
			lexer_advance(lexer);
		}
	}
}













