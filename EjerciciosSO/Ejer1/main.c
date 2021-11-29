//Librerias 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAXBUF 512
#define SIZE 512

pid_t pid1;
pid_t pid2;

//Escribir un programa que cree dos procesos que actúen de productor y de consumidor respectivamente. El produc-
//tor abrirá el fichero denominado datos.txt y leerá los caracteres almacenados en él. El consumidor tendrá que
//calcular e imprimir por la salida estándar el número de caracteres almacenados en ese fichero sin leer del fichero.
//La comunicación entre los dos procesos debe hacerse utilizando un pipe. Por otra parte, el proceso padre tendrá
//que abortar la ejecución de los dos procesos hijos, si transcurridos 60 segundos éstos no han acabado su ejecución.

//Prototipos 
void MatarProcesos(); 
void Productor(int f);
void Consumidor(int f);
void pruebaTuberia();
void pruebaTuberia2();

//Main 
int main(int argc, char const *argv[]){
    
    int fd[2]; 
    struct sigaction act; 

    //Creamos el pipe (comunicador entre procesos)
    if (pipe(fd) < 0){
        perror("Error al crear el pipe");
        return 0; 
    }
    
    //creamos el primer hijo 
    pid1 = fork(); 
    switch (pid1){
        case -1:
            perror("Error en el fork()"); 
            break;
        case 0: 
            //hijo productor 
            close(fd[0]); 
            //Llamamos a la funcion productor 
            Productor(fd[1]); 
            close(fd[1]);
            return 0; 
        default:
            //proceso padre 
            //creamos un segundo hijo 
            pid2 = fork();
            switch (pid2){
                case -1:
                    perror("Error en el fork()");
                    return 0; 
                case 0: 
                    //proceso hijo consumidor 
                    close(fd[1]);
                    //Llamamos a la funcion consumidor 
                    Consumidor(fd[0]);
                    close(fd[0]);
                default:
                    //proceso padre 
                    //cierra los descriptores de lectura porque no los necesita 
                    close(fd[0]); 
                    close(fd[1]); 
                    //Se prepara para recibir la señal SIGALRM 
                    act.sa_handler = MatarProcesos; 
                    act.sa_flags = 0; 
                    sigaction(SIGALRM,&act,NULL); 
                    alarm(60); 
                    //cuando llegue la señal SIGALRM se ejecuta la funcion Matar procesos 
                    //Esperamos a que los dos procesos hijos acaben si vence el tiempo se les mata 
                    wait(NULL);
                    wait(NULL);
            }
    }

    //pruebaTuberia();
    pruebaTuberia2();
    return 0;
}

//Desarrollo de funciones 
void Productor(int f){
    //El proceso productor lee del fichero y escribe los datos en el pipe 
    //utilizando el descriptor de escritura que se le pasa como parametro 
    int fd; 
    int n; 
    char buf[MAXBUF];
    //abrimos el archivo 
    fd = open("datos.txt",O_RDONLY);
    if (fd == -1){
        perror("Error en el open");
    }else{  
        while ((n = read(fd,buf,MAXBUF)) != 0){
            write(f,buf,n); 
            if (n == -1){
                perror("Error en el write");
            }
        }
    }
    close(fd); 
}

void Consumidor(int f){
    //El proceso consumidor lee del pipe utilizando el descriptor de lectura 
    //que se le pasa como parametro y escribe por salida estandar (descriptor 1)
    int n; 
    char buf[MAXBUF];

    while ((n = read(f,buf,MAXBUF)) != 0){
        write(1,buf,n); 
        if(n == -1){
            perror("Error en read()"); 
        }
    }
}

void MatarProcesos(){
    //se encargara de matar los procesos hijos 
    kill(pid1,SIGKILL);
    kill(pid2,SIGKILL);
    exit(0);
}

/*
Tuberias, pipe o |: es una cadena de procesos conectados, permite la comunicacion y 
sincronizacion entre procesos, Una tuberia tiene dos descriptores de fichero uno para 
el extremode escritura y otro para lectura. Como los descriptores de ficheros son enteros 
un pipe no es mas que un array de dos enteros: int tuberia[2]; 

Para crear la tuberia se emplea la funcion pipe() que abre dos descriptores de fichero y 
los almacena en el array, el primero es solo para lecturas (O_RDONLY) y el segundo para 
escrituras (O_WRONLY): pipe(tuberia) 

Se suelen usar para intercambiar dtos con otro procesos 
*/

//La tubería “p” se hereda al hacer el fork() que da lugar al proceso hijo, pero es necesario que el padre 
//haga un close() de p[0] (el lado de lectura de la tubería), y el hijo haga un close() de p[1] (el lado 
//de escritura de la tubería). Una vez hecho esto, los dos procesos pueden emplear la tubería 
//para comunicarse (siempre unidireccionalmente), haciendo write() en p[1] y read() en p[0],respectivamente.

void pruebaTuberia(){
    pid_t pid; 
    int p[2], readbytes;
    char buffer[SIZE];

    pipe(p); 
    if ((pid = fork()) == 0){
        //hijo
        //cerramos el lado de escritura del pipe 
        close(p[1]); 
        while ((readbytes = read(p[0],buffer,SIZE)) > 0){
            write(1,buffer,readbytes);
        }
        close(p[0]); 
    }else{
        //padre
        //cerramos el lado de lectura del pipe
        close(p[0]); 
        strcpy(buffer,"Esto llega a traves de la tuberia\n\n"); 
        write(p[1],buffer,strlen(buffer));
        close(p[1]);
    }
    waitpid(pid,NULL,0);
    exit(0);
}

void pruebaTuberia2(){
    int fd[2];
    char buffer[30];
    pid_t pid;

    pipe(fd); // Se crea el PIPE
    pid = fork();

    switch(pid){
        case -1: // Error
            printf("No se ha podido crear un hijo \n");
            exit(-1);
            break;
        case 0: // Hijo
            close(fd[0]); // Cierra el descriptor que no va a usar. El de lectura
            printf("El hijo escribe en el PIPE... \n");
            write(fd[1], "Hola papi", 10);
            break;
        default: // Padre
            close(fd[1]); // Cierra el descriptor de escritura
            wait(NULL); // Espera a que finalice el hijo
            printf("El padre lee el PIPE \n");
            read(fd[0], buffer, 10);
            printf("\t Mensaje leido: %s \n", buffer);
    }
}