#
#
#	Makefile for test programs
#
#


all: Monitor Simulador

Monitor: Monitor.o util.o Monitor-conf.o
	gcc -lpthread -o Monitor Monitor.o util.o Monitor-conf.o 

Simulador: Simulador.o util.o Simulador-conf.o
	gcc -lpthread -o Simulador Simulador.o util.o Simulador-conf.o 

Simulador.o: Simulador.c
	gcc -lpthread -c Simulador.c

Monitor.o: Monitor.c
	gcc -lpthread -c Monitor.c

Simulador-conf.o: Simulador-conf.c
	gcc -lpthread -c Simulador-conf.c

Monitor-conf.o: Monitor-conf.c
	gcc -lpthread -c Monitor-conf.c

util.o: util.c
	gcc -c util.c


clean:
	rm -f *.o *~ Monitor Simulador

