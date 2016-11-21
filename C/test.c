#include<stdio.h>
#include<time.h>
#include<sys/time.h>
int main()
{

	//struct timeval tv;
	time_t timep;
	struct tm *p;
	time(&timep);
	char str[200];
	strftime(str,200,"%y-%m-%d %H:%M:%S",localtime(&timep));
	printf("%s",str);
	/*
	p = gmtime(&timep);
	printf("%4d/%2d/%2d",(1900+p->tm_year),p->tm_mon+1,p->tm_mday);
	printf("%d:%d:%d\n",p->tm_hour,p->tm_min,p->tm_sec);
	*/
	return 0;

}

