# Introducción a Sockets

## Historia y Origen

La implementación de socket nace de un proyecto del Advanced Research Projects Agency (ARPA) ejecutado por un grupo de trabajo de la Universidad de California (UC, Berkeley) para llevar TCP/IP a sistemas operativos UNIX. De este proyecto nace la interfaz socket, algunas veces denominado Berkeley Socket Interface. La interfaz de socket Berkeley es considerado un estándar de facto. Este interfaz es el que se utiliza en Linux.

## Concepto de Socket

Un socket es una abstracción para comunicación de redes, tal como "file" es una abstracción para sistemas de comunicación de archivos. La interfaz define una variedad de funciones de software o rutinas que permiten a los desarrolladores implementar aplicaciones para uso en redes de computadores.

## Descriptores de Socket

La interfaz socket puede considerarse como una extensión de un descriptor de archivo, usando el mismo concepto y redefiniéndolo como un descriptor de socket. El descriptor de archivo es un entero regresado por una función que una aplicación puede utilizar para realizar operaciones sobre el mismo. El sistema operativo mantiene una tabla de descriptores de archivo para cada proceso.

### Proceso de Apertura de Archivo

Cuando un proceso abre un archivo:

1. Crea un apuntador a la estructura interna a los datos para dicho archivo
2. Este apuntador es colocado en una tabla de descriptor de archivo
3. Cuando se hace una llamada al archivo, es retornado un índice a un apuntador a la función que hace la llamada
4. La aplicación solo requiere un descriptor del archivo para realizar el trabajo sobre dicho archivo
5. El sistema operativo maneja las operaciones subyacentes para seguir al apuntador en la tabla de descripción y así obtener la información que requiere

## Tipos de Sockets

Los socket pueden ser usados de dos maneras:

### Socket Activo
- Inicia una comunicación hacia otro socket en un host remoto o en localhost
- Se utiliza para establecer conexiones con servidores
- Ejemplo: Un navegador web conectándose a un servidor

### Socket Pasivo
- Actúa como un servidor y espera por conexiones entrantes
- Se utiliza para escuchar y aceptar conexiones de clientes
- Ejemplo: Un servidor web esperando conexiones de navegadores

> **Nota**: Ambos tipos de sockets son creados de la misma manera, la diferencia está en cómo la red hace uso de ellos. 