# Constantes y Tipos de Sockets

## Creación de Sockets

Para crear un socket se necesitan tres argumentos fundamentales:

| Argumento | Descripción | Valores Comunes |
|-----------|-------------|----------------|
| Familia | Define el dominio de comunicación | `AF_INET` (TCP/IP)<br>`AF_UNIX` (interno) |
| Tipo | Define el tipo de servicio | `SOCK_STREAM` (TCP)<br>`SOCK_DGRAM` (UDP) |
| Protocolo | Número de protocolo | `0` (valor por defecto) |

## Constantes Predefinidas

### Protocol Type (Tipos de Protocolos)

| Constante | Descripción | Uso Común |
|-----------|-------------|-----------|
| `SOCK_STREAM` | Conexión orientada a flujo | TCP |
| `SOCK_DGRAM` | Conexión orientada a datagramas | UDP |

### Address Family (Familias de Direcciones)

También conocidas como "protocol families":

| Constante | Descripción | Uso |
|-----------|-------------|-----|
| `AF_INET` | Familia de protocolos internet | Comunicaciones TCP/IP y UDP |
| `AF_UNIX` | Familia de protocolos Unix | Comunicaciones internas al localhost |
| `PF_INET` | Familia de protocolos internet (alternativa) | Intercambiable con `AF_INET` |

## Ejemplo de Creación de Socket

```c
// Crear un socket TCP/IP
mySocket = socket(PF_INET, SOCK_STREAM, 0);

// Crear un socket UDP
mySocket = socket(PF_INET, SOCK_DGRAM, 0);
```

> **Nota**: Estas constantes se encuentran en los archivos header "types.h" y "socket.h" del sistema operativo, generalmente ubicados en la ruta "/usr/include". 