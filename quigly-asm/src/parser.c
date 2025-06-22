#include "parser.h"

static const char* section_names[] =
{
    "OBJECT_SECTION_TEXT",
    "OBJECT_SECTION_DATA",
    "OBJECT_SECTION_BSS",
    "OBJECT_SECTION_RODATA",
    "OBJECT_SECTION_SYMBOLS",
    "OBJECT_SECTION_RELOCATION"
};
	
static const char* token_names[] =
{
	#define TOKEN(id) #id,
	#include "tokens.inc"
	#undef TOKEN
};

static const char* instruction_names[] =
{
	"add",
	"addi",
	"j"
};

static const char* register_names[] =
{
	"x0",
	"x1",
	"x2",
	"x3",
	"x4",
	"x5",
	"x6",
	"x7",
	"x8",
	"x9",
	"x10",
	"x11",
	"x12",
	"x13",
	"x14",
	"x15",
	"x16",
	"x17",
	"x18",
	"x19",
	"x20",
	"x21",
	"x22",
	"x23",
	"x24",
	"x25",
	"x26",
	"x27",
	"x28",
	"x29",
	"x30",
	"x31"
};

const char* ast_node_names[] =
{
	"AST_NODE_TYPE_PROGRAM",
	"AST_NODE_TYPE_LABEL",
	"AST_NODE_TYPE_INSTRUCTION",
	"AST_NODE_TYPE_OPERAND"
};

bool parser_init(parser_t* parser, token_list_t* token_list)
{
	memset(parser, 0, sizeof(parser_t));

	parser->token_begin = token_list->tokens;
	parser->token_end = token_list->tokens + token_list->length;
	parser->token = token_list->tokens;

	/*for (size_t i = 0; i < token_list->length; i += 1)
	{
		token_t* token = &token_list->tokens[i];

		printf("[%s] %.*s\n",
			token_names[token->type],
			(i32)token->source.len,
			token->source.src);
	}*/

	return true;
}

void parser_advance(parser_t* parser)
{
	if (parser->token >= parser->token_end)
	{
		return;
	}

	parser->token += 1;
}

token_type_e parser_peek(parser_t* parser, i32 count)
{
	if ((parser->token + count) >= parser->token_end)
	{
		return TOKEN_TYPE_EOF;
	}

	if ((parser->token + count) < parser->token_begin)
	{
		return TOKEN_TYPE_UNKNOWN;
	}

	return (parser->token + count)->type;
}

void parser_expect(parser_t* parser, token_type_e type)
{
	if (parser->token->type != type)
	{
		printf("Expected token %s, found %s\n", token_names[type], token_names[parser->token->type]);
		exit(1);
	}
}

bool parser_match_identifier(parser_t* parser, string_t name)
{
	if (parser->token->type != TOKEN_TYPE_IDENTIFIER)
	{
		return false;
	}

	if (string_equals(parser->token->source, name))
	{
		parser_advance(parser);
		return true;
	}

	return false;
}

ast_node_t* parser_alloc_node(parser_t* parser, ast_node_type_e type, ast_node_t* parent)
{
	ast_node_t* node = malloc(sizeof(ast_node_t));
	assert(node != NULL);
	memset(node, 0, sizeof(ast_node_t));
	node->type = type;
	node->parent = parent;

	if (parent != NULL)
	{
		if (parent->child == NULL)
		{
			parent->child = node;
		}
		else
		{
			ast_node_t* child = parent->child;
			assert(child != NULL);
			while (child->next != NULL)
			{
				child = child->next;
			}
			assert(child != NULL);
			child->next = node;
		}
	}

	return node;
}

static void print_ast_node(ast_node_t* node, i32 depth)
{
	assert(node != NULL);

	for (i32 i = 0; i < depth; i += 1)
	{
		printf("    ");
	}

	printf("%s [child: %p] [next: %p]\n", ast_node_names[node->type], node->child, node->next);

	switch (node->type)
	{
		case AST_NODE_TYPE_PROGRAM:
		{
		} break;

		case AST_NODE_TYPE_LABEL:
		{

		} break;
		
		case AST_NODE_TYPE_INSTRUCTION:
		{
		} break;
		
		case AST_NODE_TYPE_OPERAND:
		{

		} break;
	}

	if (node->child != NULL)
	{
		print_ast_node(node->child, depth + 1);
	}
	
	if (node->next != NULL)
	{
		print_ast_node(node->next, depth);
	}
}

void parse_program(parser_t* parser)
{
	assert(parser);

	ast_node_t* program = parser_alloc_node(parser, AST_NODE_TYPE_PROGRAM, NULL);
	parser->program = program;
	assert(program);
	printf("Beginning program\n");
	
	while (parser->token < parser->token_end)
	{
		/*printf("[%s] %.*s\n",
			token_names[parser->token->type],
			(i32)parser->token->source.len,
			parser->token->source.src);*/

		if (parser->token->type == TOKEN_TYPE_LABEL_DECL)
		{
			parse_label_decl(parser, program);
		}
		else if (parser->token->type == TOKEN_TYPE_IDENTIFIER)
		{
			parse_identifier(parser, program);
			// parser_expect(parser, TOKEN_TYPE_IDENTIFIER);
		}
		else
		{
			printf("[%.*s]\n",
				(i32)parser->token->source.len, parser->token->source.src);
			printf("Expected token %s in %s at %i:%i\n",
				token_names[parser->token->type], parser->token->path,
				parser->token->line, parser->token->col);
			unreachable;
		}
	}

	printf("Built AST\n");

	print_ast_node(program, 0);

	assert(parser->program->child != NULL);
}

