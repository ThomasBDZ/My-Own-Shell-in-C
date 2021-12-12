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

int executeCMD(char **cmdArgs, int i,int bg){

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
        }else if(strcmp(cmdArgs[0],"fg")==0){
            if(i == 1){
                int fils = wait(&status);
                printf("[%d->%d]\n",fils,status);
            }else if(i == 2){
                int fils = atoi(cmdArgs[1]);
                waitpid(fils,&status,0);
                printf("[%d->%d]\n",fils,status);

            }else{
                printf("Trop d'arguments");
                exit(1);
            }
        }
        
        else{
            pid_t fils = fork();
            if(fils == 0){
                execvp(cmdArgs[0],cmdArgs);
                perror("Error");
                exit(0);
            }else{
                if(bg == 1){
                    printf("[%d]\n",fils);
                }else{
                    wait(&status);
                }
            }

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

// On parcours les options s'il y en a
    if(argc > 1){
        int j = 1;
        while(argv[j] != NULL){
            // Cas ou l'option -e est détectée
            if(strcmp(argv[j],"-e") == 0){
                modeE = 1;

            }else{
            // Sinon on considère que c'est un fichier contenant des commandes
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

            if(strcmp(strtoken,";") != 0 && strcmp(strtoken,"&&") != 0 && strcmp(strtoken,"||") != 0 && strcmp(strtoken,"&") != 0 && strcmp(strtoken,">") != 0 && strcmp(strtoken,">>") != 0 && strcmp(strtoken,"|") != 0 && strcmp(strtoken,"<") != 0){
                args[i] = strtoken;
                i++;
                
            }else if(strcmp(strtoken,";") == 0){
                if(retour == 0){
                    retour = executeCMD(args,i,0);
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
                    retour = executeCMD(args,i,0);
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
                    retour = executeCMD(args,i,0);
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
            }else if(strcmp(strtoken,"&") == 0){
                
                if(retour == 0){
                    executeCMD(args,i,1);
                }
                int j =0;
                while(args[j+1] != NULL){
                    args[j] = NULL;
                    j++;
                }
                i = 0;
                retour = 0;

            }else if(strcmp(strtoken,">") == 0 || strcmp(strtoken,">>") == 0 || strcmp(strtoken,"<") == 0){
                
                if(retour == 0){
                    
                    char* fdname = strtok(NULL,sep);
                    if(fdname != NULL){
                        int fd = 0;

                        if(strcmp(strtoken,">") == 0){
                            fd = open(fdname,O_WRONLY|O_CREAT|O_TRUNC , 0666);
                        }else if(strcmp(strtoken,">>") == 0){
                            fd = open(fdname,O_WRONLY|O_CREAT|O_APPEND | O_EXCL, 0666);
                        }else{
                            fd = open(fdname,O_RDONLY, 0666);
                        }

                        pid_t f = fork();
                        if(f == 0){
                            if(strcmp(strtoken,">") == 0 || strcmp(strtoken,">>") == 0){
                                close(1);
                                dup2(fd,1);
                                
                            }else{
                                //close(0);
                                //dup2(0,fd);
                                
                            }
                            
                            retour = executeCMD(args,i,0);
                            
                            exit(retour);

                        }else{
                            int status;
                            close(fd);
                            waitpid(f,&status,0);
                            retour = status;
                        
                        }

                    }else{
                        printf("Veuillez entrer un nom de fichier valide \n");
                    }
                }

                int j =0;
                while(args[j+1] != NULL){
                    args[j] = NULL;
                    j++;
                }
                i = 0;

                
            }
                

            strtoken = strtok(NULL,sep);

        }

        // Si le mode avec erreur est activé et qu'on a un code retour différent de 0, on arrête le tesh 
        if(modeE == 1 && retour != 0){
            break;
        }
        // On execute la dernière commande entrée
        else if(retour == 0 && i>0){
            retour = executeCMD(args,i,0);
            // Si le mode erreur est activé, on arrête le tesh si le code retour est différent de 0
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