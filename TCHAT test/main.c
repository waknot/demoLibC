#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "server.h"

static void init(void)
{
//socket windows
#ifdef WIN32
  WSADATA wsa;
  int err = WSAStartup(MAKEWORD(2, 2), &wsa);
  if(err < 0)
    {
      puts("WSAStartup failed !");
      exit(EXIT_FAILURE);
    }
#endif
}

static void end(void)
{
#ifdef WIN32
  WSACleanup();
#endif
}

static void app(void)
{
  SOCKET sock = init_connection(); /*Socket de type int
  char buffer[BUF_SIZE];       /*BUF_SIZE = 1024 (defini dans le .h)*/
  int actual = 0;              /* the index for the array */
  int max = sock;
  
 
  
  Client clients[MAX_CLIENTS];    /* an array for all clients 
									MAX_CLIENTS = 100 (défini dans le .h)
								  donc un tableau de structure
								  client(socket sock, char name[buf_size]))*/
  
  fd_set rdfs;  			/*initialisation d'une variable file descriptor	(type fd_set)*/

  while(1)
    {
      int i = 0;
	  
      FD_ZERO(&rdfs);			 /* void FD_ZERO(fd_set *set);
									initialise le file descriptor
									pour qu'il soit à zéro bits */

      
      FD_SET(STDIN_FILENO, &rdfs);  /* void FD_SET(int fd, fd_set *set); 
										add STDIN_FILENO ??? */

      
      FD_SET(sock, &rdfs);   /* add the connection socket au file descriptor */

      for(i = 0; i < actual; i++)   /* add socket of each client */
	{
	  FD_SET(clients[i].sock, &rdfs); /* param de gauche dans param de droite */
	}

      if(select(max + 1, &rdfs, NULL, NULL, NULL) == -1)   /*int select(int rdfs, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout); 
															select permet a un programme de surveiller plusieurs file descriptor*/
	{
	  perror("select()"); 			/*retourne la derniere erreur connue + string 'select()')*/
	  exit(errno);
	}

     
      if(FD_ISSET(STDIN_FILENO, &rdfs))   /* something from standard input : i.e keyboard */
	{
	  
	  break;                              /* stop process when type on keyboard */
	}
      else if(FD_ISSET(sock, &rdfs))
	{
	 
	  SOCKADDR_IN csin = { 0 };          /* new client */
												
	  size_t sinsize = sizeof csin; /* size_t est pour définir un int unsigned */
	  
	  
	  int csock = accept(sock, (SOCKADDR *)&csin,(socklen_t *)&sinsize); /*int accept(int sock, struct sockaddr *adresse, socklen_t *longueur);
																		extrait la première connection de la file d'attente des connexions en 
																		attente de socket et renvoi un file descripteur pour le socket si 
																		il est accepté*/
	  
	  if(csock == SOCKET_ERROR) /* socket_error défini dans .h = -1 */
	    {
	      perror("accept()");
	      continue;
	    }

	  if(read_client(csock, buffer) == -1)	  /* after connecting the client sends its name */

	    {
	      /* disconnected */

	      continue;
	    }

	  /* what is the new maximum fd ? */
	  max = csock > max ? csock : max;

	  FD_SET(csock, &rdfs);

	  Client c = { csock };
	  strncpy(c.name, buffer, BUF_SIZE - 1); /* fait sauter /0  */
	  clients[actual] = c;
	  strncat(buffer, " connected !", BUF_SIZE - strlen(buffer) - 1);
	  send_message_to_all_clients(clients, c, actual, buffer, 1);
			      
	  actual++;

	}
      else
	{
	  int i = 0;
	  for(i = 0; i < actual; i++)
	    {
	      /* a client is talking */
	      if(FD_ISSET(clients[i].sock, &rdfs))
		{
		  Client client = clients[i];
		  int c = read_client(clients[i].sock, buffer);
		  /* client disconnected */
		  if(c == 0)
		    {
		      closesocket(clients[i].sock);
		      remove_client(clients, i, &actual);
		      strncpy(buffer, client.name, BUF_SIZE - 1);
		      strncat(buffer, " disconnected !", BUF_SIZE - strlen(buffer) - 1);
		      send_message_to_all_clients(clients, client, actual, buffer, 1);
		    }
		  else
		    {
		      send_message_to_all_clients(clients, client, actual, buffer, 0);
		    }
		  break;
		}
	    }
	}
    }

  clear_clients(clients, actual);
  end_connection(sock);
}

