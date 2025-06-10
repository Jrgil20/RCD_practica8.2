/*
 * servidor.c - Un servidor de sockets TCP concurrente
 * Uso: ./servidor <puerto>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h> // Para el manejo de procesos hijos

// Prototipo de la función que maneja la conexión con un cliente
void procesar_cliente(int sock, struct sockaddr_in *cli_addr);

// Función para reportar errores y salir
void error(const char *msg) {
    perror(msg);
    exit(1);
}

// Función para manejar la terminación de procesos hijos (zombies)
void sigchld_handler(int s) {
    // waitpid() podría sobreescribir errno, así que lo guardamos y restauramos
    int saved_errno = errno;
    while(waitpid(-1, NULL, WNOHANG) > 0);
    errno = saved_errno;
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, puerto;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    struct sigaction sa;

    if (argc < 2) {
        fprintf(stderr, "Uso: %s <puerto>\n", argv[0]);
        exit(1);
    }

    // 1. Crear el socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR al abrir el socket");

    // Inicializar la estructura serv_addr
    bzero((char *) &serv_addr, sizeof(serv_addr));
    puerto = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; // Aceptar conexiones en cualquier interfaz
    serv_addr.sin_port = htons(puerto);     // Convertir el puerto a formato de red

    // 2. Asociar el socket a una dirección (bind)
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR en bind");

    // 3. Poner el socket en modo de escucha
    listen(sockfd, 5); // Aceptar hasta 5 conexiones en cola
    clilen = sizeof(cli_addr);

    // Configurar el manejador para limpiar procesos hijos (zombies)
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        error("ERROR en sigaction");
    }

    printf("Servidor esperando conexiones en el puerto %d...\n", puerto);

    // Bucle principal para aceptar conexiones
    while (1) {
        // 4. Aceptar una nueva conexión (bloqueante)
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0)
            error("ERROR en accept");

        // Crear un proceso hijo para manejar al nuevo cliente
        if (!fork()) {
            // Este es el proceso hijo
            close(sockfd); // El hijo no necesita el socket de escucha

            // Obtener IP y puerto del cliente para mostrar en consola
            char client_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &cli_addr.sin_addr, client_ip, sizeof(client_ip));
            printf("Cliente conectado: %s:%d\n", client_ip, ntohs(cli_addr.sin_port));

            procesar_cliente(newsockfd, &cli_addr); // Procesar la comunicación
            exit(0); // El hijo termina su trabajo
        }
        // El proceso padre cierra el socket de la nueva conexión y vuelve a esperar
        close(newsockfd);
    }

    close(sockfd);
    return 0;
}

// Función para la comunicación con el cliente
void procesar_cliente(int sock, struct sockaddr_in *cli_addr) {
    char buffer[1024];
    int n;

    // Obtener información del cliente para los logs
    char client_info[128];
    sprintf(client_info, "Cliente %s:%d", inet_ntoa(cli_addr->sin_addr), ntohs(cli_addr->sin_port));

    while (1) {
        bzero(buffer, 1024);
        // 5. Leer datos del cliente
        n = read(sock, buffer, 1023);
        if (n <= 0) { // Si n es 0, el cliente cerró la conexión
            if (n == 0)
                printf("%s se ha desconectado.\n", client_info);
            else
                perror("ERROR en read");
            break; // Salir del bucle
        }

        printf("[%s] Mensaje recibido: %s\n", client_info, buffer);

        // Lógica para manejar los comandos del cliente
        if (strstr(buffer, "Escanear_Carpeta") != NULL) {
            int num_archivos;
            sscanf(buffer, "%d Escanear_Carpeta", &num_archivos);
            printf("[%s] Recibiendo lista de %d archivos...\n", client_info, num_archivos);

            for (int i = 0; i < num_archivos; i++) {
                bzero(buffer, 1024);
                n = read(sock, buffer, 1023);
                if (n <= 0) break;
                printf("  -> Archivo %d: %s\n", i + 1, buffer);
            }
            printf("[%s] Lista de archivos recibida.\n", client_info);

        } else {
            // 6. Responder al cliente (echo)
            char respuesta[1024];
            sprintf(respuesta, "Servidor recibio tu mensaje: %s", buffer);
            n = write(sock, respuesta, strlen(respuesta));
            if (n < 0) {
                perror("ERROR en write");
                break;
            }
        }
    }

    // 7. Cerrar la conexión con este cliente
    close(sock);
}