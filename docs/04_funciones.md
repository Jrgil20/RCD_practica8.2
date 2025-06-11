# Funciones para Manejo de Sockets

## Funciones Principales

### socket()
```c
int socket(int domain, int type, int protocol);
```

| Parámetro | Descripción | Valores Comunes |
|-----------|-------------|----------------|
| `domain` | Dominio de comunicación | `AF_INET`, `AF_UNIX` |
| `type` | Tipo de socket | `SOCK_STREAM`, `SOCK_DGRAM` |
| `protocol` | Protocolo | `0` (por defecto) |

**Retorno**: Descriptor de socket o `-1` si hay error

### bind()
```c
int bind(int fd, struct sockaddr *my_addr, int addrlen);
```

| Parámetro | Descripción |
|-----------|-------------|
| `fd` | Descriptor de socket |
| `my_addr` | Puntero a estructura sockaddr |
| `addrlen` | `sizeof(struct sockaddr)` |

> **Nota**: Puertos < 1024 están reservados, usar entre 1024-65535

### connect()
```c
int connect(int fd, struct sockaddr *serv_addr, int addrlen);
```

| Parámetro | Descripción |
|-----------|-------------|
| `fd` | Descriptor de socket |
| `serv_addr` | Estructura con dirección IP y puerto destino |
| `addrlen` | `sizeof(struct sockaddr)` |

**Retorno**: `0` si éxito, `-1` si error

### listen()
```c
int listen(int fd, int backlog);
```

| Parámetro | Descripción |
|-----------|-------------|
| `fd` | Descriptor de socket |
| `backlog` | Número máximo de conexiones permitidas |

**Retorno**: `0` si éxito

### accept()
```c
int accept(int fd, void *addr, int *addrlen);
```

| Parámetro | Descripción |
|-----------|-------------|
| `fd` | Descriptor de socket |
| `addr` | Puntero a estructura sockaddr_in |
| `addrlen` | `sizeof(struct sockaddr_in)` |

**Uso**: Para aceptar conexiones entrantes

## Funciones de Comunicación

### write()
```c
int write(int fd, const void *msg, int len);
```

| Parámetro | Descripción |
|-----------|-------------|
| `fd` | Descriptor de socket |
| `msg` | Puntero a datos a enviar |
| `len` | Longitud de datos en bytes |

**Propósito**: Enviar datos por socket

### read()
```c
int recv(int fd, void *buf, int len);
```

| Parámetro | Descripción |
|-----------|-------------|
| `fd` | Descriptor de socket |
| `buf` | Buffer para datos recibidos |
| `len` | Longitud máxima del buffer |

**Propósito**: Recibir datos por socket

## Funciones de Utilidad

### close()
```c
close(fd);
```

**Propósito**: Cerrar conexión del descriptor de socket

### gethostname()
```c
int gethostname(char *hostname, size_t size);
```

| Parámetro | Descripción |
|-----------|-------------|
| `hostname` | Array para nombre del nodo |
| `size` | Longitud del array |

**Propósito**: Obtener nombre de la máquina local

## Diagrama de Flujo

```mermaid
graph TD
    A[Cliente] -->|socket()| B[Crear Socket]
    B -->|connect()| C[Conectar al Servidor]
    C -->|write()| D[Enviar Datos]
    D -->|read()| E[Recibir Respuesta]
    E -->|close()| F[Cerrar Conexión]
    
    G[Servidor] -->|socket()| H[Crear Socket]
    H -->|bind()| I[Asociar Puerto]
    I -->|listen()| J[Escuchar Conexiones]
    J -->|accept()| K[Aceptar Conexión]
    K -->|read()| L[Recibir Datos]
    L -->|write()| M[Enviar Respuesta]
    M -->|close()| N[Cerrar Conexión]
```

> **Nota**: El diagrama muestra el flujo de comunicación entre cliente y servidor usando las funciones mencionadas. 