static void clear_clients(Client *clients, int actual)
{
  int i = 0;
  for(i = 0; i < actual; i++)
    {
      closesocket(clients[i].sock);
    }
}

static void remove_client(Client *clients, int to_remove, int *actual)
{
  /* we remove the client in the array */
  memmove(clients + to_remove, clients + to_remove + 1, (*actual - to_remove - 1) * sizeof(Client));
  /* number client - 1 */
  (*actual)--;
}

static void send_message_to_all_clients(Client *clients, Client sender, int actual, const char *buffer, char from_server)
{
  int i = 0;
  char message[BUF_SIZE];
  message[0] = 0;
  for(i = 0; i < actual; i++)
    {
      /* we don't send message to the sender */
      if(sender.sock != clients[i].sock)
	{
	  if(from_server == 0)
	    {
	      strncpy(message, sender.name, BUF_SIZE - 1);
	      strncat(message, " : ", sizeof message - strlen(message) - 1);
	    }
	  strncat(message, buffer, sizeof message - strlen(message) - 1);
	  write_client(clients[i].sock, message);
	}
    }
}

static int init_connection(void)
{
  SOCKET sock = socket(AF_INET, SOCK_STREAM, 0); /* socket - Créer un point de communication proto:
													int socket(int domain, int type, int protocol);  
													-AF_INET defini le protocole IPV4
													-SOCK_STREAM : Support de dialogue garantissant l'intégrité, 
													 fournissant un flux de données binaires, et intégrant un mécanisme
													 pour les transmissions de données hors-bande.*/

  SOCKADDR_IN sin = { 0 }; 					 /* structure de sockaddr_in initialisée a 0
												{
												short    sin_family; 
												u_short    sin_port; 
												struct  in_addr  sin_addr; 
												char    sin_zero[8];
												};*/

  if(sock == INVALID_SOCKET)  /* invalid_socket def dans .h = -1 */
    {
      perror("socket()");  /*perror permet de sortir la dernière erreur rencontrée avec le texte ajouté ("") */
      exit(errno);
    }

  sin.sin_addr.s_addr = htonl(INADDR_ANY);  /* -hton Convert multi-byte integer types from host byte order to network byte order 
											   -INADDR_ANY. This allowed your program to work without 
												knowing the IP address of the machine it was running */
  sin.sin_port = htons(PORT);
  sin.sin_family = AF_INET; /*protocol IPV4 */

  if(bind(sock,(SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR) /* if quit violently, the program wont want to bind again and will need to be closed, recompiled and restart 
    {															-bind lie un socket avec une structure sockaddr.
																 proto : int bind(int sockfd, const struct sockaddr *addr,  socklen_t addrlen);*/
      perror("bind()");
      exit(errno);
    }

  if(listen(sock, MAX_CLIENTS) == SOCKET_ERROR) /* proto int listen(int s, int backlog);
												listen() marque la socket référencée par sockfd comme une socket passive,
												c'est-à-dire comme une socket qui sera utilisée pour accepter 
												les demandes de connexions entrantes en utilisant accept()*/
    {
      perror("listen()");
      exit(errno);
    }

  return sock;
}

static void end_connection(int sock)
{
  closesocket(sock);
}

static int read_client(SOCKET sock, char *buffer)
{
  int n = 0;

  if((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0)
    {
      perror("recv()");
      /* if recv error we disonnect the client */
      n = 0;
    }

  buffer[n] = 0;

  return n;
}

static void write_client(SOCKET sock, const char *buffer)
{
  if(send(sock, buffer, strlen(buffer), 0) < 0)
    {
      perror("send()");
      exit(errno);
    }
}

int main(int argc, char **argv)
{
  init();

  app();

  end();

  return EXIT_SUCCESS;
}
