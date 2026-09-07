scheduler: main.o validacao.o
	gcc main.o validacao.o -o scheduler

main.o: main.c
	gcc -c main.c

validacao.o: validacao.c validacao.h
	gcc -c validacao.c

log:
	script -a evidencias.log -c "date; whoami; pwd; bash"

clean:
	rm -f *.o scheduler

.PHONY: clean log