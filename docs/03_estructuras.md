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

### Campos de sockaddr_in

| Campo | Tipo | Tamaño | Descripción |
|-------|------|--------|-------------|
| `sin_family` | `__kernel_sa_family_t` | 2 bytes | Tipo de dirección |
| `sin_port` | `__be16` | 2 bytes | Número de puerto |
| `sin_addr` | `struct in_addr` | 4 bytes | Dirección IP |
| `__pad` | `unsigned char[]` | Variable | Relleno para alineación |

## Estructura hostent

Otra estructura importante se encuentra en el archivo "netdb.h" y se denomina "hostent". Las funciones utilizan la estructura hostent para almacenar información sobre un host determinado.

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

### Campos de hostent

| Campo | Tipo | Descripción |
|-------|------|-------------|
| `h_name` | `char*` | Nombre oficial del host |
| `h_aliases` | `char**` | Lista de alias del host |
| `h_addrtype` | `int` | Tipo de dirección del host |
| `h_length` | `int` | Longitud de la dirección |
| `h_addr_list` | `char**` | Lista de direcciones del servidor de nombres |
| `h_addr` | `char*` | Dirección (compatibilidad hacia atrás) |

> **Nota**: Esta estructura es particularmente útil cuando se necesita obtener información sobre un host a partir de su nombre o dirección IP. 