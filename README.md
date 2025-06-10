# RCD_practica9

Redes de Comunicacion
 - LABORATORIO
    - Practica 9:
      - Progracion de Sockets
----------------------------
Cómo compilar y ejecutar
Para poder probar los programas, sigue estos pasos:

Guarda el código en dos archivos separados: servidor.c y cliente.c.

Abre una terminal y compila ambos programas usando gcc:

``` shell
gcc -o servidor servidor.c
gcc -o cliente cliente.c
```
Ejecuta el servidor primero. Debes especificar un número de puerto (por ejemplo, 9002).

``` shell
./servidor 9002
```
El servidor se quedará esperando conexiones.

Abre una o más terminales nuevas y ejecuta el cliente en cada una. Debes especificar el hostname (si es en la misma máquina, usa localhost o 127.0.0.1) y el mismo puerto que usaste para el servidor.

``` shell
# En la terminal 2
./cliente localhost 9002

# En la terminal 3
./cliente localhost 9002
```
Cada cliente se conectará al servidor y podrás interactuar con el menú de forma independiente. En la terminal del servidor, verás los mensajes que llegan de cada cliente.
