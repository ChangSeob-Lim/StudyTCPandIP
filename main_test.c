#include <stdio.h>

int main(int argc, char *argv[])
{
	
	printf("인자갯수 : %d\n", argc);
	printf("프로그램경로 : %s\n", argv[0]);

	for(int i=1; i<argc; i++)
	{
		printf("추가된 인자 : %s\n", argv[i]);
	}

	return 0;
}  

