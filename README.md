# lily
Biblioteca en C99 que implementa un metaensamblador, un metadesensamblador, y un
metahipervisor para muchos microprocesadores.

![GitHub commit
!activity](https://img.shields.io/github/commit-activity/t/twilight1794/lily)
![GitHub code size in
!bytes](https://img.shields.io/github/languages/code-size/twilight1794/lily)

## Características
- __Portable:__ escrito en C99, corre tanto en Windows, en GNU/Linux, o incluso
  en tu navegador.
- __Extensible:__ añade soporte para un microprocesador describiéndolo con un
  esquema.
- __Libre:__ es software libre, puedes usarlo como quieras, e incluso
distribuirlo y modificarlo sin otra condición que conservar la misma licencia.
- __Fácil de integrar:__ se provee una API sencilla, en C y en JavaScript, para
  la biblioteca compartida.

## Instalación

### Requerimientos
La biblioteca compartida solo depende de liblua5.4.8. Si usas un sistema de
tipoUnix, el script [misc/scripts/install-lua](./misc/scripts/install-lua)
instalará los archivos de biblioteca necesarios en tu sistema. Si vas a generar
una compilación para web, se recomienda que uses este script en vez de instalar
la versión provista por el administrador de paquetes de tu distribución. El
cliente CLI requiere además libgettext.

### Compilación
Para compilar, necesitas make. La receta tratará de determinar tu sistema
operativo automáticamente, así que en general, bastará con hacer `make` y `make
install`. Para hacer otros ajustes, revisa el [Makefile](./Makefile) para todas
las opciones disponibles.

## Contribuciones
Son bienvenidas las contribuciones, especialmente si éstas son esquemas de
microprocesador para nuevas arquitecturas, o mejoras de las existentes. Para
contribuciones sobre el núcleo, sería buena abrir una incidencia antes para
discutir los posibles cambios.

Hay qué tener en cuenta una cosa en cuanto al código: se ha de tratar, en la
medida de lo posible, de mantener al mínimo las dependencias necesarias para
construir la biblioteca, para mantener simple la compilación y, el tamaño de los
objetos generados.

## Motivaciones
Este proyecto esta pensado para ser la base independiente de un proyecto PAPIME,
[HexaCode](https://mmedia1.fi-b.unam.mx/hexacode), que tiene como propósito
ofrecer un entorno de desarrollo moderno y amigable (aunque no por ello simple)
principalmente para todos los alumnos de las asignaturas de _Estructura y
programación de computadoras_. Mientras que HexaCode no es un programa libre,
Lily sí lo es, porque por diversas razones, su autor considera que el software
utilizado en la educacion debe ser libre (vease [Por qué las escuelas deben usar
exclusivamente software
libre](https://www.gnu.org/education/edu-schools.es.html).

## Licencia
Este proyecto es software libre, bajo la licencia [GNU GPL versión
3](https://choosealicense.com/licenses/gpl/) o posterior.  El código fuente en
_misc/asm_, y en _test/_ tienen licencia
[Unlicense](https://choosealicense.com/licenses/unlicense/). La documentación en
_doc/_ tiene licencia [GNU Free Documentation License
1.3](https://www.gnu.org/licenses/fdl-1.3.html) o posterior, y la licencia
[Creative Commons Atribución-CompartirIgual
4.0](https://creativecommons.org/licenses/by-sa/4.0/deed.es) o posterior.
