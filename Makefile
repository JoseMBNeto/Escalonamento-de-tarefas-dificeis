scheduler: main.o validacao.o tarefa.o
	gcc main.o validacao.o tarefa.o -o scheduler

main.o: main.c
	gcc -c main.c

tarefa.o: tarefa.c tarefa.h
	gcc -c tarefa.c

validacao.o: validacao.c validacao.h
	gcc -c validacao.c

log:
	script -a evidencias.log -c "date; whoami; pwd; bash"

clean:
	rm -f *.o scheduler

.PHONY: clean log