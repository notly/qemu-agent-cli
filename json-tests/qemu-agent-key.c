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

/* for jansson support
#include <jansson.h>
*/

#include "qemu-b64.h"

#define BUFFER 1024

#define JSON_OPEN "{\"execute\":\"guest-file-open\", \"arguments\":{\"path\":\"/root/.ssh/authorized_keys\",\"mode\":\"a+\"}}"
#define JSON_WRITE "{\"execute\":\"guest-file-write\", \"arguments\":{\"handle\":%s,\"buf-b64\":\"%s\"}}"
#define JSON_CLOSE   "{\"execute\":\"guest-file-close\", \"arguments\":{\"handle\":%s}}"

#define JSON_READ  "{\"execute\":\"guest-file-read\", \"arguments\":{\"handle\":%s,\"count\":1024}}"
#define JSON_OK  "{\"return\": {}}"
#define JSON_RTN " {"return": %d}"

//#define DEBUG

int main(int argc, char **argv)
{
	int sock, t, len;	
	struct sockaddr_un qga;
	char str[1024], strs[4096];	
	char *tmp, nums[10];	
	
	char key_read[BUFFER],  key_json_read[BUFFER], key_json_write[BUFFER], key_json_close[BUFFER];	
	int fd, rtn;	

	if (argc != 3)
	{
		printf("usage: %s <qmeu guest agent sock file>[/tmp/qga.sock]  <public key file[~/.ssh/id_rsa.pub]>\n", *argv+2);  //usage() : function to do this; now it is just a sample;
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
	
/* no use for this part;
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
*/

	if((fd = open(*(argv+2), O_RDONLY)) < 0) {
		perror("open");
		exit(-1);
	}
	if ((rtn = read(fd, key_read, BUFFER)) == -1) {
		perror("read");
		return (-1);
	}

//	*(key_read + strlen(key_read) -1) = '\0';   // parse the '\n' eof
	encode(key_read, key_json_read);

#ifdef DEBUG
	printf("##%s##\n", key_read);
#endif
	printf("##key_read:%s##\nkey_json_read:%s##\n", key_read, key_json_read);

	// file read
	if ( send(sock, JSON_OPEN, strlen(JSON_OPEN), 0) == -1) {
		perror("send");
		exit(-5);
	}
	if ((t = recv(sock, strs, 4096, 0)) > 0) {
#ifdef DEBUG
		printf("#############%d;\t%d\n", t, feof(stdin));
#endif
		strs[t] = '\0';
		printf("\nreturn:\t%s\n", strs);
		if (rtn = sscanf(strs, "%*[^0-9]%[0-9]", nums) < 0)
		{ perror("not insert public key ok"); exit(-1); }
	} else {
		if (t < 0) perror("recv");
		else printf("Server closed connection\n");
		exit(-6);
	}

	printf("##file descriptor:%s##\n", nums);
	if ((rtn = sprintf(key_json_write, JSON_WRITE, nums, key_json_read)) != strlen(key_json_write) ) {
		printf("rtn:%d\n key_json_write:%ld\nnums:%ld\n", rtn, strlen(key_json_write), strlen(nums));
		exit(-1);
	}
	if ((rtn = sprintf(key_json_close, JSON_CLOSE, nums)) != strlen(key_json_close)) {
		printf("##rtn:%d\n##%ld##\n", rtn, strlen(key_json_close));
		exit(-1);
	}
	
	// file write
	if ( send(sock, key_json_write, strlen(key_json_write), 0) == -1) {
		perror("send");
		exit(-5);
	}
	if ((t = recv(sock, strs, 4096, 0)) > 0) {
#ifdef DEBUG
		printf("#############%d;\t%d\n", t, feof(stdin));
#endif
		strs[t] = '\0';
		printf("\nexec write:%s##\nreturn:\t%s\n", key_json_write, strs);
		if (!strcmp(strs, JSON_OK))
		{  perror("file write not ok"); exit(-3); }
	} else {
		if (t < 0) perror("recv");
		else printf("Server closed connection\n");
		exit(-6);
	}
	
	// file close
	if ( send(sock, key_json_close, strlen(key_json_close), 0) == -1) {
		perror("send");
		exit(-5);
	}
	if ((t = recv(sock, strs, 4096, 0)) > 0) {
#ifdef DEBUG
		printf("#############%d;\t%d\n", t, feof(stdin));
#endif
		strs[t] = '\0';
		printf("\nreturn:\t%s\n", strs);
		if (!strcmp(strs, JSON_OK))
		{  perror("file close not ok"); exit(-3); }
	} else {
		if (t < 0) perror("recv");
		else printf("Server closed connection\n");
		exit(-6);
	}
	


	close(fd);
	close(sock);

}
