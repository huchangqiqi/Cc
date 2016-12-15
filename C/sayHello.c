#include<stdio.h>
#include<string.h>
char *sayH(char * name){
	char d[100] = "hello";
	return strcat(d,name);
}
int main()
{
	printf("%s",sayH("hu"));
	return 0;
}
