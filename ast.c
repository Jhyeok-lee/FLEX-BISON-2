#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

struct basic_block_head* current_block_head;
struct basic_block* current_block;
struct basic_block* prev_block;

struct basic_block* cond_block[30];
unsigned int cond_number;

void add_use(char* name)
{
	current_block->use[ current_block->use_number ] = (char*)malloc( sizeof(name) + 1 );
	strcpy( current_block->use[ current_block->use_number], name );
	current_block->use_number++;
}

void add_def(char* name)
{
	current_block->def[ current_block->def_number ] = (char*)malloc( sizeof(name) + 1 );
	strcpy( current_block->def[ current_block->def_number], name );
	current_block->def_number++;
}

void add_block_head(char* name)
{
	struct basic_block_head* new_block_head;
	new_block_head = (struct basic_block_head*)malloc( sizeof( struct basic_block_head ) );

	new_block_head->function_name = (char*)malloc( sizeof( name ) + 1 );
	strcpy( new_block_head->function_name, name ) ;
	new_block_head->branch_number = 0;

	if( b_head == NULL )
		b_head = new_block_head;

	if( current_block_head != NULL )
		current_block_head->next = new_block_head;

	current_block_head = new_block_head;
	current_block = NULL;
	prev_block = NULL;
}

void add_block(void)
{
	struct basic_block* new_block;
	new_block = (struct basic_block*)malloc( sizeof( struct basic_block ) );

	if( current_block_head->block[0] == NULL ) {
		current_block_head->block[0] = new_block;
		current_block_head->branch_number++;
		current_block = new_block;
		return;
	}

	current_block_head->block[ current_block_head->branch_number ] = new_block;
	new_block->branch_number = current_block_head->branch_number;
	current_block_head->branch_number++;

	current_block->successor[ current_block->successor_number ] = new_block;
	current_block->successor_number++;
	new_block->predecessor[ new_block->predecessor_number ] = current_block;
	new_block->predecessor_number++;

	current_block = new_block;
}

void add_block_expr(char *name)
{
	struct basic_block_expr* new_block_expr;
	new_block_expr = (struct basic_block_expr*)malloc( sizeof( struct basic_block_expr ) );

	new_block_expr->expr = (char*)malloc( sizeof( name )+1 );
	strcpy(new_block_expr->expr, name);

	if( current_block->expr == NULL ) {
		current_block->expr = new_block_expr;
		return;
	}

	struct basic_block_expr* temp = current_block->expr;

	while( temp->next != NULL )
		temp = temp->next;

	temp->next = new_block_expr;
}

void add_pre_two(void)
{
	int i;

	for(i=0; i<cond_block[ cond_number ]->predecessor_number; i++) {

		if( cond_block[ cond_number ]->predecessor[i] == current_block )
			return;
	}

	cond_block[ cond_number ]->predecessor[ cond_block[cond_number]->predecessor_number ] = current_block;
	cond_block[ cond_number ]->predecessor_number++;	
}

void add_suc_two(void)
{
	int i;

	for(i=0; i<current_block->successor_number; i++) {

		if( current_block->successor[i] == cond_block[ cond_number ] )
			return;
	}

	current_block->successor[ current_block->successor_number ] = cond_block[ cond_number ];
	current_block->successor_number++;	
}

void load_cond(void)
{
	int i;
	cond_number--;

	add_suc_two();
	add_pre_two();
}

void save_cond(void)
{
	cond_block[ cond_number ] = current_block;
	cond_number++;
}

void add_pre(void)
{
	int i;

	for(i=0; i<prev_block->predecessor_number; i++) {

		if( prev_block->predecessor[i] == current_block)
			return;
	}

	prev_block->predecessor[ prev_block->predecessor_number ] = current_block;
	prev_block->predecessor_number++;
}

void add_suc(void)
{
	int i;

	for(i=0; i<current_block->successor_number; i++) {

		if( current_block->successor[i] == prev_block)
			return;
	}

	current_block->successor[ current_block->successor_number ] = prev_block;
	current_block->successor_number++;
}

void iterative_add(void)
{
	add_pre();
	add_suc();
}

void save(void)
{
	if( current_block == NULL )
		return;

	prev_block = current_block;
}

void load(void)
{
	if( prev_block == NULL )
		return;

	current_block = prev_block;
}

void new_branch(void)
{
	if( current_block != NULL && current_block->expr == NULL )
		return;

	save();

	add_block();
}

