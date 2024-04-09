#include "../include/client.h"
#include "../include/utilities.h"

#define COMMANDSIZE 100

int main(int argc, char* argv[]){
    char command[COMMANDSIZE];
    // execute time -u "prog-a [args]"
    if(argc < 2) {
        perror("Falta de argumentos");
        return -1;
    }

    if(my_strcmp(argv[1],"-") == 0){
        if (my_strcmp(argv[2],"help") == 0){
            my_strcpy(command, "ajuda");
            printf("teste\n"); // neste momento usado para debug
        } else {
            printf("Invalid argument\n");
        }
    } else {

    }

    return 0;
}