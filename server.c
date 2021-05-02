/* 	Semplice programma server che riceve un messaggio dal client e risponde
	gcc server.c -o server
	./server 5555
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

void error(char *msg)
{
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[])
{
  int sock_fd, new_sock_fd, port_num, client_len;
  char buffer[256];
  /* Contiene le informazioni di un host sulla rete */
  struct sockaddr_in serv_addr, cli_addr;
  int n;

  /* Controlla che sia stata specificata la porta sulla linea di comando */
  if (argc < 2)	{
  	puts("ERROR, non è stata specificata la porta di ascolto\n");
  	exit(1);
  }

  /* Crea il socket */
  sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  /* Controlla che l'operazione sia riuscita */
  if (sock_fd < 0)  error("ERRORE nel creare il socket:");

  /* Predispone la struttura che conterrà le informazioni del server */
  bzero((char *) &serv_addr, sizeof(serv_addr));

  /* Prende il numero di porta passato sulla riga di comando */
  port_num = atoi(argv[1]);

  /* Riempe la struttura con le informazioni della connessione
      Tipo di socket, indirizzi su cui restare in ascolto, porta con relativa conversione
      The htonl() function converts the unsigned integer hostlong from host byte order to network byte
       order */
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(port_num);

  /* Collega il socket alle informazioni relative alla connessione */
  if (bind(sock_fd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) error("ERROR on binding");

  /* Si mette in ascolto in attesa di client che si connettano */
  listen(sock_fd,5);

  /* Predispone lo spazio per i dati necessari */
  client_len = sizeof(cli_addr);

  /*Attende un client, accetta la connessione prelevando i dati del client */
  new_sock_fd = accept(sock_fd, (struct sockaddr *) &cli_addr, &client_len);

  /* Un client si è connesso, controllo sull'operazione ...*/
  if (new_sock_fd < 0)  error("ERRORE con accept");

  /* Predispone il buffer per i dati, azzerandolo */
  bzero(buffer,256);

  /* Legge i dati arrivati */
  n = read(new_sock_fd,buffer,255);
  if (n < 0) error("ERROR reading from socket");

  /* Stampa i dati arrivati */
  printf("Questo è il messaggio ricevuto: %s\n",buffer);

  /* Invia il messaggio di avvenuta ricezione */
  n = write(new_sock_fd,"Messaggio ricevuto",18);

  if (n < 0) error("ERRORE scrivendo nel socket");

  /* Chiude la connessione */
  close(sock_fd);
  close(new_sock_fd);
  return 0;
}