void dfs(void)
{
	if( head == NULL ) {
		fprintf(stderr, "program does not exist.\n");
		return;
	}

	if( head->decl != NULL )
		Declaration( head->decl );

	if( head->func != NULL )
		Function( head->func );
}

void Declaration(struct DECLARATION* declaration)
{
	if( declaration->prev != NULL )
		Declaration( declaration->prev );

	if( declaration->t == eInt );
	
	if( declaration->t == eFloat );

	if( declaration->id != NULL )
		Identifier( declaration->id );
}

void Identifier(struct IDENTIFIER* identifier)
{
	if( identifier->prev != NULL ) 
		Identifier( identifier->prev );

	if( identifier->ID != NULL );

	if( identifier->intnum != 0 );

}

void Function(struct FUNCTION* function)
{
	if( function->prev != NULL )
		Function( function->prev );

	add_block_head( function->ID );
	
	if( function->t == eInt );

	if( function->t == eFloat );

	if( function->ID != NULL );

	if( function->param != NULL )
		Parameter( function->param );

	if( function->cstmt != NULL )
		Compoundstmt( function->cstmt );

	if( function->cstmt == NULL )
		new_branch();
}

void Parameter(struct PARAMETER* parameter)
{
	if( parameter->prev != NULL )
		Parameter( parameter->prev );

	if( parameter->t == eInt );

	if( parameter->t == eFloat );

	if( parameter->id != NULL )
		Identifier( parameter->id );
}

void Compoundstmt(struct COMPOUNDSTMT* compoundstmt)
{
	if( compoundstmt->decl != NULL )
		Declaration( compoundstmt->decl );

	if( compoundstmt->stmt != NULL ) {
		new_branch();
		Stmt( compoundstmt->stmt );
	}
}

void Stmt(struct STMT* stmt)
{
	if( stmt->prev != NULL )
		Stmt( stmt->prev );

	if( stmt->s == eAssign && stmt->stmt.assign_ != NULL ) 
		Assign( stmt->stmt.assign_ );

	if( stmt->s == eCall && stmt->stmt.call_ != NULL ) 
		Call( stmt->stmt.call_ );

	if( stmt->s == eRet && stmt->stmt.return_ != NULL ) {
		add_block_expr("return ");
		Expr( stmt->stmt.return_ );
		add_block_expr(";");
	}

	if( stmt->s == eWhile && stmt->stmt.while_ != NULL ) {
		new_branch();
		While_s( stmt->stmt.while_ );
	}

	if( stmt->s == eFor && stmt->stmt.for_ != NULL )
		For_s( stmt->stmt.for_ );

	if( stmt->s == eIf && stmt->stmt.if_ != NULL )
		If_s( stmt->stmt.if_ );

	if( stmt->s == eCompound && stmt->stmt.cstmt_ != NULL )
		Compoundstmt( stmt->stmt.cstmt_ );

	if( stmt->s == eSemi && stmt->stmt.cstmt_ != NULL )
		Compoundstmt( stmt->stmt.cstmt_ );
}

void Assign(struct ASSIGN* assign)
{
	add_block_expr(assign->ID);
	add_def(assign->ID);

	if( assign->index != NULL ) {
		add_block_expr("[");
		Expr( assign->index );
		add_block_expr("]");
	}

	add_block_expr("=");

	if( assign->expr != NULL )
		Expr( assign->expr );

	add_block_expr(";");
}

void Call(struct CALL* call)
{
	add_block_expr(call->ID);

	add_block_expr("(");
	if( call->arg != NULL ) 
		Arg( call->arg );
	add_block_expr(")");

	add_block_expr(";");
}

void Arg(struct ARG* arg)
{
	if( arg->prev != NULL ) {
		Arg( arg->prev );
		add_block_expr(",");
	}

	if( arg->expr != NULL )
		Expr( arg->expr );
}

void While_s(struct WHILE_S* while_s)
{
	if( while_s->do_while == 0 ) {

		if( while_s->cond != NULL ) {
			new_branch();
			save_cond();
			Expr( while_s->cond );
			add_block_expr(";");
		}

		if( while_s->stmt != NULL ) {
			new_branch();
			Stmt( while_s->stmt );
			load_cond();
			load();
		}
	}
	
	if( while_s->do_while == 1 ) {

		if( while_s->stmt != NULL ) {
			new_branch();
			save_cond();
			Stmt( while_s->stmt );
		}

		if( while_s->cond != NULL ) {
			new_branch();
			Expr( while_s->cond );
			add_block_expr(";");
			load_cond();
			load();
		}

	}

	new_branch();
}

