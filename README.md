# Lily Backend

Este es el motor de procesamiento (backend) para el proyecto **Hexacode**. Está desarrollado en C y utiliza Lua para la definición de arquitecturas.

## Requisitos Previos

*   Sistema Operativo: Linux
*   Compilador: `gcc`
*   Herramienta: `make`
*   Permisos de administrador (`sudo`) para la instalación de dependencias.

## Pasos de Instalación

Sigue estos pasos en orden para configurar el entorno:

### 1. Instalar Dependencias (Lua)
Lily requiere las bibliotecas de desarrollo de Lua 5.4. Existe un script automatizado para esto:

```bash
sudo ./misc/install-lua.sh
```

### 2. Actualizar el Caché de Bibliotecas
Después de instalar Lua, es necesario que el sistema reconozca la nueva ubicación de los archivos `.so`:

```bash
sudo ldconfig
```

### 3. Compilar el Backend
Para compilar la versión optimizada para Linux:

```bash
mkdir -p dist
make linux
```

---

## Cómo Ejecutar

Para que el programa funcione, debe poder localizar su propia biblioteca compartida (`liblily.so`) que se encuentra en la carpeta `dist`.

### Opción A: Ejecución Directa (Recomendado)
```bash
LD_LIBRARY_PATH=dist ./dist/lily [parámetros] <archivo.asm>
```

### Opción B: Configurar la ruta para la sesión actual
Si vas a realizar muchas pruebas, puedes ejecutar esto una sola vez en tu terminal:
```bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/dist
```
Luego podrás ejecutarlo simplemente como:
```bash
./dist/lily [parámetros] <archivo.asm>
```

## Ejemplo de Uso

Para ensamblar un archivo:
```bash
LD_LIBRARY_PATH=dist ./dist/lily -s a mi_archivo.asm
```

Para entrar al modo interactivo:
```bash
LD_LIBRARY_PATH=dist ./dist/lily -i mi_archivo.asm
```

---
*Nota: El parámetro `-s` para especificar la etapa (stage) debe ir en **minúsculas** (`p`, `a`, `l`, `d`, `e`).*
