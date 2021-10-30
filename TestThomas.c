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
#define MAX_COMMAND_LENGTH 200
#define MAX_NB_COMMAND 5


int main(int argc, char const *argv[])
{

    char username[MAX_USERID_LENGTH];
    char hostname[MAX_USERID_LENGTH];
    char repcourant[MAX_CurrentDir_LENGTH];
    cuserid(username);
    /*getlogin_r(username, MAX_USERID_LENGTH);*/
    gethostname(hostname, MAX_HOSTNAMEID_LENGTH);
    getcwd(repcourant,MAX_CurrentDir_LENGTH);

    char entree[MAX_COMMAND_LENGTH];
    char commande[MAX_COMMAND_LENGTH];

    printf("\n%s@%s:%s$",username,hostname,repcourant);    

    while(1 && fgets(entree,MAX_COMMAND_LENGTH,stdin) != NULL){

        sscanf(entree,"%[^\n]",commande);
        //fgets(commande,200,stdin);
        const char *sep = " "; 
        char *args[20];
        int numero_cmd = 0;
        
        char *strtoken = strtok(commande,sep);  
        

        char ** buff = NULL;
        int i = 0, n = MAX_NB_COMMAND ;

        buff = /*(char **)*/ malloc(n * sizeof(*buff) ); //On alloue MAX n lignes pour contenir max n commandes
            
            if (buff != NULL)
            {
                for (i=0; i<n; i++) 
                {
                    buff[i]= NULL;
                }
                for ( i = 0 ; i < n ; i++ ) 
                { 
                    //buff[i] = ( char * ) malloc(sizeof(**buff)*MAX_COMMAND_LENGTH) ; 
                    if ( buff[i] != NULL )
                    {
                        int j = 0;
                        char *s;

                        while((strtoken != NULL) || (strcmp(args[j],";" ))){
                            //printf("%s\n\n",strtoken);
                            args[j] = strtoken;
                            strtoken = strtok(NULL,sep);
                            j++;
                        } 
                        numero_cmd++;
                        args[j] = NULL;
                        strcpy(s,args);
                        buff[i] = strdup(s);
                    }

                    if (buff[i] == NULL)
                        {
                            perror ("erreur d'allocation");
                            return EXIT_FAILURE;
                        }
                }
            }
        
/* lecture du tableau */
    for (i = 0; i < n; i++)
        {
        if (buff[i] != NULL)
            {
                if(numero_cmd <1 )
                    {
                    printf("Il manque au moins 1 argument\n");
                    exit(1);
                    }
                if(fork() == 0)
                {
                    execvp(&buff[i][0],&buff[i]);
                    perror("Erreur");
                    exit(0);
                }
                else
                {
                    wait(NULL);
                }
            }
        }
        
            //libération des chaines de caractères et du tableau de pointeurs
    for (i = 0; i < n; i++)
        {
         if (buff[i] != NULL)
            free (buff[i]);
        } 
    if (buff != NULL) free (buff);
   
    else
    {
      perror ("erreur d'allocation");
      return EXIT_FAILURE;
    }
    }
return 0;

}

