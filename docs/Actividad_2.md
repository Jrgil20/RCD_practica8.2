# Actividad 2 — Servidor con múltiples clientes y menú interactivo

## Objetivo
Implementar un servidor que pueda manejar múltiples clientes (mínimo 2) con un menú interactivo.

## Interacción esperada
El cliente se conecta y puede realizar las siguientes acciones:

1. Enviar mensaje
2. Escanear carpeta
3. Desconectarse

### Funcionalidad de Escaneo de Carpeta
- El cliente abre la carpeta actual usando `opendir()` y `readdir()` con `<dirent.h>`
- Cuenta los archivos y envía un mensaje tipo "5—Escanear Carpeta"
- Envía la lista de nombres de archivo

### Funcionalidad del Servidor
- Debe identificar de qué cliente viene cada mensaje
- Responde el mismo mensaje
- Muestra los mensajes por consola

## Componentes esenciales

El servidor:

### Servidor
1. Crear el socket con `socket()`
2. Llenar estructura `sockaddr_in` con IP y puerto
3. Usar `bind()` para asociar socket a puerto
4. Usar `listen()` para esperar conexiones
5. Implementar manejo de múltiples clientes usando `fork()` o `pthread`
6. Para cada cliente:
   - Aceptar conexión con `accept()`
   - Identificar cliente usando `inet_ntoa(cli_addr.sin_addr)`
   - Leer mensaje con `read()` o `recv()`
   - Responder con `write()`
   - Cerrar sockets con `close()`

### Cliente
1. Crear socket con `socket()`
2. Obtener IP del servidor con `gethostbyname()`
3. Llenar estructura `sockaddr_in`
4. Usar `connect()` al servidor
5. Implementar menú interactivo
6. Manejar las tres opciones:
   - Enviar mensaje
   - Escanear carpeta
   - Desconectarse
7. Cerrar socket

## Recomendación de implementación
- Usar `fork()` o `pthread` en el servidor para manejar múltiples clientes
- Cada cliente debe manejarse en un hilo/proceso separado
- Para identificar al cliente, usar su socket o dirección IP con `inet_ntoa(cli_addr.sin_addr)`

🛠 Recomendación de implementación:
Usa fork() o pthread en el servidor para manejar múltiples clientes (cada cliente en un hilo/proceso).

Para identificar al cliente, usa su socket o dirección IP con inet_ntoa(cli_addr.sin_addr).

