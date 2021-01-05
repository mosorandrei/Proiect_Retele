#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <signal.h>

extern int errno;

int port;
volatile int mark=0;
void trigger(int sig)
{
mark = 1;
}

int main( int argc , char *argv[])
{
int sd;
struct sockaddr_in server;
char nume[100];
char linie[100];
char coloana[100];
char viteza[100];
char request[100];
char accident[100];
char trimitlaserver[600];
char primescdelaserver[600];
if ( argc!= 3 )
  {
   printf("Sintaxa: %s <adresa_server> <port>\n",argv[0]);
  return -1;
  }
port=atoi(argv[2]);

if((sd = socket (AF_INET , SOCK_STREAM , 0 )) == -1 )
 {
 perror ("Eroare la socket().\n");
 return errno;
 }

server.sin_family =AF_INET;
server.sin_addr.s_addr = inet_addr(argv[1]);
server.sin_port = htons(port);


if( connect (sd , (struct sockaddr *) &server , sizeof ( struct sockaddr )) == -1 )
  {
   perror("[client]Eroare la connect().\n");
  return errno;
  }

bzero(nume,100);
printf ("[client] Numele meu este: ");
fflush(stdout);
read (0 , nume , 100 );
printf("\n");
bzero(linie,100);
bzero(coloana,100);
printf ("[client] Ma aflu pe strada cu coordonatele: \n");
printf("Linia:");
fflush(stdout);
read(0,linie,100);
printf("Coloana:");
fflush(stdout);
read(0,coloana,100);
printf("\n");
bzero(viteza,100);
printf("[client] Viteza cu care ma deplasez este: ");
fflush(stdout);
read(0,viteza,100);
printf("\n");
printf("[client] Doresc informatii suplimentare despre: " );
bzero(request,100);
fflush(stdout);
read(0,request,100);
printf("\n");
printf("[client] Accidente de raportat pe strada pe care va aflati ? ");
bzero(accident,100);
fflush(stdout);
read(0,accident,100);

bzero(trimitlaserver,600);
strcat(trimitlaserver,nume);
//strcat(trimitlaserver," ");
strcat(trimitlaserver,linie);
//strcat(trimitlaserver," ");
strcat(trimitlaserver,coloana);
//strcat(trimitlaserver," ");
strcat(trimitlaserver,viteza);
//strcat(trimitlaserver," ");
strcat(trimitlaserver,request);
//strcat(trimitlaserver," ");
strcat(trimitlaserver,accident);
//strcat(trimitlaserver," ");

if ( write(sd,trimitlaserver,600) <=0 )
{
perror ("[client]Eroare la write() spre server.\n");
return errno;
}

bzero(primescdelaserver,600);
if (read (sd,primescdelaserver,600) < 0)
 {
 perror("[client]Eroare la read() de la server.\n");
 return errno;
 }

printf("[server] %s\n",primescdelaserver);
char mesajexit[100];
bzero(mesajexit,100);
char tsv[100];
bzero(tsv,100);
char psv[100];
bzero(psv,100);
if(strstr(primescdelaserver,"Nu ati respectat")!=NULL)
{ close(sd);}
else
{
printf("Pentru a iesi din aplicatie , tastati la tastatura comanda Quit\n");
printf("Introduceti viteza actuala:\n");
while(read(0,mesajexit,100)>0 && 1)
{
if(strstr(mesajexit,"Quit")!=NULL) {break;}
if ( write(sd,mesajexit,sizeof(mesajexit))<=0 )
{
perror("[client] Eroare la write() spre server");
return errno;
}
signal(SIGALRM , trigger);
alarm(10);
 while(!mark) {}
 printf("Au trecut 10 secunde..\n");
 mark=0;
 printf("Introduceti viteza actuala:\n");
}
close(sd);
}
}