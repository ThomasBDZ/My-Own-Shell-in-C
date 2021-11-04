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

    cmdArgs[i] = NULL;
    if(i <1 ){
            printf("Il manque au moins 1 argument\n");
            exit(1);
        }
        if(strcmp(cmdArgs[0],"cd")==0){
            
            int r = chdir(cmdArgs[1]);
            if(r == -1){
            perror("Error");
            }
        }
        else if(fork() == 0){
            execvp(cmdArgs[0],cmdArgs);
            perror("Error");
            exit(0);
        }
        else{
           wait(NULL);
        }
    
    return 0;
}

void afficherPrompt(){
    char username[MAX_USERID_LENGTH];
    char hostname[MAX_USERID_LENGTH];
    char repcourant[MAX_CurrentDir_LENGTH];
    cuserid(username);
    /*getlogin_r(username, MAX_USERID_LENGTH);*/
    gethostname(hostname, MAX_HOSTNAMEID_LENGTH);
    getcwd(repcourant,MAX_CurrentDir_LENGTH);
    printf("\n%s@%s:%s$ ",username,hostname,repcourant);

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
        int r;

        while(strtoken != NULL){

            if(strcmp(strtoken,";") != 0){
                args[i] = strtoken;
                i++;
                
            }else if(strcmp(strtoken,";") == 0){
                
                r = executeCMD(args,i);
                int j =0;
                while(args[j+1] != NULL){
                    args[j] = NULL;
                    j++;
                }
                i = 0;

            }
            strtoken = strtok(NULL,sep);

        } 
        
        executeCMD(args,i);

        afficherPrompt();
              
    }   
    return 0;
}