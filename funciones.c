#include "funciones.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

//Desarrollo de funciones 
/*
fork(): al ser invocada genera un proceso hijo, el cual es casi identico al padre 
a diferencia de su pid, retorna el pid del hijo al padre, la hijo le retorna en la 
copia de la llamada fork() el valor cero y ejecuta el proceso hijo. fork() devuelve 
el ID del proceso hijo en el proceso padre (else) y devuelve 0 en el proceso hijo y 
-1 si la llamada falla. 
*/
void pruebaFork(){
    printf("Impresion desde el proceso padre\n");
    int xd = 11111;
    printf("Valor del xdpadre antes del fork() -%d-\n",xd);
    //Hacemos un fork
    xd=fork();
    if (xd != 0){
        printf("Valor del xdpadre despues del primer fork() -%d-\n",xd);
    }else{
        printf("Valor de xdhijo despues del primer fork() -%d-\n",xd);
        xd=22222;
        printf("Valor modificado de xdhijo antes del segundo fork() -%d-\n",xd);
        //Segundo fork
        xd = fork(); 
        if (xd != 0){
            printf("Valor de xdhijo despues del segundo fork() -%d-\n",xd);
        }else{
            printf("Valor de xdnieto despues del segundo fork() -%d-\n",xd);
        }
    }
}

void pruebaFork2(){
    pid_t pid; 
    pid = fork();
    if (pid == 0){
        //Hijo
        printf("\tSoy el hijo (%d, hijo de %d).\n",getpid(), getppid()); 
    }else{
        //Padre
        printf("\tSoy el padre (%d, padre de %d).\n",getpid(),getppid());
        wait(NULL);
        exit(0);
    }
    
}

/*
getpid(): retorna el pid del proceso que la invoco devuelve un entero de tipo 
pid_t
getppid(): retorna el pid del padre del proceso que la invoca 
*/

void pruebaGetPid(){
    pid_t pid; 
    pid = fork();
    if (pid == 0){
        //Hijo
        printf("\tSoy el hijo mi identificador es: %d\n",getpid()); 
        printf("\tEl PID del padre que me invoca es: %d\n",getppid());
    }else{
        //Padre
        printf("\tSoy el padre mi identificador es: %d\n",getpid());
        printf("\tEl PID del padre que me invoca es: %d\n",getppid());
        wait(NULL);
        exit(0);
    }
}

/*
Llamadas exec(): se utilizan para poner en ejecucion un proceso determinado, todas reciben 
como primer parametro la ruta absoluta del archivo a ejecutar, la diferencia esta en los 
demas argumentos son: 

    execl(const char *path, const char *arg, ...);
    execlp(const char *file, const char *arg, ...);
    execle(const char *path, const char *arg , ..., char * const envp[]);
    execv(const char *path, char *const argv[]);
    execvp(const char *file, char *const argv[]);

*/
void pruebaExec(){
    //En este ejemplo haremos la llamada execlp para ejecutar el comando ls 
    printf("\tLlamada a un programa usando execlp: \n");
    execlp("/bin/ls","/bin/ls","-F","-l",NULL);
    printf("En caso de fallo este mensaje deberia salir por pantalla");
    exit(0);
}

/*
Señales: permite establecer comunicacion con otro procesos que se estan ejecutando en la 
maquina, se suelen usar para controlar las tareas son: 

    1) SIGHUP       2) SIGINT       3) SIGQUIT      4) SIGILL
    5) SIGTRAP      6) SIGABRT      7) SIGBUS       8) SIGFPE
    9) SIGKILL     10) SIGUSR1     11) SIGSEGV     12) SIGUSR2
    13) SIGPIPE     14) SIGALRM     15) SIGTERM     16) SIGSTKFLT
    17) SIGCHLD     18) SIGCONT     19) SIGSTOP     20) SIGTSTP
    21) SIGTTIN     22) SIGTTOU     23) SIGURG      24) SIGXCPU
    25) SIGXFSZ     26) SIGVTALRM   27) SIGPROF     28) SIGWINCH
    29) SIGIO       30) SIGPWR      31) SIGSYS      34) SIGRTMIN
    35) SIGRTMIN+1  36) SIGRTMIN+2  37) SIGRTMIN+3  38) SIGRTMIN+4
    39) SIGRTMIN+5  40) SIGRTMIN+6  41) SIGRTMIN+7  42) SIGRTMIN+8
    43) SIGRTMIN+9  44) SIGRTMIN+10 45) SIGRTMIN+11 46) SIGRTMIN+12
    47) SIGRTMIN+13 48) SIGRTMIN+14 49) SIGRTMIN+15 50) SIGRTMAX-14
    51) SIGRTMAX-13 52) SIGRTMAX-12 53) SIGRTMAX-11 54) SIGRTMAX-10
    55) SIGRTMAX-9  56) SIGRTMAX-8  57) SIGRTMAX-7  58) SIGRTMAX-6
    59) SIGRTMAX-5  60) SIGRTMAX-4  61) SIGRTMAX-3  62) SIGRTMAX-2
    63) SIGRTMAX-1  64) SIGRTMAX

    https://www.programacion.com.py/escritorio/c/senales-y-alarmas-en-c-linux
*/
void pruebaSenal1(){
    int i; 
    for (i = 0; i <= 64; i++){
        signal(i,trapper);
        printf("Identificativo del proceso: %d\n",getpid());
        pause();
        printf("Continuando...\n");
    }
    
}

//funcion que se usa para la funcion anterior 
void trapper(int sig){
    //sig es el numero de una señal y trapper es una funcion hecha por el programador o predefinida 
    signal(sig, trapper);
    printf("Recibida la señal: %d\n",sig); 
}

//funcion que prueba la señal SIGINT = se pulsa Ctrl+c
void pruebaSenal2(){
    signal(SIGINT,manejador); 
    while (1){
        //bucle infinito a la espera de que se introduzca la señal adecuada 
        sleep(1);
    }
    
}

//funcion manejadora de pruebaSenal2
void manejador(int signum){ 
    if (signum == SIGINT){
         printf("\tRecibi la señal SIGINT\n");
    }else{
        printf("No se de que señal se trata");
    }
    
}


