#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

//#define DEBUG

int main(int argc, char **argv)
{
	int sock, t, len;	
	struct sockaddr_un qga;
	char str[1024], strs[4096];	
	char *tmp;	

	if (argc != 2)
	{
		printf("usage: %s <qmeu guest agent sock file>[/tmp/qga.sock] \n", *argv+2);  //usage() : function to do this; now it is just a sample;
		exit(-1);
	}
	
	// create socket & connect
	if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(-3);
	}
	qga.sun_family = AF_UNIX;
	strcpy(qga.sun_path, *(argv+1));
	len = strlen(qga.sun_path) + sizeof(qga.sun_family);
	if (connect(sock, (struct sockaddr *)&qga, len) == -1) {
#ifdef DEBUG
		printf("#########%s###%s\n", *(argv+1), qga.sun_path);
#endif
		perror("connect");
		exit(-4);
	}
	// exec json command
	
	while(printf("qga: "), fgets(str, 1024, stdin), !feof(stdin)) {
		
#ifdef DEBUG
			printf("#############%s###%d\n", str, (int) strlen(str));
#endif
		if(strlen(str) == 1 || strchr(str, '{') == NULL || strrchr(str, '}') == NULL )  {
			if(!strncmp(str, "clear", 5))
			{ write(1, "\33[H\33[2J", 7); continue; }
			if(!(strncmp(str, "quit", 4) && strncmp(str, "exit", 4)))
			{ break;}
			printf("\nreturn:\t plz input the json code!! \nlike this: \n{\"execute\": \"guest-info\"}\n");
			continue;
		}
		if ( send(sock, str, strlen(str), 0) == -1) {
			perror("send");
			exit(-5);
		}
		if ((t = recv(sock, strs, 4096, 0)) > 0) {
#ifdef DEBUG
			printf("#############%d;\t%d\n", t, feof(stdin));
#endif
			strs[t] = '\0';
			printf("\nreturn:\t%s\n", strs);
		} else {
			if (t < 0) perror("recv");
			else printf("Server closed connection\n");
			exit(-6);
		}
	}
	close(sock);
	return 0;
}
