#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
int drum[5][5]= { {90,90,90,90,90},{90,70,50,50,70},{90,70,50,50,70},{90,70,30,30,70},{90,90,90,90,90} };
char pretbenzina[]="4.46 lei/litru.\n";
char pretmotorina[]="5.04 lei/litru.\n";
char temperatura[]="Temperatura de astazi este de 4 grade Celsius.\n";
char sport[]="Lovitura pentru FCSB! Unul dintre fotbalistii lui Becali s-a accidentat si RATEAZA meciul cu UTA.(Autor: Sport.ro)\nDupa ce i-a recuperat pe Coman, Man, Tanase si Olaru, FCSB are un nou accidentat in lot. Sergiu Bus (28 ani) s-ar fi accidentat la antrenamente si nu va fi disponibil pentru meciul cu UTA Arad, anunta ProSport.\n";

void preiasofer(int client)
{printf("Am primit un client!..\n");
char mesajprimit[600];
char nume[100];
int linie=-1;
char sirlinie[100];
int coloana=-1;
char sircoloana[100];
int  viteza=-1;
char sirviteza[100];
char request[100];
char accident[100];
char strada[100];
char mesajdetrimis[4096];
int contor=0;
if ( client < 0 )
 {
 printf("[server]Eroare la accept().\n");
 return;
 }
bzero(mesajprimit,600);
bzero(nume,100);
bzero(sirlinie,100);
bzero(sircoloana,100);
bzero(sirviteza,100);
bzero(strada,100);
bzero(request,100);
bzero(accident,100);
bzero(mesajdetrimis,4096);


fflush (stdout);
if ( read (client , mesajprimit , 600 ) <= 0 )
 {
   printf("[server]Eroare la read() de la client.\n");
   close(client);
  return;
 }
//printf("%s\n",mesajprimit);
char * p = strtok(mesajprimit,"\n");
while(p!=NULL)
{ if (contor==0 ) {strcat(nume,p);}
  if (contor==1) { linie = atoi (p) ;}
  if (contor==2) { coloana = atoi(p) ;}
  if (contor==3) { viteza = atoi(p) ;}
  if(contor==4) { strcat(request,p); }
  if (contor==5) { strcat(accident,p);}
 contor++;
 p=strtok(NULL,"\n");
}
//itoa(linie,sirlinie,10);
snprintf(sirlinie,sizeof(sirlinie),"%d",linie);
//itoa(coloana,sircoloana,10);
snprintf(sircoloana,sizeof(sircoloana),"%d",coloana);
//itoa(viteza,sirviteza,10);
snprintf(sirviteza,sizeof(sirviteza),"%d",viteza);
if(viteza == 0 )
{
write(client,"Nu ati respectat protocolul : viteza",sizeof("Nu ati respectat protocolul : viteza"));
close(client);
return;
}
if(linie == 0 || coloana == 0 || linie > 4 || coloana > 4)
{
write(client,"Nu ati respectat protocolul : strada",sizeof("Nu ati respectat protocolul : starda"));
close(client);
return;
}
if (strstr(accident,"Da")==NULL && strstr(accident,"Nu")==NULL )
{
write(client,"Nu ati respectat protocolul : accident",sizeof("Nu ati respectat protocolul : accident"));
close(client);
return;
}
{
printf("Numele acestuia este : %s si se deplaseaza cu viteza de %s km/h..\n",nume,sirviteza);
//itoa(drum[linie][coloana],strada,10);
snprintf(strada,sizeof(strada),"%d",drum[linie][coloana]);
strcat(mesajdetrimis,"Drumuri bune, ");
strcat(mesajdetrimis,nume);
strcat(mesajdetrimis,"! ");
strcat(mesajdetrimis,"Pe strada cu coordonatele ");
strcat(mesajdetrimis,sirlinie);
strcat(mesajdetrimis," si ");
strcat(mesajdetrimis,sircoloana);
strcat(mesajdetrimis," se circula cu viteza de: ");
strcat(mesajdetrimis,strada);
strcat(mesajdetrimis," km/h.");
if(drum[linie][coloana]<viteza) {strcat(mesajdetrimis,"Atentie! Reduceti viteza! ");}
if(strstr(request,"sport")!=NULL) { strcat(mesajdetrimis,sport); }
if(strstr(request,"temperatura")!=NULL ) { strcat(mesajdetrimis,temperatura);}
if(strstr(request,"benzina")!=NULL ) {strcat(mesajdetrimis,"Pretul benzinei este de: ");strcat(mesajdetrimis,pretbenzina);}
if(strstr(request,"motorina")!=NULL) {strcat(mesajdetrimis,"Pretul motorinei este de: ");strcat(mesajdetrimis,pretmotorina);}
//printez continutul unui fisier strcat(mesajdetrimis,siraccidente);
int fd;
fd=open("accidente.txt",O_RDONLY);
if(fd==-1)
{
printf("[server]Eroare la deschiderea fisierului cu accidente..");
return;
}

char citire[4096];
bzero(citire,4096);
if ( -1==read(fd,citire,4096) )
{
printf("[server]Eroare la citirea din fisier..");
return;
}
if ( -1==close(fd))
{
printf("[server]Eroare la inchiderea fisierului fd ..");
return;
}

strcat(mesajdetrimis,citire);
 if(strstr(accident,"Da")!=NULL) { //updateaccidente(siraccidente,accident);
int od;
od=open("accidente.txt",O_WRONLY);
if(od==-1)
{
printf("[server]Eroare la deschiderea fisierului cu accidente..");
return;
}

if( -1 == lseek (od,0,SEEK_END) )
{
printf("[server]Eroare la lseek()..");
return;
}
char descris[100];
bzero(descris,100);
strcat(descris,"Atentie! Este un accident pe strada cu coordonatele : ");
strcat(descris,sirlinie);
strcat(descris," ");
strcat(descris,sircoloana);
strcat(descris," ! ");
if(-1== write(od,descris,strlen(descris) ) )
{
printf("[server]Eroare la write()..");
return;
}

if(-1==close(od) )
{
printf("Eroare la inchiderea fisierului od.. ");
return;
}


 }
//accidentu si logica lui
//printf("%s\n",nume);
//printf("%d\n",linie);
//printf("%d\n",coloana);
//printf("%d\n",viteza);
//printf("%s\n",request);
//printf("%s\n",accident);
if( write(client,mesajdetrimis,4096) <=0)
 {
 printf("[server]Eroare la write() catre client.\n");
 return;
 }
char mesajexit[100];
bzero(mesajexit,100);
while(read(client,mesajexit,sizeof(mesajexit))>0 && strcmp(mesajexit,"")!=0)
{
printf("Soferul %s se deplaseaza in momentul de fata cu viteza de %s",nume,mesajexit);
printf("km/h. \n");
bzero(mesajexit,sizeof(mesajexit));
}
}
printf("Soferul %s s-a deconectat .\n",nume);
close(client);
}
int main(int argc , char*argv[])
{
if(argc !=2 ) {
 printf("Sintaxa: %s <port>\n",argv[0]);
 return -1;
}
int PORT=atoi(argv[1]);
struct sockaddr_in server;
struct sockaddr_in from;
int sd;
//bzero(siraccidente,4096);
if ( (sd=socket (AF_INET , SOCK_STREAM , 0) ) == -1 )
 {
 perror("[server]Eroare la socket().\n");
 return errno;
 }
bzero(&server,sizeof(server));
bzero(&from , sizeof(from) );

server.sin_family = AF_INET;
server.sin_addr.s_addr = htonl (INADDR_ANY);
server.sin_port= htons (PORT);

if ( bind (sd, (struct sockaddr * ) &server,sizeof (struct sockaddr)) == -1 )
 {
 perror ("[server]Eroare la bind().\n");
 return errno;
 }
if (listen (sd,5) ==-1)
 {
 perror ("[server]Eroare la listen(). \n");
 return errno;
 }
printf("[server]Asteptam la portul %d...\n",PORT);

while (1)
 {
 int client;
 int length=sizeof(from);
 fflush(stdout);
 client=accept ( sd, (struct sockaddr * ) &from , &length);
 int pid=fork();
 if (pid==-1)
 {
 perror("[server]Eroare la fork().\n");
 return errno;
 }
 if (pid==0 )
 {
 preiasofer(client);
 exit(0);
 }
}
}