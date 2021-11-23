#include "funciones.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>


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

//funcion que prueba la señal kill
int bandera = 1;
void pruebaSenal3(){
    int status, pid; 
    //si fork() es igual a 0 entonces es hijo 
    if ((pid=fork()) == 0){
        //proceso hijo 
        printf("Soy el hijo y estoy esperando una señal de mi padre, mi PID es: %d\n",getpid()); 
        //capturamos la señal SIGUSR1
        signal(SIGUSR1,manejadorKill);
        //utilizamos la señal de bandera para que el proceso no termine 
        while (bandera){
            //usamos el comando kill para enviar una señal 
            //se necesita el PID del padre y la señal 
            kill(getppid(),SIGUSR2); 
        }
    }else{
        //proceso padre 
        //capturamos la señal SIGUSR2
        signal(SIGUSR2,manejadorKill); 
        printf("Soy el Padre, mi PID es: %d\n",getpid());
        //esperamos 3 segundos 
        sleep(2); 
        kill(pid,SIGUSR1);
        //esperamos a que termine el hijo 
        wait(&status); 
        printf("Mi hijo termino con un estado:  %d\n",status); 
    }
}

void manejadorKill(int signum){
    if (signum == SIGUSR1){
        printf("Recibi una señal de mi padre %d\n",signum);
    }else{
        //si es SIGUSR2
        printf("Recibi una señal de mi hijo: %d\n",signum); 
    }
    bandera = 0;    
}

/*
Llamadas para el manejo de ficheros: estan la funcion create, open, close, unlink, read, write, lseek
para la funcion open existen unos modos de apertura son: 

    O_RDONLY :abre en modo lectura
    O_WRONLY: abre en modo escritura
    O_RDWR: abre en modo lectura-escritura
    O_APPEND: abre en modo apéndice (escritura desde el final)
    O_CREAT: crea el fichero y lo abre (si existía, se lo machaca)
    O_EXCL: usado con O_CREAT. Si el fichero existe, se retorna un error
    O_TRUNC: abre el fichero y trunca su longitud a 0

    Para lseek: 
    SEEK_SET: inicio del fichero
    SEEK_CUR: relativo a la posición actual
    SEEK_END: relativo al final del fichero
*/

//Funcion que crea un fichero y escribe en el unos caracteres
void ficheros1(){
    //cadena que se va a escribir
    const char *cadena = "Hola, mundo"; 
    //creacion y apertura del fichero
    //int fichero = open ("mi_fichero", O_CREAT|O_WRONLY,0644);
    int descriptor = open("archivo.txt",O_CREAT|O_WRONLY,0644);
    if (descriptor == -1){
        perror("Error al abrir el fichero");
        exit(1);
    }
    //Escritura de la cadena 
    write(descriptor,cadena,strlen(cadena));
    printf("\tSe ha escrito la cadena en archivo.txt\n");
    close(descriptor);
}

//Funcion que lee 10 caracteres, a partir de la posicion 400, de un fichero ya existente 
void ficheros2(){
    //Deposito de los caracteres 
    char cadena[11]; 
    int leidos; 
    //Abrimos el fichero 
    int descriptor = open("archivo.txt",O_RDONLY);
    if (descriptor == -1){
        perror("Error al abrir el fichero"); 
    }
    //colocamos el puntero en la posicion 400 inicio del fichero 
    lseek(descriptor,0,SEEK_SET);
    //Lee 10 bytes 
    leidos = read(descriptor,cadena,10);
    close(descriptor);
    cadena[10] = 0; 
    //mensaje para ver que se leyo 
    printf("\tSe leyeron %d bytes. La cadena leido es: %s\n",leidos,cadena);
}

//http://www.aprendeaprogramar.com/mod/forum/discuss.php?d=664
//https://www.aprenderaprogramar.com/index.php?option=com_content&view=article&id=936:escribir-guardar-datos-en-ficheros-o-archivos-en-lenguaje-c-fputc-putc-fputs-fprintf-ejemplos-cu00537f&catid=82&Itemid=210
//https://parzibyte.me/blog/2019/10/16/escribir-archivo-con-c-usando-fprintf/

void imprimeTiket(){
    char *nombreArchivo = "tiket.txt";     
    char *modo = "w"; 
    FILE *archivo = fopen(nombreArchivo,modo); 
    if (archivo == NULL){
        printf("Error al abrir el archivo %s",nombreArchivo); 
    }
    //Escribimos el tiket 
    fprintf(archivo,"\t-----------------------------\n"); 
    fprintf(archivo,"\t-------TIKET DE COMPRA-------\n"); 
    fprintf(archivo,"\t-----------------------------\n"); 
    fprintf(archivo,"\tGominolas          1.00 euros\n"); 
    fprintf(archivo,"\tGominolas          1.00 euros\n"); 
    fprintf(archivo,"\tGominolas          1.00 euros\n"); 
    fprintf(archivo,"\tGominolas          1.00 euros\n"); 
    fprintf(archivo,"\t                             \n");
    fprintf(archivo,"\tTotal              4.00 euros\n");
    fprintf(archivo,"\t                            \n");
    fprintf(archivo,"\t   PAGO EN EFECTIVO GRACIAS  \n");
    fprintf(archivo,"\t-----------------------------\n");  

    //cerramos el archivo
    fclose(archivo); 
    puts("\tContenido escrito correctamente en tiket.txt");
}

//programa que pregunte un nombre de fichero y muestre en pantalla el contenido de ese
//fichero, haciendo una pausa después de cada 25 líneas, para que dé tiempo a leerlo. Cuando el usua-
//rio pulse intro, se mostrarán las siguientes 25 líneas, y así hasta que termine el fichero.
void muestraFichero(){
    FILE *fichero; 
    char linea[100], nombre[40]; 
    int i=0; 
    do{
        printf("\n\tNombre del fichero: ");
        gets(nombre);
        fichero = fopen(nombre,"rt");
        if (fichero ==NULL){
            i++;
            if (i == 5){
                exit(1);
            }
        }
    } while (fichero == NULL);
    
    while (!feof(fichero)){
        for (i=0; i<25; i++){
            fgets(linea, 100, fichero);
            if (!feof(fichero)){
                puts(linea);                
            }
        }
        getchar();       
    }
    fclose(fichero);
}