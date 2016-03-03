miniBloq v0.84.0
=======

#Advertencia: Esta documentación está en desarrollo.

## Descripción

## Compilación

El proyecto para compilar correctamente necesita:
	
  * codeblocks
  * wxwidgets-dev
  * libusb-dev

Cumpliendo estos requisitos no debería hacer más que ejecutar:

```bash
cd source
./build.sh

```

(El script "build.sh" envuelve llamadas a un Makefile que a su 
vez envuelven la llamada a Codeblocks para compilar, pero sin requerir 
que se levante este entorno previamente.)

##Cambios

Minibloq v0.84.0

  * Soporte exclusivo para Huayra Primaria.
  * DuinoBot v2.3 HID soportado.
  * Uso del toolchain instalado en el sistema.
  * Nueva ruta de instalación.
  * Instalador de updatepacks.
  * Un Makefile para no levantar Codeblocks (aunque lo usa de fondo).
  * Castellano por defecto.
