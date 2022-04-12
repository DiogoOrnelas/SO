#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h> 
#include <semaphore.h> 
#include "unix.h"
#define MAXLINE 12
#define N 9

int x;
struct buffer_t{
  int sockfd;
}; 

struct  equipa{   //TESTE?
	int IDEquipa;
	int IDJogador;
	//int tentativas;
};


struct ThreadData {   
     pthread_t thread_id;       
     int       thread_num;
	 int	   thread_done;     
	 int	   *socket;
	 char 	   *message;
} ;
struct ThreadData thread_data_array[N];

/*
void *thread_start(void *thread)
{
	ThreadData *my_data  = (ThreadData*)thread;
	struct ThreadData *my_data;

	my_data = (struct ThreadData *) thread;
	taskid = my_data->thread_id;
	num = my_data->thread_num;
	done = my_data->thread_done;
	message = my_data->message;
	//there is no guarantee that prints will be in order
	//  we will use its initial thread->num ,cause it differs for each thread
	//plus you will see how threads will behave
	int order=my_data->thread_num;
	printf("%i) before num T: %i\n",order, my_data->thread_num);
	//my_data->thread_num=4;
	//printf("%i) after assignment num T: %i\n",order ,my_data->thread_num);

   return NULL;
}*/



/* Cliente do tipo socket stream.
   Lê string de fp e envia para sockfd.
   Lê string de sockfd e envia para stdout */
const char file_name[] = "Tabuleiro";
int solving[9][9]={};

int leTabuleiro(){
	int i, j;
	FILE *fd;
	fd = fopen ("Tabuleiro", "r");
		if (fd == NULL) return 0;	
		for(i=0;i<9; i++){
			for( j=0;j<9;j++){
				int num = (int)fgetc(fd)-48;
				if(num == -38){
					num = (int)fgetc(fd)-48;
					solving[i][j]= num;
				}else{	
					solving[i][j]= num;
				}
				printf("%d",solving[i][j]);
			}
			printf("\n");
		}
	fclose(fd);
	return solving;
}

//Funcao para imprimir o tabuleiro 
void imprimeTabuleiro(){
	int i, j;
	for(i=0;i<9; i++){
		for( j=0;j<9;j++){
			printf("%d",solving[i][j]);
		}
		printf("\n");		
	}
}

//Funcao para ler o ficheiro
void criaficheiro(){
	int ficheiro;
	char output[1000];
	ficheiro = open(file_name, O_RDONLY);

	if(access(file_name, F_OK ) != -1){ //verifica o acesso ao ficheiro
		printf("Lendo o ficheiro: \n");
		read(ficheiro,output,100);
		close(ficheiro);
		printf(output);
		printf("\n");
	}
	else{
		printf("Erro no acesso ao ficheiro\n");
		exit(0);
	}
}

bool resposta(char *mensagem){
	
	int IDMensagem, resultado, IDEquipa, IDJogador, numero, linha, coluna;

	sscanf(mensagem,"%d %d %d %d %d %d", &IDMensagem, &resultado, &IDJogador, &linha, &coluna, &numero);
	
	if (IDMensagem == 5) {
		return true;
	}else{
		return false;
	}	
}

void updateMsg(char *mensagem){
	int IDMensagem, resultado,IDEquipa, IDJogador, numero, linha, coluna;
	
	sscanf(mensagem,"%d %d %d %d %d %d %d", &IDMensagem, &resultado, &IDJogador,  &linha, &coluna, &numero);
	
	switch (IDJogador) { 
		case 1: { 
			thread_data_array[0].message = mensagem;
			break;
		} 
		case 2: { 
			thread_data_array[1].message = mensagem;
			break;
		} 
		case 3: { 
			thread_data_array[2].message = mensagem;
			break;
		} 
		case 4: { 
			thread_data_array[3].message = mensagem;
			break;
		}
		case 5: { 
			thread_data_array[4].message = mensagem;
			break;
		} 
		case 6: { 
			thread_data_array[5].message = mensagem;
			break;
		} 
		case 7: { 
			thread_data_array[6].message = mensagem;
			break; 
		}
		case 8: { 
			thread_data_array[7].message = mensagem;
			break;
		} 
		case 9: { 
			thread_data_array[8].message = mensagem;
			break;
		}
		
	}
}

