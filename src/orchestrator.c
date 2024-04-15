#include "../include/client.h"
#include "../include/utilities.h"

#define COMMANDSIZE 100

int main(int argc, char* argv[]){
    //char command[COMMANDSIZE];
    if(argc != 4) {               // ./orchestrator output_folder parallel-tasks sched-policy
        if(argc == 2){          //  ./orchestrator -help
            if(strcmp(argv[1],"-help") == 0){
                printf("• Executar o servidor:\n\n"
                        "\t$ ./orchestrator output_folder parallel-tasks sched-policy\n\n"
                        "\tArgumentos:\n\n"
                        "\t \t1. output-folder: pasta onde são guardados os ficheiros com o output de tarefas executadas.\n"
                        "\t \t2. parallel-tasks: número de tarefas que podem ser executadas em paralelo.\n"
                        "\t \t3. sched-policy: identificador da política de escalonamento, caso o servidor suporte várias políticas.\n");
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
        mkdir(output_folder, 0777);
        int client_to_server = open("client_to_server", O_RDONLY);
        int nr_task = 1;
        char tasks_file[60];
        sprintf(tasks_file, "./%s/ALL_TASKS.txt", output_folder);
        int tasks = open(tasks_file, O_RDWR | O_CREAT | O_APPEND, 0666);
        while(1){   
            int bytes_read = 0;
            char buf[1000];
            char* string = NULL;
            while((bytes_read = read(client_to_server, buf, 1000)) > 0){

                struct timeval start_time;
                gettimeofday(&start_time, NULL);

                char* cmd = strdup_n(buf, bytes_read);  //copia para o cmd apenas a parte inicial do buffer que acabou de ser escrita pelo client
                char* args[300];
                int i = 0;
                while((string = strsep(&cmd, " ")) != NULL){
                    args[i] = string;
                    i++;
                }
                free(cmd);           

                int server_to_client = open("server_to_client", O_WRONLY);            
                if(i == 1){   // ./cliente status    ou      ./cliente end
                    if(strcmp(args[0], "status") == 0){
                        //...
                    }
                    else{ 
                        if(strcmp(args[0], "end") == 0){
                            close(client_to_server);
                            close(server_to_client);
                            unlink("client_to_server");
                            unlink("server_to_client");
                            return 0;
                        }
                        else{
                            write(server_to_client, "Comando inválido\n", 18);
                            close(server_to_client);
                        }
                    }
                }
                else{
                    if(i > 3){  //  ./client execute time -u "prog-a [args]"
                        if((strcmp(args[0], "execute") == 0)  &&  ((strcmp(args[2], "-u") == 0) || (strcmp(args[2], "-p") == 0))){
                            
                            char* args_prog[20][20];   //  args_prog[número do programa][número do argumento]
                            int NR_P = 0, nr_p = 0, nr_arg = 0;
                            for(int j = 3; j<i; j++){
                                if(strcmp(args[j], "|") == 0){
                                    args_prog[nr_p][nr_arg] = NULL;
                                    nr_p++;
                                    NR_P++;
                                    nr_arg = 0;
                                }
                                else{
                                    args_prog[nr_p][nr_arg] = strdup (args[j]);
                                    nr_arg++;
                                }
                            }
                            args_prog[nr_p][nr_arg] = NULL;
                            nr_p++;
                            NR_P++;
                            nr_arg = 0;
                            
                            char message[20];
                            sprintf(message, "\nTASK %d Received\n\n", nr_task);
                            write(server_to_client, message, strlen(message));
                            
                            char taskX_file[60];
                            sprintf(taskX_file, "./%s/TASK%d.txt", output_folder, nr_task);
                            int taskX = open(taskX_file, O_RDWR | O_CREAT, 0666);

                            if(strcmp(args[2], "-u") == 0){  // "-u"
                                int status;
                                int pid = fork();
                                if(pid == -1){
                                    write(server_to_client, "Erro na criação do processo-filho\n", 37);
                                    close(server_to_client);
                                }

                                if(pid == 0){ //processo-filho
                                    dup2(taskX, 1);
                                    dup2(taskX, STDERR_FILENO);
                                    execvp(args_prog[0][0], args_prog[0]);
                                    char error[50];
                                    sprintf(error, "Erro no programa '%s'", args_prog[0][0]);
                                    perror(error);
                                    _exit(255);
                                }
                                wait(&status);

                                // Obter o tempo de fim
                                struct timeval end_time;
                                gettimeofday(&end_time, NULL);
                                long microseconds = (end_time.tv_sec - start_time.tv_sec) * 1000000 + (end_time.tv_usec - start_time.tv_usec);

                                char task_completed[50];
                                sprintf(task_completed, "TASK %d was completed in %ld microseconds\n", nr_task, microseconds);
                                write(tasks, task_completed, strlen(task_completed));
                                close(server_to_client);
                            }

                            else{   // "-p"
                                // ...
                            }

                            for(int nr_p = 0; nr_p<NR_P ; nr_p++){
                                for(int nr_arg = 0; nr_arg<20 && args_prog[nr_p][nr_arg]!=NULL; nr_arg++){
                                    free(args_prog[nr_p][nr_arg]);
                                }
                            }
                            nr_task++;
                            close(taskX);
                        }
                        else{
                            write(server_to_client, "Comando inválido\n", 18);
                            close(server_to_client); 
                        }
                    }
                    else{
                        write(server_to_client, "Comando inválido\n", 18);
                        close(server_to_client);  
                    }
                }
            }
        }
        close(tasks);
    }
    printf("Argumentos inválidos\n");
    return -1;
}