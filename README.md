# Lily Backend

Este es el motor de procesamiento (backend) para el proyecto **Hexacode**. Está desarrollado en C y utiliza Lua para la definición de arquitecturas.

---

## 🐧 Instalación en Linux (Ubuntu/Debian)

### Requisitos Previos
*   Herramientas de construcción: `build-essential` (incluye `gcc` y `make`)
*   Control de versiones: `git`

### Pasos
1. **Instalar herramientas:** `sudo apt update && sudo apt install -y build-essential git`
2. **Inicializar submódulos:** `git submodule update --init --recursive`
3. **Instalar Lua 5.4:** `sudo ./misc/install-lua.sh`
4. **Compilar:** `mkdir -p dist && make linux`

---

## 🪟 Instalación en Windows (PowerShell/CMD)

Para Windows, el proyecto ha sido adaptado para compilarse usando **MinGW** y **GNU Make**.

### 1. Requisitos Previos (Instalación de Herramientas)
Abre una terminal de PowerShell como administrador e instala las herramientas necesarias usando `winget`:

```powershell
# Instalar compilador GCC (MinGW)
winget install BrechtSanders.WinLibs.POSIX.UCRT

# Instalar GNU Make
winget install GnuWin32.Make
```

*Nota: Es necesario reiniciar la terminal después de la instalación para que los comandos `gcc` y `mingw32-make` sean reconocidos.*

### 2. Preparar Dependencias (Lua 5.4)
Lily requiere Lua 5.4. En Windows, se debe compilar dentro del proyecto:

```powershell
mkdir deps
cd deps
# Descargar Lua (ejemplo con curl)
curl.exe -L -o lua-5.4.7.tar.gz https://www.lua.org/ftp/lua-5.4.7.tar.gz
tar.exe -xzf lua-5.4.7.tar.gz
cd lua-5.4.7
# Compilar Lua para Windows
mingw32-make mingw
cd ../..
```

### 3. Archivos de Compatibilidad (Ya incluidos)
Para que Lily funcione en Windows, se han creado/editado los siguientes archivos (incluidos en esta versión):
*   `src/common/sys/mman.h`: Simula la gestión de memoria de Linux en Windows.
*   `src/common/libintl.h`: Stub para funciones de traducción (gettext) en Windows.
*   `Makefile`: Actualizado para detectar `OS=Windows_NT` y enlazar correctamente `lua54.dll`.

### 4. Compilación del Backend
Desde la raíz del proyecto `lily/`, ejecuta:

```powershell
mkdir dist
mingw32-make windows
# Copiar la DLL de Lua a la carpeta de salida
cp deps/lua-5.4.7/src/lua54.dll dist/lua54.dll
```

---

## 🚀 Cómo Ejecutar (Windows & Linux)

El programa genera un ejecutable en la carptea `dist/`.

### Comandos de Ejecución
Para procesar un archivo assembly (ejemplo `SUMA.asm`):

**En Windows:**
```powershell
./dist/lily.exe -x misc/cpu/z80.lua -s a SUMA.asm
```

**En Linux:**
```bash
LD_LIBRARY_PATH=dist ./dist/lily -x misc/cpu/z80.lua -s a SUMA.asm
```

### Parámetros principales:
*   `-x <archivo.lua>`: Especifica la arquitectura (ej. `misc/cpu/z80.lua`).
*   `-s <etapa>`: Etapa del proceso: `p` (preproceso), `a` (ensamble), `l` (enlace), `d` (desensamble), `e` (ejecución).
*   `-i`: Modo interactivo.

---

## 📂 Visualización de Resultados
Al ejecutar el ensamble (`-s a`), el programa generará un archivo de objeto (ej. `SUMA.o`). Puedes ver el rastro de la ejecución en la consola (logs) para verificar que el análisis léxico y sintáctico fue exitoso.

---
*Nota: El parámetro `-s` debe ir en **minúsculas**.*
