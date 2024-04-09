#include <client.h>

int main(){
    if(mkfifo("fifo",0666) == -1) perror("Erro ao criar pipeline");
    return 0;
}