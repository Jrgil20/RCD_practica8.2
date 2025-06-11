# RCD_practica9

Redes de Comunicacion
 - LABORATORIO
    - Practica 9:
      - Programación de Sockets

## Actividades

- [Actividad 1 - Cliente-Servidor básico con sockets TCP](docs/Actividad_1.md)
- [Actividad 2 - Servidor con múltiples clientes y menú interactivo](docs/Actividad_2.md)

## Documentación

La documentación detallada sobre sockets se encuentra en la carpeta `docs/`:

- [Introducción a Sockets](docs/01_introduccion.md)
- [Constantes y Tipos de Sockets](docs/02_constantes.md)
- [Estructuras de Datos](docs/03_estructuras.md)
- [Funciones para Manejo de Sockets](docs/04_funciones.md)

## Cómo compilar y ejecutar

Para poder probar los programas, sigue estos pasos:

1. Guarda el código en dos archivos separados: `servidor.c` y `cliente.c`.

2. Abre una terminal y compila ambos programas usando gcc:

```shell
gcc -o servidor servidor.c
gcc -o cliente cliente.c
```

3. Ejecuta el servidor primero. Debes especificar un número de puerto (por ejemplo, 9002).

```shell
./servidor 9002
```

El servidor se quedará esperando conexiones.

4. Abre una o más terminales nuevas y ejecuta el cliente en cada una. Debes especificar el hostname (si es en la misma máquina, usa localhost o 127.0.0.1) y el mismo puerto que usaste para el servidor.

```shell
# En la terminal 2
./cliente localhost 9002

# En la terminal 3
./cliente localhost 9002
```
Cada cliente se conectará al servidor y podrás interactuar con el menú de forma independiente. En la terminal del servidor, verás los mensajes que llegan de cada cliente.

