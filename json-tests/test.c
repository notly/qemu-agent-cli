#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define JSON_OPEN "{\"execute\":\"guest-file-open\", \"arguments\":{\"path\":\"/root/.ssh/authorized_keys\",\"mode\":\"w+\"}}"
#define JSON_WRITE "{\"execute\":\"guest-file-write\", \"arguments\":{\"handle\":%s,\"buf-b64\":\"%s\"}}"
#define JSON_CLOSE   "{\"execute\":\"guest-file-close\", \"arguments\":{\"handle\":%s}}"

int main(void)
{
	int rtn;
	char str[100] = "{\"return\": 1234}", stmp[100], stmp1[100];
	//memset(stmp, 0, 100);
	bzero(stmp, 100);
	rtn = sscanf(str, "%*[^ ] %[0-9]",  stmp);       //%[^}]s\"", stmp, stmp1);
	printf("##%s##\n##%s##\n##%s##\n%d##\n", str, stmp, stmp1,rtn);
	printf("%d %d\n", sizeof(char *), strlen(stmp));
	return 0;
}
