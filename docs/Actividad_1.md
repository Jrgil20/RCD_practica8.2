# Actividad 1 — Cliente-Servidor básico con sockets TCP

## Objetivo
Implementar dos programas: Cliente y Servidor usando sockets orientados a conexión (TCP).

## Interacción esperada
El cliente envía un mensaje → el servidor lo imprime y responde → el cliente muestra la respuesta.

## Componentes esenciales

### Cabeceras necesarias
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
```

### Servidor
1. Crear el socket con `socket()`
2. Llenar estructura `sockaddr_in` con IP y puerto
3. Usar `bind()` para asociar socket a puerto
4. Usar `listen()` para esperar conexiones
5. Aceptar conexión con `accept()`
6. Leer mensaje con `read()` o `recv()`
7. Responder con `write()`
8. Cerrar sockets con `close()`

### Cliente
1. Crear socket con `socket()`
2. Obtener IP del servidor con `gethostbyname()`
3. Llenar estructura `sockaddr_in`
4. Usar `connect()` al servidor
5. Enviar mensaje con `write()`
6. Leer respuesta con `read()`
7. Cerrar socket

## Recomendación de estructura
- Define una constante para el puerto (ej. `#define PORT 5000`)
- Usa variables como:
  - `sockfd`
  - `newsockfd`
  - `serv_addr`
  - `cli_addr`

