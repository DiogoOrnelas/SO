#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h> 
#include <pthread.h>
#include "unix.h"

#define MAXLINE 12
#define N 9
int solution[9][9]={};    //Declaração do tabuleiro completo

typedef struct  {
	int tempo_jogo;
	int tempo_medio;
	int inicio_simulacao;
	int tempo_simulacao;
	int num_jogadores;
}configs;
configs configuracao;

void inicializaConfs(){ 	// guarda nas vari�veis os valores definidos no ficheiro de configuracao
	FILE *fileConfs;
	fileConfs = fopen("config.txt", "r");

	char parametro[100];
	int dados;
		
	if (fileConfs == NULL) {
         printf("ERRO: impossivel ler o ficheiro de configuracao\n");    
    } 
	else {
		while( fscanf(fileConfs, "%s : %d", parametro, &dados) != EOF ){
			if(strcmp(parametro, "tempo_jogo") == 0){
					configuracao.tempo_jogo = dados;
			}			
			if(strcmp(parametro, "tempo_medio") == 0){
					configuracao.tempo_medio = dados;
			}			
			if(strcmp(parametro, "inicio_simulacao") == 0){
					configuracao.inicio_simulacao = dados;
			}			
			if(strcmp(parametro, "tempo_simulacao") == 0){
					configuracao.tempo_simulacao = dados;
			}
			if(strcmp(parametro, "num_jogadores") == 0){
					configuracao.num_jogadores = dados;
			}
		}
	}
	printf("\n ------ Configuracoes: ------\n" );
	printf("tempo_jogo: %d \n", configuracao.tempo_jogo );
	printf("tempo_medio: %d \n", configuracao.tempo_medio );
	printf("inicio_simulacao: %d \n", configuracao.inicio_simulacao );
	printf("tempo_simulacao: %d \n", configuracao.tempo_simulacao );
	printf("num_jogadores: %d \n", configuracao.num_jogadores );	
	fclose(fileConfs);
}

/* Servidor do tipo socket stream.
   Manda linhas recebidas de volta para o cliente */


/* //Função para criar o tabuleiro por resolver
void criaTabuleiro(){
	int i, j;
	FILE *fp;
	int tabuleiro[N][N]={
						{9, 4, 0, 1, 0, 2, 0, 5, 8},
						{6, 0, 0, 0, 5, 0, 0, 0, 4},
						{0, 0, 2, 4, 0, 3, 1, 0, 0},
						{0, 2, 0, 0, 0, 0, 0, 6, 0},
						{5, 0, 8, 0, 2, 0, 4, 0, 1},
						{0, 6, 0, 0, 0, 0, 0, 8, 0},
						{0, 0, 1, 6, 0, 8, 7, 0, 0},
						{7, 0, 0, 0, 4, 0, 0, 0, 3},
						{4, 3, 0, 5, 0, 9, 0, 1, 2}};
	
	fp = fopen ("Tabuleiro", "w");
		if (fp == NULL) return 0;
		//fprintf(fp, "%d %d\n", N, N);
		for(i=0;i<N; i++){
			for( j=0;j<N;j++){
				fprintf(fp, "%d", tabuleiro[i][j]);
			}
			fprintf(fp,"\n");
		}
		fclose(fp);
}  */

//Função para ler o tabuleiro completo
void leSolucao(){
	int i, j;
	
	FILE *fd;
	fd = fopen ("TabuleiroFinal", "r");
		if (fd == NULL) return 0;	
		for(i=0;i<9; i++){
			for( j=0;j<9;j++){
				int num = (int)fgetc(fd)-48;
				if(num == -38){
					num = (int)fgetc(fd)-48;
					solution[i][j]= num;
				}else{	
					solution[i][j]= num;
					
				}
				printf("%d",solution[i][j]);
			}
			
			printf("\n");
		}
	fclose(fd);
}




void validarResposta(char *mensagem, int sockfd) {
	int IDmensagem,Resultado, IDEquipa, IDJogador, linha, coluna, numero;

	sscanf(mensagem,"%d %d %d %d %d %d", &IDmensagem, &Resultado, &IDJogador, &linha, &coluna, &numero);	
	memset(mensagem, 0, sizeof mensagem);
	printf("O jogador %d enviou para %dx%d o numero:%d \n\n", IDJogador, linha+1, coluna+1, numero);	
	if (numero == solution[linha][coluna]) {    // -1 pq a a tebela foi guardada com inidice 0 
		printf("O jogador %d preencheu a posicao %dx%d corretamente (numero:%d) \n\n", IDJogador, linha+1, coluna+1, numero);

		char sendline[MAXLINE];
		sprintf(sendline,"5 1 %d %d %d %d", IDJogador, linha, coluna, numero);		// 1 = acertou na numero
		int n;
		n = strlen(sendline);
		if (write(sockfd, sendline, n) != n)
			err_dump("str_cli: writen error on socket");

	}
		
	else {
		printf("O jogador %d falhou a jogada do numero %d na posicao %dx%d \n\n",IDJogador, numero,linha+1, coluna+1);
		//Coloca no socket a resposta para ser mostrada no monitor
		char sendline[MAXLINE];
		sprintf(sendline,"5 0 %d %d %d %d", IDJogador, linha, coluna, numero);		// 0 = errou na numero
		int n;
		n = strlen(sendline);

		if (write(sockfd, sendline, n) != n)
			err_dump("str_cli: writen error on socket");

	}
	sem_post(&sem_jogador);	
	
}


str_echo(sockfd)
int sockfd;
{
	char line[MAXLINE];
	int n;
	
	for(;;) {

		/* Lê uma linha do socket */
		

		n = readline(sockfd, line, MAXLINE);

		if (n == 0){
			return;
		}
		else if (n < 0)
			err_dump("str_echo: readline error");
		//criaTabuleiro();
		/*ESCREVE O TABULEIRO NO FICHEIRO*/
		int v;
		validarResposta(line,sockfd);

		/* Manda linha de volta para o socket. n conta com
		o \0 da string, caso contrário perdia-se sempre 
		um caracter! */
		if (writen(sockfd, line, n) != n)
			err_dump("str_echo: writen error");
	}
}
