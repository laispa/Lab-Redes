/****************************************/
/* FGA/EngSofrware/FRC - udpServer.c    */
/* Prof. Fernando W. Cruz               */
/****************************************/ 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /* close() */
#include <string.h> /* memset() */
#include <stdlib.h>

#define MAX_MSG 100

int main(int argc, char *argv[]) {
  int sd, rc, n, tam_Cli;
  struct sockaddr_in endCli;   /* Vai conter identificacao do cliente, endereço */ 
  struct sockaddr_in endServ;  /* Vai conter identificacao do servidor local, endereço */
  char   msg[MAX_MSG];/* Buffer que armazena os dados que chegaram via rede */
  char   msg_enviada[MAX_MSG];/* Buffer que armazena os dados que chegaram via rede */

  if (argc<3) {
	  printf("Digite IP e Porta para este servidor\n");
	  exit(1); }
/* Criacao do socket UDP */
  sd=socket(AF_INET, SOCK_DGRAM, 0); // diferente de fopen - sock-dgram (upd)
  if(sd<0) {
    printf("%s: nao pode abrir o socket \n",argv[0]);
    exit(1);  }

/* Preenchendo informacoes sobre o servidor */
  endServ.sin_family 	  = AF_INET;
  endServ.sin_addr.s_addr = inet_addr(argv[1]); // endereço ip do servidor
  endServ.sin_port 	  = htons(atoi(argv[2])); // a porta do servidor

/* Fazendo um bind na porta local do servidor */ // juntando as duas
  rc = bind (sd, (struct sockaddr *) &endServ,sizeof(endServ));
  if(rc<0) {
    printf("%s: nao pode fazer bind na porta %s \n", argv[0], argv[2]);
    exit(1); }

  printf("%s: esperando por dados no IP: %s, porta UDP numero: %s\n", argv[0], argv[1], argv[2]);

/* Este servidor entra num loop infinito esperando dados de clientes */
  while(1) {
    
    /* inicia o buffer */
    memset(msg,0x0,MAX_MSG);
    tam_Cli = sizeof(endCli);
    /* recebe a mensagem  */
    n = recvfrom(sd, msg, MAX_MSG, 0, (struct sockaddr *) &endCli, &tam_Cli); // aguardando algum remetente enviar informações
    if(n<0) {
      printf("%s: nao pode receber dados \n",argv[0]);
      continue;} 

    if (strncmp(msg, "FIM",3) == 0) 
			break;
    
      
    /* imprime a mensagem recebida na tela do usuario */
    printf("{UDP, IP_L: %s, Porta_L: %u", inet_ntoa(endServ.sin_addr), ntohs(endServ.sin_port));
    printf(" IP_R: %s, Porta_R: %u} => %s\n",inet_ntoa(endCli.sin_addr), ntohs(endCli.sin_port), msg);

    printf("Digite uma mensagem: ");
    fgets(msg_enviada, MAX_MSG, stdin);
   
    rc = sendto(sd, msg_enviada, strlen(msg_enviada), 0,(struct sockaddr *) &endCli, tam_Cli);
    if(rc<0) {
        printf("%s: nao pode enviar dados \n",argv[0]);
        close(sd);
        exit(1); 
     }

   if (strncmp(msg_enviada, "FIM",3) == 0) 
			break;
    
  } /* fim do while */
return 0;

} /* fim do programa */
