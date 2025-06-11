# Estructuras de Datos para Sockets

La interfaz socket Berkeley provee un conjunto de estructuras definidas en C que facilitan el manejo de direcciones diferentes entre los puntos finales, entre otros beneficios.

## Estructura sockaddr_in

Una de las estructuras más utilizadas es la `sockaddr_in` que contiene una dirección IP y un número de puerto de protocolo. La estructura está definida de la siguiente manera en el archivo "in.h":

```c
/* Una estructura que contiene una direccion de Internet
   definido en el archivo de inclusion in.h */
struct sockaddr_in {
    __kernel_sa_family_t sin_family; /* Address family */
    __be16 sin_port;                /* Port number */
    struct in_addr sin_addr;        /* Internet address */
    /* Pad to size of 'struct sockaddr'. */
    unsigned char __pad[__SOCK_SIZE__ - sizeof(short int) -
                       sizeof(unsigned short int) - sizeof(struct in_addr)];
};
```

Donde:
- `sin_family`: Define el tipo de dirección (2 bytes)
- `sin_port`: Define el número de puerto de protocolo (2 bytes)
- `sin_addr`: Define la dirección de red del host remoto (4 bytes)

## Estructura hostent

Otra estructura importante se encuentra en el archivo "netdb.h" y se denomina "hostent". Las funciones utilizan la estructura hostent para almacenar información sobre un host determinado, como el nombre del host, la dirección IPv4, entre otra información.

```c
struct hostent {
    char *h_name;       /* Official name of host. */
    char **h_aliases;   /* Alias list. */
    int h_addrtype;     /* Host address type. */
    int h_length;       /* Length of address. */
    char **h_addr_list; /* List of addresses from name server. */
#ifdef __USE_MISC
# define h_addr h_addr_list[0] /* Address, for backward compatibility.*/
#endif
};
```

Esta estructura es particularmente útil cuando se necesita obtener información sobre un host a partir de su nombre o dirección IP. 