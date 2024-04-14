#include "../include/client.h"
#include "../include/utilities.h"

#define COMMANDSIZE 100

int main(int argc, char* argv[]){
    //char command[COMMANDSIZE];
    if(argc != 4) {               // ./orchestrator output_folder parallel-tasks sched-policy
        if(argc == 2){          //  ./orchestrator -help
            if(strcmp(argv[1],"-help") == 0){
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
    if(strcmp(sched_policy, "FIFO") == 0){
        if(mkfifo("client_to_server",0666) == -1){   // ligar o pipe com nome do cliente para o servidor
            perror("Erro ao criar pipe com nome");
            return -1;
        }
        if(mkfifo("server_to_client",0666) == -1){   // ligar o pipe com nome do servidor para o cliente
            perror("Erro ao criar pipe com nome");
            return -1;
        }
        if(mkdir(output_folder, 0777) == -1){
            perror("Erro ao criar pasta");
            return -1;
        }
        int client_to_server = open("client_to_server", O_RDONLY);
        while(1){
            int bytes_read = 0;
            char buf[1000];
            while((bytes_read = read(client_to_server, buf, 1000)) > 0){
                
                char* cmd = strdup_n(buf, bytes_read);  //copia para o cmd apenas a parte inicial do buffer que acabou de ser escrita pelo client
                //char* cmd = strdup(buf);
                char* args[300];
                int i = 0;
                while((args[i] = strsep(&cmd, " ")) != NULL) i++;
                if(i == 0) i++;
                args[i] = NULL;
                free(cmd);

                int server_to_client = open("server_to_client", O_WRONLY);
                if(i == 1){   // ./cliente status    ou      ./cliente end
                    if(strcmp(args[0], "status") == 0){
                        //...
                    }
                    else{ 
                        if(strcmp(args[0], "end") == 0){
                            close(client_to_server);
                            write(server_to_client, "Servidor encerrado\n", 19);
                            close(server_to_client);
                            return 0;
                        }
                        else{
                            write(server_to_client, "Comando inválido1\n", 18);
                            close(server_to_client);
                        }
                    }
                }
                else{
                    if(i > 3){  //  ./client execute time -u "prog-a [args]"
                        if((strcmp(args[0], "execute") == 0)  &&  ((strcmp(args[2], "-u") == 0) || (strcmp(args[2], "-p") == 0))){
                            int time = atoi(args[1]);
                            if(strcmp(args[2], "-u") == 0){  // "-u"
                                for(int j = 0; j<i; j++){
                                    write(server_to_client, args[i], sizeof(args[i]));
                                    if(j!=i-1) write(server_to_client, " ", 1);
                                }
                                write(server_to_client, "\n", 1);
                                close(server_to_client);
                            }
                            else{   // "-p"
                                // ...
                            }
                        }
                        else{
                            write(server_to_client, "Comando inválido2\n", 18);
                            close(server_to_client); 
                        }
                    }
                    else{
                        write(server_to_client, "Comando inválido3\n", 18);
                        close(server_to_client);  
                    }
                }
            }
            // pid_t pid;
            // if((pid = fork()) == -1){
            //     perror("Erro na criação de processo-filho");
            //     close(fd);
            //     return -1;
            // }
            // char* cmd = strdup_n(buf, bytes_read);
            // char* args[100];
            // int i = 0, status;
            // while((args[i] = strsep(&cmd, " ")) != NULL) i++;
            // args[i] = NULL;
            // if(pid == 0){ //processo-filho responsável pela execução do programa
            //     execvp(args[0], args);
            //     _exit(-1);
            //     perror("Erro no execvp");
            //     close(fd);
            //     return -1;
            // }
            // waitpid(pid, &status, 0);
            // if(WIFEXITED(status)) write(fd, &nr_task, sizeof(int));
        }
    }
    printf("Argumentos inválidos\n");
    return -1;
}