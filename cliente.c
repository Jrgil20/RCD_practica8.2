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
#include <dirent.h>
#include <errno.h>

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
    
    // Validar puerto
    if (puerto <= 0 || puerto > 65535) {
        fprintf(stderr, "ERROR: Puerto debe estar entre 1 y 65535\n");
        exit(1);
    }

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
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy((char *)&serv_addr.sin_addr.s_addr,
           (char *)server->h_addr,
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
        
        if (scanf("%d", &opcion) != 1) {
            printf("Opción no válida.\n");
            // Limpiar buffer de entrada
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        getchar(); // Consumir el newline que queda en el buffer

        switch (opcion) {
            case 1:
                printf("Escriba su mensaje: ");
                memset(buffer, 0, sizeof(buffer));
                if (fgets(buffer, sizeof(buffer) - 1, stdin) == NULL) {
                    printf("Error leyendo entrada\n");
                    break;
                }
                buffer[strcspn(buffer, "\n")] = 0; // Eliminar el salto de línea

                // 3. Enviar mensaje al servidor
                n = write(sockfd, buffer, strlen(buffer));
                if (n < 0) error("ERROR en write");

                // 4. Leer respuesta del servidor
                memset(buffer, 0, sizeof(buffer));
                n = read(sockfd, buffer, sizeof(buffer) - 1);
                if (n < 0) error("ERROR en read");
                buffer[n] = '\0'; // Asegurar terminación nula

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
    char **file_list = NULL; // Arreglo dinámico para nombres de archivo
    int file_count = 0;
    int capacity = 10; // Capacidad inicial

    // Asignar memoria inicial
    file_list = malloc(capacity * sizeof(char*));
    if (!file_list) {
        error("ERROR: No se pudo asignar memoria");
    }

    d = opendir("."); // Abrir el directorio actual
    if (d) {
        // Contar archivos y guardar sus nombres
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG) { // Solo archivos regulares
                // Expandir arreglo si es necesario
                if (file_count >= capacity) {
                    capacity *= 2;
                    char **temp = realloc(file_list, capacity * sizeof(char*));
                    if (!temp) {
                        // Limpiar memoria antes de salir
                        for (int i = 0; i < file_count; i++) {
                            free(file_list[i]);
                        }
                        free(file_list);
                        closedir(d);
                        error("ERROR: No se pudo reasignar memoria");
                    }
                    file_list = temp;
                }
                
                file_list[file_count] = strdup(dir->d_name);
                if (!file_list[file_count]) {
                    // Limpiar memoria antes de salir
                    for (int i = 0; i < file_count; i++) {
                        free(file_list[i]);
                    }
                    free(file_list);
                    closedir(d);
                    error("ERROR: strdup falló");
                }
                file_count++;
            }
        }
        closedir(d);
    } else {
        free(file_list);
        error("No se pudo abrir el directorio actual");
    }

    // A. Enviar comando al servidor
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer) - 1, "%d Escanear_Carpeta", file_count);
    if (write(sock, buffer, strlen(buffer)) < 0) {
        // Limpiar memoria antes de salir
        for (int i = 0; i < file_count; i++) {
            free(file_list[i]);
        }
        free(file_list);
        error("ERROR escribiendo comando de escaneo al socket");
    }

    printf("Enviando %d nombres de archivo al servidor...\n", file_count);

    // B. Enviar la lista de archivos uno por uno
    for (int i = 0; i < file_count; i++) {
        // Enviar longitud del nombre del archivo (4 bytes en orden de red)
        uint32_t len = strlen(file_list[i]);
        uint32_t len_net = htonl(len);
        if (write(sock, &len_net, sizeof(len_net)) < 0) {
            // Limpiar memoria antes de salir
            for (int j = 0; j < file_count; j++) {
                free(file_list[j]);
            }
            free(file_list);
            error("ERROR escribiendo longitud del nombre del archivo");
        }
        // Enviar el nombre del archivo
        if (write(sock, file_list[i], len) < 0) {
            // Limpiar memoria antes de salir
            for (int j = 0; j < file_count; j++) {
                free(file_list[j]);
            }
            free(file_list);
            error("ERROR escribiendo nombre del archivo");
        }
    }

    // C. Liberar memoria
    for (int i = 0; i < file_count; i++) {
        free(file_list[i]);
    }
    free(file_list);
}