/*
 * cliente.c - Un cliente de sockets TCP
 * Uso: ./cliente <hostname> <puerto>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <dirent.h> // Para escanear directorios

// Función para reportar errores y salir
void error(const char *msg) {
    perror(msg);
    exit(1);
}

// Prototipo de la función para escanear carpeta
void escanear_carpeta(int sock);

int main(int argc, char *argv[]) {
    int sockfd, puerto, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[1024];

    if (argc < 3) {
        fprintf(stderr, "Uso: %s <hostname> <puerto>\n", argv[0]);
        exit(1);
    }

    puerto = atoi(argv[2]);
    // 1. Crear el socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR al abrir el socket");

    // Obtener la información del host servidor
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no se encontró el host\n");
        exit(1);
    }

    // Inicializar la estructura serv_addr
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(puerto);

    // 2. Conectar con el servidor
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR al conectar");

    printf("Conectado al servidor. ¡Listo para enviar mensajes!\n");

    // Bucle del menú de opciones
    int opcion;
    do {
        printf("\n--- MENU ---\n");
        printf("1. Enviar mensaje\n");
        printf("2. Escanear carpeta y enviar lista de archivos\n");
        printf("3. Desconectar del servidor\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        getchar(); // Consumir el newline que queda en el buffer

        switch (opcion) {
            case 1:
                printf("Escriba su mensaje: ");
                bzero(buffer, 1024);
                fgets(buffer, 1023, stdin);
                buffer[strcspn(buffer, "\n")] = 0; // Eliminar el salto de línea

                // 3. Enviar mensaje al servidor
                n = write(sockfd, buffer, strlen(buffer));
                if (n < 0) error("ERROR en write");

                // 4. Leer respuesta del servidor
                bzero(buffer, 1024);
                n = read(sockfd, buffer, 1023);
                if (n < 0) error("ERROR en read");

                printf("Respuesta del servidor: %s\n", buffer);
                break;

            case 2:
                escanear_carpeta(sockfd);
                break;

            case 3:
                printf("Desconectando del servidor...\n");
                break;

            default:
                printf("Opción no válida.\n");
                break;
        }
    } while (opcion != 3);

    // 5. Cerrar el socket
    close(sockfd);
    return 0;
}

// Función para la opción de escanear carpeta
void escanear_carpeta(int sock) {
    DIR *d;
    struct dirent *dir;
    char buffer[1024];
    char *file_list[256]; // Arreglo para guardar los nombres de archivo
    int file_count = 0;

    d = opendir("."); // Abrir el directorio actual
    if (d) {
        // Contar archivos y guardar sus nombres
        while ((dir = readdir(d)) != NULL && file_count < 256) {
            if (dir->d_type == DT_REG) { // Contar solo archivos regulares
                file_list[file_count] = strdup(dir->d_name);
                file_count++;
            }
        }
        closedir(d);
    } else {
        error("No se pudo abrir el directorio actual");
    }

    // A. Enviar comando al servidor
    bzero(buffer, 1024);
    sprintf(buffer, "%d Escanear_Carpeta", file_count);
    if (write(sock, buffer, strlen(buffer)) < 0) {
        error("ERROR escribiendo comando de escaneo al socket");
    }

    printf("Enviando %d nombres de archivo al servidor...\n", file_count);

    // B. Enviar la lista de archivos uno por uno
    for (int i = 0; i < file_count; i++) {
        if (write(sock, file_list[i], strlen(file_list[i])) < 0) {
            error("ERROR escribiendo nombre de archivo al socket");
        }
        free(file_list[i]); // Liberar memoria
        usleep(10000); // Pequeña pausa para evitar que los mensajes se peguen (opcional)
    }

    printf("Lista de archivos enviada con éxito.\n");
}