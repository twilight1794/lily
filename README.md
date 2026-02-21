# Lily Backend

Este es el motor de procesamiento (backend) para el proyecto **Hexacode**. Está desarrollado en C y utiliza Lua para la definición de arquitecturas.

## Requisitos Previos

*   Sistema Operativo: Linux
*   Herramientas de construcción: `build-essential` (incluye `gcc` y `make`)
*   Control de versiones: `git`
*   Permisos de administrador (`sudo`) para la instalación de dependencias.

## Pasos de Instalación

Sigue estos pasos en orden para configurar el entorno:

### 1. Instalar Herramientas de Construcción (si no las tienes)
```bash
sudo apt update
sudo apt install -y build-essential git
```

### 2. Inicializar Submódulos
Lily utiliza submódulos para las pruebas unitarias. Ejecuta esto para descargarlos:
```bash
git submodule update --init --recursive
```

### 3. Instalar Dependencias (Lua 5.4)
Lily requiere las bibliotecas de desarrollo de Lua 5.4. Existe un script automatizado que descarga, parchea e instala Lua:

```bash
sudo ./misc/install-lua.sh
```
*Nota: Es normal ver errores relacionados con `emcc` si no tienes Emscripten instalado; estos solo afectan a la versión web, no al backend de Linux.*

### 4. Actualizar el Caché de Bibliotecas
Después de instalar Lua, es necesario que el sistema reconozca la nueva ubicación de los archivos `.so`:

```bash
sudo ldconfig
```

### 5. Compilar el Backend
Para compilar la versión optimizada para Linux:

```bash
mkdir -p dist
make linux
```

### 6. Ejecutar Pruebas (Opcional)
Para verificar que todo funcione correctamente:
```bash
make test-linux
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
