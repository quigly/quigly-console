#pragma once

#include "language_layer.h"
#include "lexer.h"

typedef enum
{
	AST_NODE_TYPE_PROGRAM,
	AST_NODE_TYPE_LABEL,
	AST_NODE_TYPE_INSTRUCTION,
	AST_NODE_TYPE_OPERAND
} ast_node_type_e;

typedef struct ast_node_s ast_node_t;
struct ast_node_s
{
	ast_node_type_e type;
	ast_node_t* parent;
	ast_node_t* next;
	ast_node_t* child;
	token_t* token;

	union
	{
		struct
		{
		} program;

		struct
		{
			string_t name;
			u32 offset;
		} label;

		struct
		{
			string_t name;
		} instruction;

		struct
		{
			i32 integer;
			f32 number;
			string_t string;
		} operand;
	};
};

typedef struct
{
	token_t* token_begin;
	token_t* token_end;
	token_t* token;

	ast_node_t* program;
} parser_t;

bool parser_init(parser_t* parser, token_list_t* token_list);
void parser_advance(parser_t* parser);
token_type_e parser_peek(parser_t* parser, i32 count);
void parser_expect(parser_t* parser, token_type_e type);
bool parser_match_identifier(parser_t* parser, string_t name);
ast_node_t* parser_alloc_node(parser_t* parser, ast_node_type_e type, ast_node_t* parent);
void parse_program(parser_t* parser);
void parse_label_decl(parser_t* parser, ast_node_t* parent);
void parse_identifier(parser_t* parser, ast_node_t* parent);
bool parser_is_instruction(string_t name);
bool parser_is_register(string_t name);

