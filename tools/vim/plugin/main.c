#include <stdio.h>
#include <stdlib.h>
 
int main(void)
{
	FILE *fp;
	char buf[1024] = { 0 };
 
 
	if ((fp = popen("date +%Y-%m-%d-%H-%M-%S", "r")) == NULL)
	{
		perror("Fail to popen\n");
		exit(1);
	}
 
	while (fgets(buf, 200, fp) != NULL)
	{
		printf("%s\n",buf);
	}
	pclose(fp);
	return EXIT_SUCCESS;
}

static int test()
{
	return 1;
}
