#include "lexer.h"
#include "parser.h"

#define MAX_LABELS 512

typedef struct label_t label_t;
struct label_t
{
	char name[128];
	u32 address;
	ast_node_t* node;
	label_t* next;
};

static const char* ast_node_names[] =
{
	"AST_NODE_TYPE_PROGRAM",
	"AST_NODE_TYPE_LABEL",
	"AST_NODE_TYPE_INSTRUCTION",
	"AST_NODE_TYPE_OPERAND"
};

static label_t* labels;
static u32 current_address;

static void insert_label(string_t name, u32 address, ast_node_t* node)
{
	assert(node != NULL);

	label_t* label;

	if (labels == NULL)
	{
		label = malloc(sizeof(label_t));
		assert(label != NULL);
	}
	else
	{
		label_t* t = labels;
		while (t->next != NULL)
		{
			t = t->next;
		}
		t->next = malloc(sizeof(label_t));
		assert(t->next != NULL);
		label = t->next;
	}

	assert(label != NULL);
	memset(label, 0, sizeof(label_t));
	label->node = node;
	strncpy(label->name, name.src, name.len);
	label->address = address;
	
	printf("Inserted label %s at 0x%08X\n", label->name, address);
}

i32 main(i32 argc, char** argv)
{
	char* output_file = "out.bin";
	char* input_file;

	if (argc <= 1)
	{
		printf("Missing path!\n");
	}

	for (i32 i = 1; i < argc; i += 1)
	{
		if (strcmp(argv[i], "-o") == 0 ||
			strcmp(argv[i], "--output") == 0 &&
			i + 1 < argc)
		{
			output_file = argv[i + 1];
			i += 1;
		}
		else
		{
			input_file = argv[i];
		}
	}

	token_list_t token_list = { 0 };

	lexer_t lexer;
	lexer_init(input_file, &lexer);
	while (lexer_next_token(input_file, &lexer, &token_list)) {};

	parser_t parser;
	if (!parser_init(&parser, &token_list))
	{
		return 1;
	}
	parse_program(&parser);

	printf("Building label table\n");
	ast_node_t* node = parser.program->child;
	assert(node != NULL);
	while (node != NULL)
	{
		// printf("[%s]\n", ast_node_names[node->type]);
		if (node->type == AST_NODE_TYPE_LABEL)
		{
			string_t name = node->token->source;
			// printf("Found label %.*s\n", (i32)name.len, name.src);
			insert_label(name, current_address, node);

		}
		else if (node->type == AST_NODE_TYPE_INSTRUCTION)
		{
			current_address += 4;
		}

		node = node->next;
	}

	return 0;
}
