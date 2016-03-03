miniBloq v0.84.0
=======

#DOCUMENTACION EN DESARROLLO

## Descripción

## Compilación

El código de miniBloq usa como toolkit gráfico wxwidgets y tiene sus archivos de compilación hechos para Codeblocks.
El script "source/build.sh" envuelve llamadas a un Makefile que a su vez envuelven la llamada a Codeblocks para compilar, pero sin requerir que se levante este entorno previamente.

El proyecto para compilar correctamente necesita:
    * codeblocks
    * wxwidgets-dev
    * libusb-dev

Cumpliendo estos requisitos no debería hacer más que ejecutar:
    ./build.sh


Minibloq v0.84.0
- Cambios:
	- Soporte exclusivo para Huayra Primaria.
	- DuinoBot v2.3 HID soportado.
	- Uso del toolchain instalado en el sistema.
	- Nueva ruta de instalación.
	- Instalador de updatepacks.
	- Un Makefile para no levantar Codeblocks (aunque lo usa de fondo).
	- Castellano por defecto.

Minibloq v0.83 version. This is the latest wxWidgets based Minibloq version.

- Changes:
    - Integrated AERobot ultra low cost educational robot (http://wyss.harvard.edu/viewpage/539).
	- Added calibration screen for AERobot.
	- Changed the icons by the new Flat Icons designed by Camilo Parra Palacio (https://github.com/cparrapa)
    - Added Russian translation (credits on http://blog.minibloq.org/p/credits.html)
    - Added Japanese translation (credits on http://blog.minibloq.org/p/credits.html)
	- Added updatePack1 (Flat Icons) and updatePack2 (Classic 3D Icons) so the user can install the them he prefers (available here: https://github.com/miniBloq/v0.83/tree/master/updatePacks).

