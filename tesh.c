#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include "fcntl.h"
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char const *argv[])
{
    
    while(1 == 1){

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

