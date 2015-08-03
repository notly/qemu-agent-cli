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

/*
#include <json.h>
#include <b64/cencode.h>
*/

//#define DEBUG

#define B64_SYNTAX_ERROR        1
#define B64_FILE_ERROR          2
#define B64_FILE_IO_ERROR       3
#define B64_ERROR_OUT_CLOSE     4
#define B64_LINE_SIZE_TO_MIN    5
#define B64_SYNTAX_TOOMANYARGS  6

/*
** b64_message
**
** Gather text messages in one place.
**
*/
#define B64_MAX_MESSAGES 7
static char *b64_msgs[ B64_MAX_MESSAGES ] = {
            "b64:000:Invalid Message Code.",
            "b64:001:Syntax Error -- check help (-h) for usage.",
            "b64:002:File Error Opening/Creating Files.",
            "b64:003:File I/O Error -- Note: output file not removed.",
            "b64:004:Error on output file close.",
            "b64:005:linesize set to minimum.",
            "b64:006:Syntax: Too many arguments."
};
#define b64_message( ec ) ((ec > 0 && ec < B64_MAX_MESSAGES ) ? b64_msgs[ ec ] : b64_msgs[ 0 ])


static void encodeblock(unsigned char *in, unsigned char *out, int len);
static int encode(char *instr, char *outstr);

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
/*
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
*
	char *pas = malloc(sizeof(char) * 100);
	char *base = pas;
	memset(pas, 0, 100);

	char *pass = argv[3];
	while(*pass)
	{
		*pas++ = base64_decode_value(*pass++);
	}
	printf("code::::%s\n", base);

	free(base);
*/
	char *pass = malloc(sizeof(char)*100);
	char command[1024] = "{ \"execute\": \"guest-set-user-password\", \"arguments\": { \"crypted\": false,  \"username\": \"" ; // root\", \"password\": \""; //\" }}";
	int command_len = strlen(command);

	memset(pass, 0, 100);
	encode(*(argv+3), pass);
	//	encodeblock(base, pass, strlen(base));
	//	tmp += 3; pass += 3;
	//printf("decode:\t##%s##\n", pass);
	 //  exec change the password;

	strncat(command, *(argv + 2), strlen(*(argv+2)));
	strcat(command, "\",  \"password\": \"");
	strncat(command, pass, strlen(pass));

	strcat(command, "\" }}");

	//  send command;
	if (send(sock, command, strlen(command), 0) == -1) {
		perror("send");
		exit(-5);
	}
	printf("exec json command: %s\n", command);
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

	free(pass);
	close(sock);
	return 0;
}


static const char cb64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char cd64[]="|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`abcdefghijklmnopq";
/*
** encodeblock
**
** encode 3 8-bit binary bytes as 4 '6-bit' characters
*/
static void encodeblock( unsigned char *in, unsigned char *out, int len )
{
    out[0] = (unsigned char) cb64[ (int)(in[0] >> 2) ];
    out[1] = (unsigned char) cb64[ (int)(((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4)) ];
    out[2] = (unsigned char) (len > 1 ? cb64[ (int)(((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6)) ] : '=');
    out[3] = (unsigned char) (len > 2 ? cb64[ (int)(in[2] & 0x3f) ] : '=');
}

/*
** encode
**
** base64 encode a stream adding padding and line breaks as per spec.
*/

static int encode(char *ins, char *outs)
{
	unsigned char in[3];
	unsigned char out[4];
	int i = 0;
	char *tmp = outs;

	*in = (unsigned char) 0;
	*out = (unsigned char) 0;
	while(*ins) {
			i = i % 3;
			in[i++] = (unsigned char) *ins++; in[i] = '\0';
			//putchar(in[i]);
		if(i == 3 || !*ins)
		{
			encodeblock(in, out, i);
		//	printf("\nin: %s;  out: %s\n", in, out);
			strncpy(tmp, out, strlen(out)); tmp += strlen(out);
		}
	}
}

