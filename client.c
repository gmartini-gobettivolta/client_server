/* 	Semplice programma client che invia un messaggio al server
	gcc client.c -o client
	./client 127.0.0.1 5555
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

/* Funzione per uscire dal programma segnalando l'errore */
void error(char *msg)
{
  perror(msg);
  exit(0);
}

int main(int argc, char *argv[])
{
  int sock_fd, port_num, n;
  /* Contiene le informazioni di un host sulla rete */
  struct sockaddr_in serv_addr;

  /* Contiene le informazioni relative ad un host rese dalla funzione get hostbyname */
  struct hostent *server;

  char buffer[256];

  /* Controlla che gli argomenti passati da riga di comando ci siano tutti */
  if (argc < 3)	{
     fprintf(stderr,"Uso %s nome host/IP porta\n", argv[0]);
     exit(0);
  }

  /* Prende il numero di porta dalla riga di comando */
  port_num = atoi(argv[2]);

  /* Crea il socket */
  sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  /* Controllo sull'operazione */
  if (sock_fd < 0) error("ERROR opening socket");

  /* Converte un eventuale nome di host nel suo indirizzo ip */
  server = gethostbyname(argv[1]);

  /* Controllo sull'operazione */
  if (server == NULL) error ("ERROR, no such host\n");

  /* Predispone la struttura che deve contenere le informazioni */
  bzero((char *) &serv_addr, sizeof(serv_addr));

  /* Inserisce le informazioni nella struttura */
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(port_num);

  /* Cerca di connettersi al server */
  if (connect(sock_fd, (const struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
error("ERROR connecting");

  /* Si è connesso, chiede all'operatore il messaggio da inviare */
  printf("Inserisci il messaggio: ");

  /* Legge la tastiera */
  fgets(buffer,255,stdin);

  /* Lo invia in rete */
  n = write(sock_fd,buffer,strlen(buffer));
  if (n < 0) error("ERROR writing to socket");

  /* Legge la risposta */
  n = read(sock_fd,buffer,255);
  if (n < 0) error("ERROR reading from socket");
  buffer[n] = '\0';

  /* Stampa la risposta */
  printf("%s\n",buffer);
  return 0;
}
