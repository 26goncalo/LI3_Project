#include "../include/client.h"
#include "../include/utilities.h"

#define COMMANDSIZE 100

int main(int argc, char* argv[]){
    //char command[COMMANDSIZE];
    if(argc != 4) {               // ./orchestrator output_folder parallel-tasks sched-policy
        if(argc == 2){          //  ./orchestrator -help
            if(my_strcmp(argv[1],"-help") == 0){
                //my_strcpy(command, "-help");
                printf("teste\n"); // neste momento usado para debug
                printf("Comando: %s\n",argv[1]);
                return 0;
            }
            else{
                printf("Segundo argumento errado\n");
                return -1;
            }
        }
        else{
            if(argc < 4)printf("Falta de argumentos\n");
            if(argc > 4) printf("Argumentos em excesso\n");
            return -1;
        }
    }

    char* output_folder = argv[1];
    int parallel_tasks = atoi(argv[2]);
    char* sched_policy = argv[3];      //Não sei como vai ser utilizado este argumento
    if(my_strcmp(sched_policy, "FIFO") == 0){
        if(mkfifo("server",0666) == -1){   // ligar o server
            perror("Erro ao criar pipeline");
            return -1;
        }
        if(mkdir(output_folder, 0777) == -1){
            perror("Erro ao criar pasta");
            return -1;
        }
        int fd = open("server", O_RDONLY);
        while(1){
            int bytes_read = 0;
            char buf[100];
            while((bytes_read = read(fd, buf, 100)) > 0){
                if(strncmp(buf, "end", bytes_read) == 0){
                    printf("\n");
                    close(fd);
                    return 0;
                }
                write(1, buf, bytes_read);
            }
        }
    }
    printf("Argumentos inválidos\n");
    return -1;
}