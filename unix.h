#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <semaphore.h> 
#include <pthread.h> 


#define UNIXSTR_PATH "/tmp/2101618"
#define UNIXDG_PATH  "/tmp/s.unixdfddffdfdgx"
#define UNIXDG_TMP   "/tmp/dgXXXXXfdjhfdjhfdXX"

pthread_mutex_t mutex_mensagem;
pthread_mutex_t mutex_resposta;
sem_t sem_validacao;	
sem_t sem_jogador;