//Le o socket!!!!
void lesocket(int socket){
	int n;
	char recvline[MAXLINE+1];
	n = readline(socket, recvline, MAXLINE);
	updateMsg(recvline);
}
//Recebe uma resposta do servidor
bool recebeResposta(char *mensagem){
	
	int IDMensagem, resultado, IDEquipa, IDJogador, numero, linha, coluna;

	sscanf(mensagem,"%d %d %d %d %d %d", &IDMensagem, &resultado, &IDJogador, &linha, &coluna, &numero);
	
	if (resultado == 1) {
		return true;
	}else if (resultado == 0) {
		return false;
	}	
}

//Envia uma mensagem para o servidor -----------------------------------------------------------------------------------
void Escreve_Mensagem(int socket,int IDJogador, int linha, int coluna, int numero){ 
	
	int n;
	int IDmensagem = 1;
	int IDEquipa = getpid();
	int resultado=0;
	char sendline[MAXLINE];
	sprintf(sendline,"%d %d %d %d %d %d", IDmensagem, resultado, IDJogador, linha, coluna, numero);
	thread_data_array[IDJogador-1].message = sendline;
	n = strlen(sendline);
	if (writen(socket, sendline, n) != n)
		err_dump("str_cli: writen error on socket");
}


//Verifica se o numero escolhido existe na linha
bool UsedInRow(int grid[N][N], int linha, int num){
	int coluna;
    for (coluna = 0; coluna < N; coluna++){
        if (grid[linha][coluna] == num){
			//printf("o numero %d já é usado na linha %d \n", num, linha+1);
            return true;
		}
	}
    return false;
}
 

/*Funcoes do sudoku?
//Verifica se o numero escolhido existe na coluna
bool UsedInCol(int grid[N][N], int coluna, int num){
	int linha;
    for (linha = 0; linha < N; linha++){
        if (grid[linha][coluna] == num){
            return true;
		}
	}
    return false;
}
 
//Verifica se o numero escolhido existe no "quadrado 3x3"
bool UsedInBox(int grid[N][N], int boxStartRow,int boxStartCol, int num){
	int linha, coluna;
    for ( linha = 0; linha < 3; linha++){
        for ( coluna = 0; coluna < 3; coluna++){
            if (grid[linha + boxStartRow][coluna + boxStartCol] ==num){
                return true;
			}
		}
	}
    return false;
}

//Verifica se o numero escolhido pode ser inserido
bool isSafe( int grid[N][N], int linha,int coluna, int num){

    return !UsedInRow(grid, linha, num) && !UsedInCol(grid, coluna, num) && !UsedInBox(grid, linha - linha % 3, coluna - coluna % 3, num) && grid[linha][coluna] == 0;
}
*/

//Funcao para verificar se há casas por preencher
bool FindUnassignedLocation(int grid[N][N], int linha){


	int coluna;
    //for (linha = 0; linha < N; linha++){
        for (coluna = 0; coluna < N; coluna++){
            if (grid[linha][coluna] == 0){
                return true;
			}
		}
	//}
    return false;
}
int escolheColuna(int grid[N][N], int linha){
	int coluna=0;
	while(coluna<9){
		if (grid[linha][coluna] == 0){
			//printf("existe uma casa vazia em %dx%d \n", linha+1,coluna+1);
			return coluna;
		}else{
			coluna++;
		}
	}
}


