#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	pid_t pid=fork();

	if(pid==0) // if Child Prosess
		puts("Hi, I am a child process");
	else
	{
		printf("child Process ID: %d \n", pid);
		sleep(30); // Sleep 30 sec.
	}

	if(pid==0)
		puts("End child process");
	else
		puts("End parent process");

	return 0;
}
