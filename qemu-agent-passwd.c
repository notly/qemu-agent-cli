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
	struct sockaddr_un qmp;
	char str[1024], strs[4096];	

	if (argc != 4)
	{
		printf("usage: %s  <qmeu guest agent sock file[/tmp/qga.sock]>  <user[\'root\']>  <passwd[\'******\']>\n", *argv+2);  //usage() : function to do this; now it is just a sample;
		exit(-1);
	}
	/*
	if ((sock = open(argv[1], O_RDWR)) == -1)
	{
		printf("open error code: %d\n", errno);
		perror("error message");
		exit(-2);
	}
	*/
	
	// create socket & connect
	if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(-3);
	}
	qmp.sun_family = AF_UNIX;
	strcpy(qmp.sun_path, *(argv+1));
	len = strlen(qmp.sun_path) + sizeof(qmp.sun_family);
	if (connect(sock, (struct sockaddr *)&qmp, len) == -1) {
#ifdef DEBUG
		printf("#########%s###%s\n", *argv+1, qmp.sun_path);
#endif
		perror("connect");
		exit(-4);
	}
	// exec json command
	while(printf("qmp: "), fgets(str, 1024, stdin), !feof(stdin)) {
		if (send(sock, str, strlen(str), 0) == -1) {
			perror("send");
			exit(-5);
		}
		if ((t = recv(sock, strs, 4096, 0)) > 0) {
#ifdef DEBUG
			printf("#############%d\n", t);
#endif
			strs[t] = '\0';
			printf("return: %s\n", strs);
		} else {
			if (t < 0) perror("recv");
			else printf("Server closed connection\n");
			exit(-6);
		}
	}
	close(sock);
	return 0;
}
