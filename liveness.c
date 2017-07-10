#include <stdio.h>
#include <string.h>
#include "timer.h"
#include "ast.h"

FILE* f;

struct basic_block_head* _head;
struct basic_block* _block;

void print_liveness(void)
{
	int i;
	int j;

	while(1) {

		fprintf(f, "---------------%s---------------\n", _head->function_name);
		fprintf(f, "\n     IN          OUT\n");

		for(i=0; i<_head->branch_number; i++) {

			_block = _head->block[i];

			fprintf(f, "B%d : ", i);

			fprintf(f, "{ ");
			for(j=0; j<_block->in_number; j++)
				fprintf(f, "%s ", _block->in[j] );
			fprintf(f, "} ");

			fprintf(f, "{ ");
			for(j=0; j<_block->out_number; j++)
				fprintf(f, "%s ", _block->out[j] );
			fprintf(f, "}\n");
		}


		if( _head->next != NULL )
			_head = _head->next;
		else
			return;
	}
}

void _delete_repeated(void)
{
	int i;
	int j;
	int k;

	for(i=0; i<_block->in_number; i++) {
		for(j=0; j<_block->in_number; j++) {

			if( i != j && strcmp( _block->in[i], _block->in[j] ) == 0) {

				for(k=i; k<_block->in_number; k++)
					_block->in[k] = _block->in[k+1];

				_block->in[ _block->in_number-1 ] = NULL;
				_block->in_number--;

				i=0;
				j=0;
			}
		}
	}

	for(i=0; i<_block->out_number; i++) {
		for(j=0; j<_block->out_number; j++) {

			if( i != j && strcmp( _block->out[i], _block->out[j] ) == 0) {

				for(k=i; k<_block->out_number; k++)
					_block->out[k] = _block->out[k+1];

				_block->out[ _block->out_number-1 ] = NULL;
				_block->out_number--;

				i=0;
				j=0;
			}
		}
	}	
}

void delete_repeated(void)
{
	int i;

	while(1) {

		for(i=0; i<_head->branch_number; i++) {

			_block = _head->block[i];

			_delete_repeated();
		}

		if( _head->next != NULL )
			_head = _head->next;
		else
			return;
	}
}

void substraction_set(char* d[], char* a[], char* b[], int* d_len, int a_len, int b_len)
{
	int i;
	int j;
	int u;
	u = 0;

	for(i=0; i<a_len; i++) {
		for(j=0; j<b_len; j++) {

			if( strcmp( a[i], b[j] ) == 0)
				u = 1;
		}

		if( u == 0 ) {
			d[ (*d_len) ] = (char*)malloc( sizeof( a[i] ) + 1);
			strcpy( d[ (*d_len) ], a[i] );
			(*d_len)++;
		}

		u = 0;
	}
}

void reset_set(char* d[], int* d_len)
{
	int i;

	for(i=0; i<50; i++)
		d[i] = NULL;
	(*d_len) = 0;
}

void union_set(char* d[], char* s[], int* d_len, int s_len)
{
	int i;
	int j;
	int u;
	u = 0;

	for(i=0; i<s_len; i++){
		for(j=0; j<(*d_len); j++) {

			if( strcmp( s[i], d[j] ) == 0 )
				u = 1;
		}

		if( u == 0 ) {
			d[ (*d_len) ] = (char*)malloc( sizeof( s[i] ) + 1);
			strcpy( d[ (*d_len) ], s[i] );
			(*d_len)++;
		}

		u = 0;
	}
}

void initialize_in(void)
{
	int i;

	for(i=0; i<_block->use_number; i++ ) {

		_block->in[i] = (char*)malloc( sizeof( _block->use[i] ) + 1 );
		strcpy( _block->in[i], _block->use[i] );
	}

	_block->in_number = _block->use_number;
}

void iterative_liveness(void)
{
	int i;
	int j;
	int is_change;
	int prev_n;
	int next_n;
	unsigned int newout_number;
	char* newout[50];

	while(1) {

		for(i=0; i<_head->branch_number; i++ ) {

			_block = _head->block[i];
			initialize_in();
		}

		if( _head->next != NULL )
			_head = _head->next;
		else
			break;
	}

	while(1) {

		is_change = 0;

		for(i=_head->branch_number-1; i>=0; i-- ) {

			_block = _head->block[i];
			
			for(j=0; j<_block->successor_number; j++ ) {

				prev_n = _block->out_number;
				union_set( _block->out, _block->successor[j]->in, &(_block->out_number), _block->successor[j]->in_number );
				next_n = _block->out_number;

				if( (prev_n - next_n) != 0 )
					is_change++;
			}

			prev_n = _block->in_number;
			union_set( _block->in, _block->use, &(_block->in_number), _block->use_number );
			next_n = _block->in_number;

			if( (prev_n - next_n) != 0 )
				is_change++;

			reset_set(newout, &newout_number);
			substraction_set(newout, _block->out, _block->def, &newout_number, _block->out_number, _block->def_number);

			prev_n = _block->in_number;
			union_set( _block->in, newout, &(_block->in_number), newout_number );
			next_n = _block->in_number;

			if( (prev_n - next_n) != 0 )
				is_change++;
		}

		if( _head->next == NULL && is_change == 0 )
			return;

		if( _head->next != NULL )
			_head = _head->next;
		else
			_head = b_head;

	}
}

int main(void)
{
	double t;
	f = fopen("Liveness.out", "w");

	fprintf(f, "# The Result of Liveness Analysis\n");
	fprintf(f, "\n");

	if(!yyparse())
		dfs();

	_head = b_head;

	if( _head == NULL )
		return 0;

	set_timer();
	iterative_liveness();
	t = get_timer();
	printf("Time : %lf\n", t);

	_head = b_head;
	delete_repeated();

	_head = b_head;
	print_liveness();

	return 0;
}