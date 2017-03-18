#include <stdio.h>
#include <stdlib.h>
#include <string>

void change_msg( char * msg1){
    const char * s = "hiiiii";
    sprintf(msg1, "%s", s);
    printf("in change = %s\n",msg1);
}
void change_msg_ref(std::string & msg2)
{
    msg2 = "iiiiiiiih";
}

int main()
{
    char * msg = ( char *)malloc(sizeof(char) * 100) ;
    change_msg( msg);
    printf("in main = %s\n",msg);

    std::string str;
    change_msg_ref(str);
    printf("ref = %s\n",str.c_str());
    return 0;
}
