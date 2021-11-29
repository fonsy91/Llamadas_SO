//Librerias 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <dirent.h>

#define NOMBRE_SIZE 20

/*
Se quiere desarrollar una aplicación que se encargue de ejecutar los archivos que se encuentren en una serie de di -
rectorios (todos los archivos serán ejecutables). Para ello, se deben seguir los siguientes pasos:
a) Escribir un programa que cree cuatro procesos, que denominaremos de forma lógica 0, 1, 2 y 3. Estos cuatro
procesos estarán comunicados entre ellos a través de un único pipe. Uno de los procesos (por ejemplo el 0)
ejecutará el código de una función que se denomina Ejecutar() y el resto (procesos 1, 2 y 3) ejecutarán
una función que se denomina Distribuir(). Los códigos de estas funciones se describen en los siguien-
tes apartados.

b) La función Distribuir() en el proceso i abrirá el directorio con nombre “directorio_i” y leerá las en-
tradas de este directorio (todas las entradas se refieren a nombres de archivos ejecutables). Cada vez que lea
una entrada enviará, utilizando el pipe creado en el apartado a, el nombre del archivo al proceso que está
ejecutando la función Ejecutar(). Cuando el proceso que está ejecutando la función Distribuir()
llega al fin del directorio acaba su ejecución. Escribir el código de la función Distribuir() para el pro-
ceso i.

c) El proceso que ejecuta la función Ejecutar() leerá del pipe que le comunica con el resto de procesos
nombres de archivos. Cada vez que lee un archivo lo ejecuta (recuerde que todos los archivos de los directo-
rios son ejecutables) esperando a que acabe su ejecución. Este proceso finaliza su trabajo cuando todos los
procesos que ejecutan la función Distribuir() han acabado. Escribir el código de la función Ejecu-
tar(). ¿Existe algún problema en la lectura que realiza este proceso del pipe?
*/

//Prototipos 
void Distribuir(int fd, int i);
void Ejecutar(int fd);

//Main 
int main(int argc, char const *argv[]){

    int fd[2];
    int j; 
    int pid; 

    if (pipe(fd) < 0){
        perror("Error en piep");
        exit(1); 
    }
    //for para crear los 4 procesos 
    for (j = 0; j < 4; j++){
        pid = fork(); 
        switch (pid){
        case -1:
            perror("Error en el fork()");
            close(fd[0]);
            close(fd[1]);
            exit(1);
        case 0:
            if (j == 0){
                close(fd[1]);
                Ejecutar(fd[0]); 
                close(fd[0]);
                return 0; 
            }else{
                close(fd[0]);
                Distribuir(fd[1],j);
                close(fd[1]);
                return 0; 
            }
            
        default:
            break;
        }
    }
    //El proceso main cierra los descriptores del pipe y acaba su ejecucion 
    close(fd[0]);
    close(fd[1]);
    return 0; 
}

//Desarrolo de funciones 
void Ejecutar(int fd){
    int pid;
    char nombre[NOMBRE_SIZE];
    while (read(fd,nombre,NOMBRE_SIZE) > 0){
        pid = fork();
        switch (pid){
        case -1:
            perror("Error al crear el fork()");
            break; 
        case 0: 
            close(fd); 
            //ejecutamos con execlp
            execlp(nombre,nombre,NULL); 
            perror("Error en el execlp");    
            exit(1); 
        default:
            //espera la terminacion del proceso creado 
            if (wait(NULL) < 0){
                perror("Error en el wait");
            }
            
        }
    }
    return;
}

void Distribuir(int fd, int i){
    DIR *dirp; 
    struct dirent *dp; 
    dirp = opendir("directorio_i");
    if (dirp == NULL){
        perror("Error opendir"); 
        return; 
    }
    while ((dp = readdir(dirp)) != NULL){
        //Se saltan las entradas "." y ".."
        if ((strcmp(dp->d_name,".")) == 0 || (strcmp(dp->d_name,"..")) == 0){
            continue; 
        }
        //Se escribe el nombre en el pipe. La escritura es atomica  
        if (write(fd,dp->d_name,NOMBRE_SIZE) < NOMBRE_SIZE){
            perror("Error en el write");
            closedir(dirp);
            return; 
        }
    }
    return; 
}