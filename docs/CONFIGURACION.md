# Guía de Configuración del Entorno

## Opción 1: GitHub Codespaces

### Requisitos
- Cuenta de GitHub
- Acceso a GitHub Codespaces (disponible en planes de pago o para estudiantes)

### Pasos para configurar:

1. Crear un archivo `.devcontainer/devcontainer.json` en la raíz del proyecto:
```json
{
    "name": "C Development Environment",
    "image": "mcr.microsoft.com/devcontainers/cpp:1",
    "features": {
        "ghcr.io/devcontainers/features/common-utils:2": {
            "installZsh": true,
            "configureZshAsDefaultShell": true,
            "installOhMyZsh": true,
            "upgradePackages": true
        }
    },
    "customizations": {
        "vscode": {
            "extensions": [
                "ms-vscode.cpptools",
                "ms-vscode.cmake-tools"
            ]
        }
    },
    "postCreateCommand": "sudo apt-get update && sudo apt-get install -y gcc make"
}
```

2. Crear un archivo `.devcontainer/Dockerfile`:
```dockerfile
FROM mcr.microsoft.com/devcontainers/cpp:1

# Instalar dependencias adicionales
RUN apt-get update && apt-get install -y \
    gcc \
    make \
    && rm -rf /var/lib/apt/lists/*

# Configurar el directorio de trabajo
WORKDIR /workspace
```

3. Para iniciar el Codespace:
   - Ve a tu repositorio en GitHub
   - Haz clic en el botón "Code"
   - Selecciona la pestaña "Codespaces"
   - Haz clic en "Create codespace on main"

## Opción 2: Docker Local

### Requisitos
- Docker instalado en tu sistema
- Git instalado

### Pasos para configurar:

1. Crear un archivo `Dockerfile` en la raíz del proyecto:
```dockerfile
FROM ubuntu:22.04

# Evitar interacciones durante la instalación
ENV DEBIAN_FRONTEND=noninteractive

# Instalar dependencias
RUN apt-get update && apt-get install -y \
    gcc \
    make \
    git \
    && rm -rf /var/lib/apt/lists/*

# Configurar el directorio de trabajo
WORKDIR /app

# Copiar los archivos del proyecto
COPY . .

# Compilar los programas
RUN gcc -o servidor servidor.c
RUN gcc -o cliente cliente.c

# Exponer el puerto que usará el servidor
EXPOSE 5000

# Comando por defecto
CMD ["./servidor", "5000"]
```

2. Crear un archivo `docker-compose.yml`:
```yaml
version: '3'
services:
  servidor:
    build: .
    ports:
      - "5000:5000"
    command: ./servidor 5000
    volumes:
      - .:/app
    networks:
      - app-network

  cliente1:
    build: .
    command: ./cliente servidor 5000
    volumes:
      - .:/app
    networks:
      - app-network
    depends_on:
      - servidor

  cliente2:
    build: .
    command: ./cliente servidor 5000
    volumes:
      - .:/app
    networks:
      - app-network
    depends_on:
      - servidor

networks:
  app-network:
    driver: bridge
```

### Instrucciones de uso con Docker:

1. Construir la imagen:
```bash
docker build -t cliente-servidor .
```

2. Ejecutar el servidor:
```bash
docker run -p 5000:5000 cliente-servidor ./servidor 5000
```

3. Ejecutar el cliente (en otra terminal):
```bash
docker run --network host cliente-servidor ./cliente localhost 5000
```

### Instrucciones de uso con Docker Compose:

1. Iniciar todos los servicios:
```bash
docker-compose up
```

2. Para ejecutar solo el servidor:
```bash
docker-compose up servidor
```

3. Para ejecutar solo un cliente:
```bash
docker-compose up cliente1
```

## Notas Importantes

### Para GitHub Codespaces:
- El entorno ya viene con todas las herramientas necesarias
- Puedes usar múltiples terminales dentro del Codespace
- Los puertos se exponen automáticamente
- El código se sincroniza automáticamente con tu repositorio

### Para Docker:
- Asegúrate de que el puerto 5000 no esté en uso
- Los contenedores se comunican a través de la red Docker
- Los cambios en el código requieren reconstruir la imagen
- Puedes usar `docker-compose` para manejar múltiples clientes

## Solución de Problemas

### GitHub Codespaces:
1. Si el Codespace no inicia:
   - Verifica tu conexión a internet
   - Asegúrate de tener suficientes créditos de Codespaces

2. Si los puertos no se exponen:
   - Verifica la configuración de puertos en Codespaces
   - Asegúrate de que el servidor está escuchando en el puerto correcto

### Docker:
1. Si el contenedor no inicia:
   - Verifica los logs con `docker logs <container_id>`
   - Asegúrate de que el puerto 5000 está disponible

2. Si los contenedores no se comunican:
   - Verifica que están en la misma red Docker
   - Asegúrate de que el servidor está escuchando en `0.0.0.0`

3. Si hay problemas de permisos:
   - Ejecuta los contenedores con `--privileged` si es necesario
   - Verifica los permisos de los archivos en el volumen 