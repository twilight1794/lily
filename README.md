# lily
Biblioteca en C99 que implementa un metaensamblador, un metadesensamblador, y un
metahipervisor para muchos microprocesadores.

![GitHub commit activity](https://img.shields.io/github/commit-activity/t/twilight1794/lily)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/twilight1794/lily)

## Características
- __Portable:__ escrito en C99, corre tanto en Windows, en Linux, o incluso en tu
  navegador.
- __Extensible:__ añade soporte para un microprocesador describiéndolo con un
  esquema.
- __Fácil de integrar:__ se provee una API sencilla, en C y en JavaScript, para la
  biblioteca compartida.

## Instalación

### Requerimientos
La biblioteca compartida solo depende de liblua5.4. Si usas un sistema
GNU/Linux, el script
[misc/scripts/install-lua-linux](./misc/scripts/install-lua-linux) instalará los
archivos .so y .a necesarios en tu sistema. Si vas a generar una compilación
para web, se recomienda que uses este script en vez de instalar la versión
provista por el administrador de paquetes de tu distribución. El cliente CLI
requiere además libgettext.

### Compilación
Para compilar, necesitas make. La receta tratará de determinar tu sistema
operativo automáticamente, así que en general, bastará con hacer `make` y `make
install`. Para hacer otros ajustes, revisa el Makefile para todas las opciones
disponibles.

## Contribuciones
Son bienvenidas las contribuciones, especialmente si éstas son esquemas de
microprocesador para nuevas arquitecturas, o mejoras de las existentes. Para
contribuciones sobre el núcleo, sería buena abrir una incidencia antes para
discutir los posibles cambios.

Hay qué tener en cuenta una cosa en cuanto al código: se ha de tratar, en la
medida de lo posible, de mantener al mínimo las dependencias necesarias para
construir la biblioteca, para mantener simple la compilación y, el tamaño de los
objetos generados.

## Licencia
Este proyecto es software libre, bajo la licencia [GNU GPL versión
3](https://choosealicense.com/licenses/gpl/) o posterior.  El código fuente en
_misc/examples_, y en _test/_ tienen licencia
[Unlicense](https://choosealicense.com/licenses/unlicense/).
