#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include "fcntl.h"
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


#define MAX_USERID_LENGTH 32
#define MAX_HOSTNAMEID_LENGTH 256
#define MAX_CurrentDir_LENGTH 256


int main(int argc, char const *argv[])
{
    
    char username[MAX_USERID_LENGTH];
    char hostname[MAX_USERID_LENGTH];
    char repcourant[MAX_CurrentDir_LENGTH];
    cuserid(username);
    /*getlogin_r(username, MAX_USERID_LENGTH);*/
    gethostname(hostname, MAX_HOSTNAMEID_LENGTH);
    getcwd(repcourant,MAX_CurrentDir_LENGTH);



    while(1){
        printf("\n%s@%s:%s$",username,hostname,repcourant);
        char commande[200];
        fgets(commande,200,stdin);
        const char *sep = " \n"; 
        char *args[20];
        
        char *strtoken = strtok(commande,sep);
        int i =0;
        
        while(strtoken != NULL){
            args[i] = strtoken;
            strtoken = strtok(NULL,sep);
            i++;
        } 
        
        args[i] = NULL;
        
        if(i <1 ){
            printf("Il manque au moins 1 argument\n");
            exit(1);
        }
        if(fork() == 0){
            execvp(args[0],args);
            perror("Erreur");
            exit(0);
        }
        else{
           wait(NULL);
        }
              
    }
    
    return 0;
}