void parse_label_decl(parser_t* parser, ast_node_t* parent)
{
	assert(parser);

	token_t* token = parser->token;

	ast_node_t* node = parser_alloc_node(parser, AST_NODE_TYPE_LABEL, parent);
	assert(node != NULL);
	node->token = token;

	parser_advance(parser);
	
	if (parser->token->type == TOKEN_TYPE_END_LINE)
	{
		parser_advance(parser);
		return;
	}
	else if (parser->token->type == TOKEN_TYPE_EOF)
	{
		return;
	}
	else
	{
		printf("Expected token %s in %s at %i:%i\n",
			token_names[parser->token->type], parser->token->path,
			parser->token->line, parser->token->col);
		exit(1);
	}
}

void parse_identifier(parser_t* parser, ast_node_t* parent)
{
	assert(parser != NULL);

	if (parser_is_instruction(parser->token->source))
	{
		ast_node_t* inst = parser_alloc_node(parser, AST_NODE_TYPE_INSTRUCTION, parent);
		assert(inst != NULL);
		inst->token = parser->token;

		if (parser_match_identifier(parser, string_from("addi")))
		{
			// if (!(parser_peek(parser, 1) == TOKEN_TYPE_IDENTIFIER)) { unreachable; }

			parser_expect(parser, TOKEN_TYPE_IDENTIFIER);
			token_t* rd_tok = parser->token;
			
			parser_advance(parser);
			parser_expect(parser, TOKEN_TYPE_COMMA);
			
			parser_advance(parser);
			parser_expect(parser, TOKEN_TYPE_IDENTIFIER);
			token_t* rs1_tok = parser->token;
			
			parser_advance(parser);
			parser_expect(parser, TOKEN_TYPE_COMMA);

			parser_advance(parser);
			parser_expect(parser, TOKEN_TYPE_INTEGER);
			token_t* imm_tok = parser->token;
			
			parser_advance(parser);
			parser_expect(parser, TOKEN_TYPE_END_LINE);
			parser_advance(parser);

			ast_node_t* op0 = parser_alloc_node(parser, AST_NODE_TYPE_OPERAND, inst);
			assert(op0 != NULL);
			ast_node_t* op1 = parser_alloc_node(parser, AST_NODE_TYPE_OPERAND, inst);
			assert(op1 != NULL);
			ast_node_t* op2 = parser_alloc_node(parser, AST_NODE_TYPE_OPERAND, inst);
			assert(op2 != NULL);

			op0->token = rd_tok;
			op1->token = rs1_tok;
			op2->token = imm_tok;
		}
		else if (parser_match_identifier(parser, string_from("j")))
		{
			parser_expect(parser, TOKEN_TYPE_IDENTIFIER);
			token_t* dst = parser->token;
			
			parser_advance(parser);
			parser_expect(parser, TOKEN_TYPE_END_LINE);
			parser_advance(parser);
			
			ast_node_t* op0 = parser_alloc_node(parser, AST_NODE_TYPE_OPERAND, inst);
			assert(op0 != NULL);
			op0->token = dst;
		}
		else if (parser_match_identifier(parser, string_from("lui")))
		{
			parser_expect(parser, TOKEN_TYPE_IDENTIFIER);
			token_t* rd_tok = parser->token;
			
			parser_advance(parser);
			parser_expect(parser, TOKEN_TYPE_COMMA);
			
			parser_advance(parser);
			parser_expect(parser, TOKEN_TYPE_IDENTIFIER);
			token_t* rs1_tok = parser->token;
			
			parser_advance(parser);
			parser_expect(parser, TOKEN_TYPE_END_LINE);
			parser_advance(parser);
		}
		else
		{
			printf("unhandled instruction %.*s in %s at %i:%i\n",
				(i32)parser->token->source.len, parser->token->source.src,
				parser->token->path, parser->token->line, parser->token->col);
			exit(1);
		}
	}
	else
	{
		printf("Expected identifier '%.*s' in %s at %i:%i\n",
			(i32)parser->token->source.len, parser->token->source.src,
			parser->token->path, parser->token->line, parser->token->col);
		exit(1);
	}
}

bool parser_is_instruction(string_t name)
{
	for (size_t i = 0; i < array_count(instruction_names); i += 1)
	{
		if (string_equals(name, string_from((char*)instruction_names[i])))
		{
			return true;
		}
	}

	return false;
}

bool parser_is_register(string_t name)
{
	for (size_t i = 0; i < array_count(register_names); i += 1)
	{
		if (string_equals(name, string_from((char*)register_names[i])))
		{
			return true;
		}
	}

	return false;
}
