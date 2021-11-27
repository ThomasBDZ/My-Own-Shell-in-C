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

void afficherPrompt(int mode){
    if(mode == 0){
        char username[MAX_USERID_LENGTH];
        char hostname[MAX_USERID_LENGTH];
        char repcourant[MAX_CurrentDir_LENGTH];
        cuserid(username);
        /*getlogin_r(username, MAX_USERID_LENGTH);*/
        gethostname(hostname, MAX_HOSTNAMEID_LENGTH);
        getcwd(repcourant,MAX_CurrentDir_LENGTH);
        printf("%s@%s:%s$ ",username,hostname,repcourant);
        
    }
    
}

int main(int argc, char const *argv[])
{
    FILE* desc = stdin;
    
    int modeNI = 0;
    int modeE = 0;

    if(argc > 1){
        int j = 1;
        while(argv[j] != NULL){
            if(strcmp(argv[j],"-e") == 0){
                modeE = 1;

            }else{
                modeNI = 1;
                char* filename = argv[j];        
                desc = fopen(filename,"r");
            }
            j = j+1;
        }     
    }

    if(!isatty(0)){
        modeNI = 1;
    }

    char entree[1000];
    char commande[1000];

    afficherPrompt(modeNI);


    while(fgets(entree,1000,desc) != NULL){

        
        sscanf(entree,"%[^\n]",commande);
        
        const char *sep = " "; 
        char *args[20];
        
        char *strtoken = strtok(commande,sep);  
        int i =0;
        
        int retour = 0;
        while(strtoken != NULL){

            if(strcmp(strtoken,";") != 0 && strcmp(strtoken,"&&") != 0 && strcmp(strtoken,"||") != 0){
                args[i] = strtoken;
                i++;
                
            }else if(strcmp(strtoken,";") == 0){
                if(retour == 0){
                    retour = executeCMD(args,i);
                }
                int j =0;
                while(args[j+1] != NULL){
                    args[j] = NULL;
                    j++;
                }
                i = 0;

                if(modeE == 1 && retour !=0){
                    break;
                }else{
                    retour = 0;
                }

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

                if (modeE == 1 && retour != 0){
                    break;
                }

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
                    if(modeE == 1){
                        break;
                    }else{
                        retour = 0;
                    }
                }
            }


            strtoken = strtok(NULL,sep);

        } 
        if(modeE == 1 && retour != 0){
            break;
        }
        else if(retour == 0 ){
            retour = executeCMD(args,i);
            if(modeE == 1 && retour != 0 ){
                break;
            }
        }

        afficherPrompt(modeNI);
              
    }
    if(argc > 1){
        fclose(desc);
    }

    return 0;
}