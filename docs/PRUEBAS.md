# Guía de Pruebas - Cliente-Servidor TCP

## Requisitos Previos
- Sistema operativo Linux
- Compilador GCC instalado
- Terminal con soporte para múltiples ventanas

## Compilación

1. Compilar el servidor:
```bash
gcc -o servidor servidor.c
```

2. Compilar el cliente:
```bash
gcc -o cliente cliente.c
```

## Pruebas de la Actividad 1 - Cliente-Servidor Básico

### Pasos para probar:

1. Abrir una terminal y ejecutar el servidor:
```bash
./servidor 5000
```
Deberías ver el mensaje: "Servidor esperando conexiones en el puerto 5000..."

2. Abrir otra terminal y ejecutar el cliente:
```bash
./cliente localhost 5000
```
Deberías ver el mensaje: "Conectado al servidor. ¡Listo para enviar mensajes!"

3. En la terminal del cliente, seleccionar la opción 1 y escribir un mensaje.

4. Verificar que:
   - El servidor muestra el mensaje recibido
   - El cliente recibe la respuesta del servidor

## Pruebas de la Actividad 2 - Múltiples Clientes

### Pasos para probar:

1. Mantener el servidor ejecutándose en la primera terminal.

2. Abrir dos terminales adicionales y ejecutar el cliente en cada una:
```bash
./cliente localhost 5000
```

3. En cada cliente, probar las tres opciones:

   a. **Opción 1 - Enviar mensaje:**
   - Seleccionar opción 1
   - Escribir un mensaje
   - Verificar que el servidor muestra el mensaje con la IP del cliente
   - Verificar que el cliente recibe la respuesta

   b. **Opción 2 - Escanear carpeta:**
   - Seleccionar opción 2
   - Verificar que el servidor muestra la lista de archivos
   - Verificar que el cliente muestra el mensaje de envío

   c. **Opción 3 - Desconectar:**
   - Seleccionar opción 3
   - Verificar que el cliente se desconecta
   - Verificar que el servidor muestra el mensaje de desconexión

### Comportamiento esperado:

1. **Servidor:**
   - Muestra "Servidor esperando conexiones en el puerto 5000..."
   - Para cada cliente conectado, muestra su IP y puerto
   - Muestra los mensajes recibidos con la IP del cliente
   - Muestra la lista de archivos cuando se recibe el comando de escaneo
   - Muestra mensajes de desconexión

2. **Cliente:**
   - Muestra el menú de opciones
   - Permite enviar mensajes y recibir respuestas
   - Muestra el progreso del escaneo de carpeta
   - Se desconecta limpiamente al seleccionar la opción 3

## Verificación de Requisitos

### Actividad 1:
- [ ] El servidor acepta conexiones
- [ ] El cliente puede conectarse
- [ ] Los mensajes se envían y reciben correctamente
- [ ] La comunicación es bidireccional

### Actividad 2:
- [ ] El servidor maneja múltiples clientes simultáneamente
- [ ] Cada cliente puede enviar mensajes independientemente
- [ ] El escaneo de carpeta funciona correctamente
- [ ] Los clientes pueden desconectarse limpiamente
- [ ] El servidor identifica correctamente a cada cliente

## Solución de Problemas

1. **Error "Address already in use":**
   - Esperar unos segundos antes de volver a ejecutar el servidor
   - O usar un puerto diferente

2. **Error de conexión:**
   - Verificar que el servidor está ejecutándose
   - Verificar que el puerto es correcto
   - Verificar que se usa "localhost" o la IP correcta

3. **Error de permisos:**
   - Asegurarse de que los archivos tienen permisos de ejecución:
   ```bash
   chmod +x servidor cliente
   ``` 