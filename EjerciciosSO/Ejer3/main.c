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
#include <errno.h>
//indices de descriptores de archivos 
#define READ_END 0
#define WRITE_END 1


//Prototipos 
void pruebadup2();
void pipesProcesos();

//Main 
int main(int argc, char const *argv[]){

    //pruebadup2();
    pipesProcesos();

    return 0; 
}

//Desarrolo de funciones 

/*
https://www.youtube.com/watch?v=8LdQ09Ep9RY

Antes haremos un ejercicio de practica con 3 procesos (ls,grep y wc) usando 
dos pipes, fork(), dup2(), execlp(). EN terminal seria: ls -l | grep u | wc -l
esto realiza lo siguiente: 
    1. ls -l (lista contenidos)
    2. grep u (busca la letra "u")
    3. wc -l (muestra el numero de "nuevas lineas", contar directorio,ficheros)

Hay que saber que es la funcion dup2():
La función dup2 crea una copia del descriptor de fichero dado y le asigna un nuevo entero. 
dup2 toma como primer parámetro un descriptor de fichero antiguo a clonar y el segundo parámetro 
es el entero de un nuevo descriptor de fichero. Como resultado, ambos descriptores de archivo 
apuntan al mismo archivo y pueden usarse indistintamente. Tenga en cuenta que si el usuario 
especifica un entero utilizado actualmente por el archivo abierto como segundo parámetro, éste 
se cerrará y se reutilizará como el descriptor de archivo clonado.
*/

void pruebadup2(){
    int fd = open("tmp.txt",O_WRONLY | O_APPEND);
    printf("tmp.txt fd = %d\n",fd);
    //cogemos el descriptor fd (3) y lo clona con 121 ahora los dos apuntan al archivo 
    dup2(fd,121); 
    //con el descriptor clonado escribimos la frase 
    dprintf(121,"Este string va a ser escrito en tmp.txt\n");
    exit(EXIT_SUCCESS);
}

/*
          ------------
fd[1]-->  ----pipe----  --> fd[0]                           
          ------------

fd[0] = apunta al final de la lectura de la tuberia 
fd[1] = apunta al finañ de la escritura de la tuberia 

*/

void pipesProcesos(){
    //descriptores para ambos pipes 
    int fd1[2], fd2[2]; 
    int status,pid; 
    pipe(fd1); 
    pid = fork(); 
    if (pid == -1){
        perror("Error en el fork() uno.");
    }else if(pid == 0){
        //estamos en el hijo
        close(fd1[READ_END]);
        //STDOUT_FILENO es un duplicado de fd1[WRITE_END]
        dup2(fd1[WRITE_END],STDOUT_FILENO);
        close(fd1[WRITE_END]); 
        //ejecuto la funcion ls 
        execlp("/bin/ls","ls","-l",NULL);
    }else{
        //estamos en el padre 
        close(fd1[WRITE_END]);
        pipe(fd2);  //comunica grep con wc
        pid=fork();
        if (pid == -1){
            perror("Error en el segundo fork()");
        }else if(pid == 0){
            //segundo hijo
            //cerramos el extremo no necesario en ese hijo 
            close(fd2[READ_END]);
            dup2(fd1[READ_END],STDIN_FILENO);
            close(fd1[READ_END]);
            dup2(fd2[WRITE_END],STDOUT_FILENO);
            close(fd2[WRITE_END]);
            execlp("/bin/grep","grep","u",NULL); 
        }else{
            //segundo padre 
            close(fd1[READ_END]);
            close(fd2[WRITE_END]);
            pid = fork(); 
            if (pid == 0){
                //tercer hijo
                dup2(fd2[READ_END],STDIN_FILENO);
                close(fd2[READ_END]);
                execlp("/usr/bin/wc","wc","-l",NULL); 
            }
            
        }
    }
    close(fd2[READ_END]);
    //esperamos a que termine cada hijo 
    wait(&status);
    wait(&status);
    wait(&status);
}