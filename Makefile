all:
	flex c2c.l
	bison -d c2c.y
	gcc lex.yy.c c2c.tab.c ast.c cfg.c -o cfg -lfl -g
	gcc lex.yy.c c2c.tab.c ast.c liveness.c -o liveness -lfl -g

clean:
	rm -rf lex.yy.c c2c.tab.c c2c.tab.h cfg CFG.out liveness Liveness.out
