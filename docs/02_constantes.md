# Constantes y Tipos de Sockets

## Creación de Sockets

Para crear un socket se necesitan tres argumentos:
- **Familia**: AF_INET para TCP/IP; AF_UNIX interno
- **Tipo**: tipo de servicio (SOCK_STREAM para TCP y SOCK_DGRAM para UDP)
- **Protocolo**: el número de protocolo a usar, típicamente cero para usar el valor por defecto para el tipo y la familia dada

## Constantes Predefinidas

Las constantes agrupan a los símbolos predefinidos para el manejo de sockets. Hay dos grupos principales:

### Protocol Type (Tipos de Protocolos)
- **SOCK_STREAM**: Para conexiones TCP
- **SOCK_DGRAM**: Para conexiones UDP

### Address Family (Familias de Direcciones)
También conocidas como "protocol families":
- **AF_INET**: Familia de protocolos internet para todas las comunicaciones TCP/IP incluyendo UDP
- **AF_UNIX**: Usada cuando las comunicaciones de socket son internas al localhost
- **PF_INET**: Alternativa a AF_INET (ambas formas son intercambiables)

## Ejemplo de Creación de Socket

Una llamada a `socket()` para propósito de abrir una conexión a otro host y comunicarse con TCP sería:

```c
mySocket = socket(PF_INET, SOCK_STREAM, 0);
```

Estas constantes se encuentran en los archivos header "types.h" y "socket.h" del sistema operativo, generalmente ubicados en la ruta "/usr/include". 