void *completaTabuleiro(void *threadarg){
	
	struct ThreadData *my_data;
	my_data = (struct ThreadData *) threadarg;
	int linha, col;
	int num=1;
	int done=0;
	int numlinha,taskid;
    char *mensagem;
	int socket;

    taskid = my_data->thread_id;
    numlinha = my_data->thread_num;
    done = my_data->thread_done;
	socket = my_data->socket;
    mensagem = my_data->message;
	int IDEquipa = getpid();
	int IDJogador = numlinha +1;
	
	col = escolheColuna(solving, numlinha);
	while(done==0){   //Enquanto não acabar a sua linha, continua ativo
		while (num<=9){
			int y=1;
			if(!UsedInRow(solving,numlinha, num)){   //Verifica se o número não existe na linha
				//sem_wait(&sem_jogador);
				
				Escreve_Mensagem(socket, IDJogador, numlinha, col, num);//envia mensagem para saber se está certo

				//sem_wait(&sem_validacao);                 ---------------

				sleep(1);
				
				/*while(!resposta(mensagem)){
					lesocket(socket);
					mensagem = my_data->message;
				}*/

				lesocket(socket);
				mensagem = my_data->message;

				
				if (recebeResposta(mensagem)){	  //caso a resposta do servidor seja positiva
					//sem_post(&sem_jogador);
					printf("\n O jogador %d preencheu a posicao %dx%d corretamente com o numero %d \n", IDJogador, numlinha+1, col+1, num);
					solving[numlinha][col]= num;
					col=escolheColuna(solving, numlinha); //prox "casa"
					num=1;
				}else{							//caso a resposta do servidor seja negativa
					//sem_post(&sem_jogador);	
					printf("\n O jogador %d falhou a jogada do numero %d na posicao %dx%d \n",IDJogador, num, numlinha+1, col+1);
					num++;
				}
				

			}else if (!FindUnassignedLocation(solving, numlinha)){  //Verifica se a linha já está toda preenchida
				//TERMINAR A TAREFA (APAGAR JOGADOR?)
				printf("\n O jogador %d  preencheu completamente a sua linha (%d) e abandonou o jogo \n",IDJogador, numlinha+1);
				imprimeTabuleiro();
				
				done =1;
				my_data->thread_done =1;
				pthread_exit(NULL);

				//break;
			}else{   //Se o número já é usado na linha,
				num++;
			}
		}
		num=1;
	}
}




void teste(){
   int i;
   //ThreadData thread[N]; 
   pthread_t threads[N];
	int *taskids[N];
	int rc, t, num;
	int done =0;
	//char *messages;
	char *testemensagem = "1 0 0 0 0 1";
   for(i=0; i<N; i++){
		thread_data_array[i].thread_id = i;
   		thread_data_array[i].thread_num = i;
		thread_data_array[i].thread_done = done;
		//thread_data_array[i].socket = socket;
		//thread_data_array[i].tab = solving[i];
  		thread_data_array[i].message = testemensagem;
   		rc = pthread_create(&threads[i], NULL, completaTabuleiro, (void *) &thread_data_array[i]);
    	if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
   }
	pthread_exit(NULL);
}

/*VER O ESTADO DA SIMULAÇÃO
void Estado(){
	printf("\nEstado da Simulação\n");
	if(iniciado== 1)
	{
		tempInic = tempo_universal;
		printf("\n1. Estado atual: Simulação a correr.\n");
		printf("2. Nº de equipas: %d\n", Equipas);
		printf("3. Jogadores atuais: %d\n", Jogadores);
		printf("4. Nº de desistências: %d\n", Desistencias);
		printf("5. Nº de Tentativas: %d\n", Tentativas);

		//printf("8. Tempo de Simulação: %d\n", (FUNÇÃO PARA MEDIR O TEMPO));
	}
	else
	{
		printf("\n1. Estado atual: Simulação terminada.\n");
	}
}
*/



str_cli(fp, sockfd)
FILE *fp;
int sockfd;
{
	int n, i;
	int teste123;
	char sendline[MAXLINE], recvline[MAXLINE+1];
	int opcao;
	

	for (i=0;i<9;i++){
		thread_data_array[i].socket = sockfd;

	}
	leTabuleiro();
	printf("1 - Iniciar a simulacao \n"
	"2 - Mostrar o tabuleiro\n \n"
	"3 - Imprimir estado do jogo \n"
	"4 - Terminar a simulacao \n\n");
	opcao = getchar() - 48;		
	if(opcao == 1){
		teste();
	}else if(opcao == 2){
		imprimeTabuleiro();
	}else if(opcao == 3){
		//Estado();
	}else if(opcao == 4){
		
	}
	
	
	while (fgets(sendline, MAXLINE, fp) != NULL) {
		for(;;){
				/* Envia string para sockfd. Note-se que o \0 não 
				é enviado */
	
			n = strlen(sendline);
			if (writen(sockfd, sendline, n) != n)
				err_dump("str_cli: writen error on socket");

			/* Tenta ler string de sockfd. Note-se que tem de 
			terminar a string com \0 */

			n = readline(sockfd, recvline, MAXLINE);
			
			if (n<0)
				err_dump("str_cli:readline error");
			recvline[n] = 0;
			
			/* Envia a string para stdout */
			//recebeResposta(recvline);
			//criaficheiro();
			//imprimeTabuleiro();
			//fputs(recvline, stdout);
		}
	}
	if (ferror(fp))
		err_dump("str_cli: error reading file");
}
