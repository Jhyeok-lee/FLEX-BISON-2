#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "timer.h"

FILE* f;

struct basic_block_head* _head;
struct basic_block* _block;
struct basic_block_expr* _expr;

void visit_expr(void)
{
	if( _expr->expr == NULL )
		return;

	if( strcmp( _expr->expr, ";" ) == 0 )
		fprintf(f, "\n");
	else
		fprintf(f, "%s", _expr->expr);

	if( _expr->next != NULL ) {
		_expr = _expr->next;
		visit_expr();
	}
}

void print_predecessor(void)
{
	int i;

	fprintf(f, "Predecessor:");

	if( _block->predecessor[0] == NULL ) {
		fprintf(f, "start\n");
		return;
	}

	for(i=0; i < _block->predecessor_number; i++)
		fprintf(f, "B%d ", _block->predecessor[i]->branch_number );

	fprintf(f, "\n");
}

void print_successor(void)
{
	int i;

	fprintf(f, "Successor:");

	if( _block->successor[0] == NULL ) {
		fprintf(f, "end\n");
		return;
	}

	for(i=0; i < _block->successor_number; i++)
		fprintf(f, "B%d ", _block->successor[i]->branch_number );

	fprintf(f, "\n");
}

void visit_block(void)
{
	fprintf(f, "\nB%d\n{\n", _block->branch_number);

	_expr = _block->expr;
	if( _expr != NULL )
		visit_expr();

	fprintf(f, "}\n");
	print_predecessor();
	print_successor();
}

void visit_block_head(void)
{
	int i;

	fprintf(f, "--------%s--------\n", _head->function_name);

	for(i=0; i < _head->branch_number; i++ ) {
		_block = _head->block[i];
		visit_block();
	}

	if( _head->next != NULL ) {
		_head = _head->next;
		visit_block_head();
	}
}

int main(void)
{
	f = fopen("CFG.out", "w");

	fprintf(f, "# Control Flow Graph\n");
	fprintf(f, "\n");

	if(!yyparse())
		dfs();

	_head = b_head;

	if( _head == NULL )
		return 0;

	visit_block_head();

	return 0;
}