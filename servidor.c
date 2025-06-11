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
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>

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
    int opt = 1;

    if (argc < 2) {
        fprintf(stderr, "Uso: %s <puerto>\n", argv[0]);
        exit(1);
    }

    puerto = atoi(argv[1]);
    
    // Validar puerto
    if (puerto <= 0 || puerto > 65535) {
        fprintf(stderr, "ERROR: Puerto debe estar entre 1 y 65535\n");
        exit(1);
    }

    // 1. Crear el socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR al abrir el socket");

    // Permitir reutilizar la dirección (evitar "Address already in use")
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        error("ERROR en setsockopt");
    }

    // Inicializar la estructura serv_addr
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; // Aceptar conexiones en cualquier interfaz
    serv_addr.sin_port = htons(puerto);     // Convertir el puerto a formato de red

    // 2. Asociar el socket a una dirección (bind)
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR en bind");

    // 3. Poner el socket en modo de escucha
    if (listen(sockfd, 5) < 0) // Aceptar hasta 5 conexiones en cola
        error("ERROR en listen");
    
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
        if (newsockfd < 0) {
            if (errno == EINTR) {
                // accept() fue interrumpido por una señal, continuar
                continue;
            }
            error("ERROR en accept");
        }

        // Crear un proceso hijo para manejar al nuevo cliente
        pid_t child_pid = fork();
        if (child_pid == -1) {
            perror("ERROR en fork");
            close(newsockfd);
            continue;
        }
        
        if (child_pid == 0) {
            // Este es el proceso hijo
            close(sockfd); // El hijo no necesita el socket de escucha

            // Obtener IP del cliente para mostrar en consola
            char client_ip[INET_ADDRSTRLEN];
            if (inet_ntop(AF_INET, &cli_addr.sin_addr, client_ip, sizeof(client_ip)) == NULL) {
                strcpy(client_ip, "IP_DESCONOCIDA");
            }
            printf("Cliente conectado: %s:%d (PID: %d)\n", 
                   client_ip, ntohs(cli_addr.sin_port), getpid());

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
    char client_ip[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &cli_addr->sin_addr, client_ip, sizeof(client_ip)) == NULL) {
        strcpy(client_ip, "IP_DESCONOCIDA");
    }
    
    char client_info[256];
    snprintf(client_info, sizeof(client_info), "Cliente %s:%d", 
             client_ip, ntohs(cli_addr->sin_port));

    printf("[%s] Conexión establecida\n", client_info);

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        
        // 5. Leer datos del cliente
        n = read(sock, buffer, sizeof(buffer) - 1);
        if (n <= 0) { // Si n es 0, el cliente cerró la conexión
            if (n == 0) {
                printf("[%s] se ha desconectado.\n", client_info);
            } else {
                printf("[%s] ERROR en read: %s\n", client_info, strerror(errno));
            }
            break; // Salir del bucle
        }
        
        buffer[n] = '\0'; // Asegurar terminación nula
        printf("[%s] Mensaje recibido: %s\n", client_info, buffer);

        // Lógica para manejar los comandos del cliente
        if (strstr(buffer, "Escanear_Carpeta") != NULL) {
            int num_archivos;
            if (sscanf(buffer, "%d Escanear_Carpeta", &num_archivos) == 1) {
                printf("[%s] Recibiendo lista de %d archivos...\n", client_info, num_archivos);

                for (int i = 0; i < num_archivos; i++) {
                    // Leer longitud del nombre del archivo (4 bytes en orden de red)
                    uint32_t len_net;
                    if (read(sock, &len_net, sizeof(len_net)) <= 0) {
                        printf("[%s] ERROR: Conexión perdida al leer longitud del nombre del archivo\n", client_info);
                        break;
                    }
                    uint32_t len = ntohl(len_net);
                    if (len >= sizeof(buffer)) {
                        printf("[%s] ERROR: Longitud del nombre del archivo excede el buffer\n", client_info);
                        break;
                    }
                    // Leer el nombre del archivo
                    memset(buffer, 0, sizeof(buffer));
                    n = read(sock, buffer, len);
                    if (n <= 0) {
                        printf("[%s] ERROR: Conexión perdida al leer nombre del archivo\n", client_info);
                        break;
                    }
                    buffer[n] = '\0';
                    printf("  -> Archivo %d: %s\n", i + 1, buffer);
                }
                
                if (n > 0) {
                    printf("[%s] Lista de archivos recibida completamente.\n", client_info);
                }
            } else {
                printf("[%s] ERROR: Formato de comando Escanear_Carpeta inválido\n", client_info);
            }

        } else {
            // 6. Responder al cliente (echo)
            char respuesta[1024];
            snprintf(respuesta, sizeof(respuesta), "Servidor recibio tu mensaje: %s", buffer);
            n = write(sock, respuesta, strlen(respuesta));
            if (n < 0) {
                printf("[%s] ERROR en write: %s\n", client_info, strerror(errno));
                break;
            }
        }
    }

    // 7. Cerrar la conexión con este cliente
    printf("[%s] Cerrando conexión\n", client_info);
    close(sock);
}