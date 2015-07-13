#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{	
	if(argc ==1) {
	time_t t1, t2;

	time(&t1);
	t2 = time(NULL);
	printf("%ld\t%ld\n", t1, t2);
	printf("%s\n", asctime(localtime(&t1)));
	printf("%s\n", ctime(&t1));
	return 0;
}
	//printf("%s\n", ctime((time_t *)argv[1]));
	time_t t;
	t = atol(argv[1]);
	printf("%s\n", asctime(localtime(&t)));
	printf("%s\n", ctime(&t));
}
