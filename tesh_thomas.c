#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include "fcntl.h"
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_USERID_LENGTH 32
#define MAX_HOSTNAMEID_LENGTH 256
#define MAX_CurrentDir_LENGTH 256

int executeCMD(char **cmdArgs, int i){

    int status;

    cmdArgs[i] = NULL;
    if(i <1 ){
            printf("Il manque au moins 1 argument\n");
            exit(1);
        }
        if(strcmp(cmdArgs[0],"cd")==0){
            
            status = chdir(cmdArgs[1]);
            if(status == -1){
            perror("Error");
            }
        }
        else if(fork() == 0){
            execvp(cmdArgs[0],cmdArgs);
            perror("Error");
            exit(0);
        }
        else{
           wait(&status);
           
        }
    
    return status;
}

void afficherPrompt(){
    char username[MAX_USERID_LENGTH];
    char hostname[MAX_USERID_LENGTH];
    char repcourant[MAX_CurrentDir_LENGTH];
    cuserid(username);
    /*getlogin_r(username, MAX_USERID_LENGTH);*/
    gethostname(hostname, MAX_HOSTNAMEID_LENGTH);
    getcwd(repcourant,MAX_CurrentDir_LENGTH);
    printf("%s@%s:%s$ ",username,hostname,repcourant);

}

int main(int argc, char const *argv[])
{
    
    char entree[200];
    char commande[200];

    afficherPrompt();

    while(1 == 1 && fgets(entree,200,stdin) != NULL){

        sscanf(entree,"%[^\n]",commande);
        
        const char *sep = " "; 
        char *args[20];
        
        char *strtoken = strtok(commande,sep);  
        int i =0;
        
        int retour = 0;
        while(strtoken != NULL){

            if(strcmp(strtoken,";") != 0 && strcmp(strtoken,"&&") != 0 && strcmp(strtoken,"||") && strcmp(strtoken,">") && strcmp(strtoken,">>")!= 0){
                args[i] = strtoken;
                i++;
                
            }else if(strcmp(strtoken,";") == 0){
                if(retour == 0){
                    executeCMD(args,i);
                }
                int j =0;
                while(args[j+1] != NULL){
                    args[j] = NULL;
                    j++;
                }
                i = 0;
                retour = 0;

            }else if(strcmp(strtoken,"&&") == 0){
                
                if(retour == 0){
                    retour = executeCMD(args,i);
                }
                int j =0;
                while(args[j+1] != NULL){
                    args[j] = NULL;
                    j++;
                }
                i = 0;
                /*if (retour != 0){
                    break;
                }*/

            }else if(strcmp(strtoken,"||") == 0){
                if(retour == 0){
                    retour = executeCMD(args,i);
                }
                int j =0;
                while(args[j+1] != NULL){
                    args[j] = NULL;
                    j++;
                }
                i = 0;
                
                if (retour == 0){
                    retour = -1;
                }else{
                    retour = 0;
                }

            }else if(strcmp(strtoken,">") == 0){
                if(retour == 0){

                    int filefd = open(argv[1], O_WRONLY|O_CREAT|O_TRUNC, 0666); //0777?
                    if (!fork()) {
                    close(1);//Close stdout
                    dup(filefd);
                    executeCMD(args,i);
                    } else {
                    close(filefd);
                    wait(NULL);
                    }
                }
                int j =0;
                while(args[j+1] != NULL){
                    args[j] = NULL;
                    j++;
                }
                i = 0;
                retour = 0;

            }else if(strcmp(strtoken,">>") == 0){
                if(retour == 0){

                    int filefd = open(argv[1], O_WRONLY|O_CREAT|O_TRUNC, 0666); //0777?
                    if (!fork()) {
                    close(1);//Close stdout
                    dup(filefd);
                    executeCMD(args,i);
                    } else {
                    close(filefd);
                    wait(NULL);
                    }
                }
                int j =0;
                while(args[j+1] != NULL){
                    args[j] = NULL;
                    j++;
                }
                i = 0;
                retour = 0;

            }
            /**else if(strcmp(strtoken,"|") == 0){

                int nb_pipe = 1;
                while(strtoken != NULL){
                    if(strcmp(strtoken,"|") == 0) {
                        nb_pipe++;
                    }
                }


                if(retour == 0){
                    executeCMD(args,i);
                }
                int j =0;
                while(args[j+1] != NULL){
                    args[j] = NULL;
                    j++;
                }
                i = 0;
                retour = 0;

            }**/


            strtoken = strtok(NULL,sep);

        } 
        if(retour == 0 ){
            executeCMD(args,i);
        }

        afficherPrompt();
              
    }   
    return 0;
}

/**int fd[2];
pipe(fd);
if (fork() == 0) {
    dup2(fd[1],1);
    close(fd[0]);
    close(fd[1]);
    execlp("prog1", "prog2", NULL);
}
if (fork() == 0) {
    dup2(fd[0],0);
    close(fd[0]);
    close(fd[1]);
    execlp("prog2","prog1", NULL);
}

wait(NULL);
wait(NULL);**/

