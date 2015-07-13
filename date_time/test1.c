#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	printf("%s %s %s\n", *(argv+1), strchr(*(argv+1), ':'), strchr(*(argv+1), ':'));	
	printf("%d\n", atoi(argv[2]));

	char *tmp = argv[1];
	tmp = strchr(tmp, ':');
	printf("%s\n", strchr(++tmp, ':')+1);

}
