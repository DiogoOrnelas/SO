/* Cliente do tipo socket stream.
   Lê linhas do teclado e envia-as para o servidor */

#include "unix.h"

main(void)
{
	int sockfd, servlen;
	struct sockaddr_un serv_addr;
	/* Cria socket stream */
	sem_init(&sem_jogador,1,1);
	sem_init(&sem_validacao,1,1);
	if ((sockfd= socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
		err_dump("client: can't open stream socket");

	/* Primeiro uma limpeza preventiva!
	   Dados para o socket stream: tipo + nome do ficheiro.
         O ficheiro identifica o servidor */

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sun_family = AF_UNIX;
	strcpy(serv_addr.sun_path, UNIXSTR_PATH);
	servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);
	
	for (;;) {
		/* Tenta estabelecer uma ligação. Só funciona se o servidor tiver 
			sido lançado primeiro (o servidor tem de criar o ficheiro e associar
			o socket ao ficheiro) */
		
		if (connect(sockfd, (struct sockaddr *) &serv_addr, servlen) < 0)
			err_dump("client: can't connect to server");

		/* Envia as linhas lidas do teclado para o socket */
		
		str_cli(stdin, sockfd);
		

		/* Fecha o socket e termina */
	}
	close(sockfd);
	exit(0);
}


