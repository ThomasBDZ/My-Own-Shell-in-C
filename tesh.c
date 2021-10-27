#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include "fcntl.h"
#include <string.h>


int main(int argc, char const *argv[])
{
    while(1 == 1){

        char commande[200];
        fgets(commande,200,stdin);
        const char *sep = " \n";
        char help[200];
        strcpy(help,commande);

        char * strtoken = strtok(commande,sep);
        int i =0 ;
        
        while(strtoken != NULL){
            strtoken = strtok(NULL,sep);
            i++;
        } 
        
        char **args = calloc(i,sizeof(char*));
        for(int j=0;j<i;j++){
            args[j] = calloc(1,sizeof(char)*20);
        }
        strtoken = strtok(help,sep);
        int k =0;
        
        while(strtoken != NULL){
            strcpy(args[k],strtoken);
            strtoken = strtok(NULL,sep);
            k++;
        } 

        
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
        

        for(int j=0;j<i;j++){
            free(args[j]);
        }
        free(args);

    }

    return 0;
}