void For_s(struct FOR_S* for_s)
{
	if( for_s->init != NULL )
		Assign( for_s->init );

	if( for_s->cond != NULL ) {
		new_branch();
		save_cond();
		Expr( for_s->cond );
		add_block_expr(";");
	}

	if( for_s->stmt != NULL ) {
		new_branch();
		Stmt( for_s->stmt );
	}

	if( for_s->inc != NULL )  {
		Assign( for_s->inc );
		load_cond();
		load();
	}

	new_branch();

}

void If_s(struct IF_S* if_s)
{
	if( if_s->cond != NULL ) {
		new_branch();
		Expr( if_s->cond );
		add_block_expr(";");
	}

	if( if_s->if_ != NULL ) {
		new_branch();
		Stmt( if_s->if_ );
		load();
	}

	if( if_s->else_ != NULL ) {
		new_branch();
		Stmt( if_s->else_ );
		load();
	}

	new_branch();
}

void Expr(struct EXPR* expr)
{
	if( expr->e == eUnop && expr->expression.unop_ != NULL )
		Unop( expr->expression.unop_ );

	if( expr->e == eAddi  && expr->expression.addiop_ != NULL )
		Addiop( expr->expression.addiop_ );

	if( expr->e == eMulti && expr->expression.multop_ != NULL )
		Multop( expr->expression.multop_ );

	if( expr->e == eRela && expr->expression.relaop_ != NULL )
		Relaop( expr->expression.relaop_ );

	if( expr->e == eEqlt && expr->expression.eqltop_ != NULL )
		Eqltop( expr->expression.eqltop_ );

	if( expr->e == eCallExpr && expr->expression.call_ != NULL ) 
		Call( expr->expression.call_);

	if( expr->e == eId && expr->expression.ID_ != NULL )
		Id_s( expr->expression.ID_ );

	if( expr->e == eExpr && expr->expression.bracket != NULL ) {
		add_block_expr("(");
		Expr( expr->expression.bracket );
		add_block_expr(")");
	}

	if( expr->e == eIntnum ) {
		char buf[10];
		sprintf(buf, "%d", expr->expression.intnum);
		add_block_expr(buf);
	}

	if( expr->e == eFloatnum ){
		char buf[20];
		sprintf(buf, "%f", expr->expression.floatnum);
		add_block_expr(buf);
	}
}

void Unop(struct UNOP* unop)
{
	if( unop->u == eNegative )
		add_block_expr("-");

	if( unop->expr != NULL )
		Expr( unop->expr );
}

void Addiop(struct ADDIOP* addiop)
{
	if( addiop->lhs != NULL )
		Expr( addiop->lhs );

	if( addiop->a == ePlus )
		add_block_expr("+");

	if( addiop->a == eMinus )
		add_block_expr("-");

	if( addiop->rhs != NULL )
		Expr( addiop->rhs );
}

void Multop(struct MULTOP* multop)
{
	if( multop->lhs != NULL )
		Expr( multop->lhs );

	if( multop->m == eMult )
		add_block_expr("*");

	if( multop->m == eDiv )
		add_block_expr("/");

	if( multop->rhs != NULL )
		Expr( multop->rhs );
}

void Relaop(struct RELAOP* relaop)
{
	if( relaop->lhs != NULL )
		Expr( relaop->lhs );

	if( relaop->r == eLT )
		add_block_expr("<");

	if( relaop->r == eGT )
		add_block_expr(">");

	if( relaop->r == eLE )
		add_block_expr( "<=" );

	if( relaop->r == eGE ) 
		add_block_expr(  "<=" );

	if( relaop->rhs != NULL )
		Expr( relaop->rhs );
}

void Eqltop(struct EQLTOP* eqltop)
{

	if( eqltop->lhs != NULL )
		Expr( eqltop->lhs );

	if( eqltop->e == eEQ )
		add_block_expr( "==" );

	if( eqltop->e == eNE ) 
		add_block_expr( "!=" );

	if( eqltop->rhs != NULL )
		Expr( eqltop->rhs );
}

void Id_s(struct ID_S* id_s)
{
	add_block_expr(id_s->ID);
	add_use(id_s->ID);

	if( id_s->expr != NULL ) {
		add_block_expr("[");
		Expr( id_s->expr );
		add_block_expr("]");
	}
}
