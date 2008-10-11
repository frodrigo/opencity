------------------------------------------------------------------------------
LÉEME para OpenCity 0.0.5stable
------------------------------------------------------------------------------
Última modificación:
	$Id: README_es.txt 294 2007-12-01 14:47:10Z neoneurone $

Orientado a:
	# Usuarios Finales:
		Puedes encontrar bastante información útil en este archivo. ¡Léelo!
	# Desarrolladores:
		Este archivo contiene información útil para las opciones a través de 
		la línea de comandos.
		Para ayuda con la compilación e instalación, por favor, comprueba el
		archivo "INSTALL".

Descripción:
	OpenCity es, otro simulador más de ciudades. La idea es bastante simple: 
	tienes que construir una ciudad con 3 tipos de "zonas":
	Residenciales, Comerciales e Industriales.
	Cada una depende de la otra durante su construcción. Intenta conseguir
	todo lo que necesitas y podrás ver cómo crece tu ciudad.


------------------------------------------------------------------------------
Requisitos de Hardware:
------------------------------------------------------------------------------
Procesador:
	No importa

Velocidad:
	Cualquiera

Tarjeta Gráfica:
	Se necesita una tarjeta aceleradora 3D.

Tarjeta de Sonido:
	Cualquiera que pueda funcionar con "SDL_mixer"

Red:
	Soporte experimental


------------------------------------------------------------------------------
Requisitos de Software:
------------------------------------------------------------------------------
Sistema Operatvo:
	Linux (recomendado)
	o Mac OS o superior,
	o Microsoft Windows XP/2000 o superior,
	o Wine

Interfaz Gráfica de Usuario:
	X Windows con DRI activado (recomendado)
	o Darwin
	o Microsotf Windows
	o Wine

Librerías:
	GL (OpenGL)
	GLU
	SDL
	SDL_image
	SDL_net
	SDL_mixer (opcional)


------------------------------------------------------------------------------
Notas de la publicación del auto-empaquetado para Linux:
------------------------------------------------------------------------------
	¿Has descargado el archivo "opencity-x-y-z_algo.package"?
Dale permiso de ejecución eligiendo el comando "chmod" correcto (consulta
tu distribución para conseguir más información sobre el comando "chmod")
	
	Ahora ábrelo desde la consola o en un navegador de archivos (P.E. Konqueror). 
OpenCity se instalará en tu ordenador, y se creará un icono dentro del grupo
"Juegos".
	
	Después de la instalación, puedes abrir OpenCity pulsando en su icono o 
escribiendo "opencity" en tu consola favorita.


------------------------------------------------------------------------------
Notas de la publicación del binario para Microsoft Windows:
------------------------------------------------------------------------------
	Para poder jugar a OpenCity, debes instalarlo primero. Una vez lo hayas 
	hecho, pulsa en el icono OpenCity en el menú de inicio para comenzar.

------------------------------------------------------------------------------
Descripción de los controles
------------------------------------------------------------------------------
Atajos del teclado:
		n: Herramienta en "Blanco"
		r: Herramienta Zona Residencial
		c: Herramienta Zona Comercial
		i: Herramienta Zona Industrial
		p: Herramienta carretera
		l: Herramienta líneas eléctricas
		x: Herramienta Destruir
		q: Herramienta Consulta
		e: Construir una central eléctrica de carbón
		ctrl: cancelar la acción de la herramienta seleccionada
	
		u / d: Herramienta aumentar / disminuir terreno

Navegación:
		arriba/abajo/izquierda/derecha: mover el mapa
		avpag/repag: Rotar el mapa
		insertar/suprimir: Alejar / acercar Zoom
		alt: Multiplicar los efectos de los botones por 10
	
		h: Inicio
		escape: Activar/desactivar menú principal

Opciones:
		b: Activar/desactivar mostrar edificios
		g: Activar/desactivar mostrar grilla
		k: Activar/desactivar mostrar la barra de estado principal
		f: Activar/desactivar mostrar en modo grilla
		o: Activar/desactivar mostrar proyección perspectiva/octogonal
		t: Activar/desactivar mostrar terreno
		w: Activar/desactivar mostrar agua

Sonido y música:
		</>: reproducir canción anterior/siguiente
		m: Activar/desactivar la música
		s: Activar/desactivar sonidos

Experimental:
		F2: Guardar
		F6: Cargar

Opciones indocumentadas (pruébalas bajo tu propio riesgo):
		a: Menú con varios códigos de prueba
		v: Menú de prueba MAS
		z: Conectar al servidor ZeN local


------------------------------------------------------------------------------
Sonido y música
------------------------------------------------------------------------------
	OpenCity usa "SDL_mixer" para reproducir los sonidos y la música. A sí que,
debería soportar todos los tipos de archivo que admita el "SDL_mixer".

	Si no escuchas ningún sonido, intenta modificar el archivo "sound.m3u" y
configura el directorio acorde con tu archivo ejecutable de OpenCity.

	Por favor, ten en cuenta que OpenCity automáticamente añade un "prefijo" a
todos los nombres de los archivos de audio. El prefijo es el directorio donde 
el proceso de instalación haya guardado los archivos de OpenCity.

	Sin embargo, puedes poner un archivo llamado "music.m3u" el la carpeta 
"music", y OpenCity cargará y reproducirá tu música favorita. Si no encuentras
ninguna carpeta con el nombre "music" créala y pon tu lista de reproducción 
(con el nombre "music.m3u") en él.

	Me gustaría advertirte de que SDL_mixer no es extremadamente estable. Puede
"colgar" OpenCity mientras reproduce algunos tipos de archivos ".mp3". Pero 
ninguno de los tipos que crearon "SDL_mixer" soy yo!! :P. Si te da problemas, tan
sólo apaga la música (tecla 'm').


------------------------------------------------------------------------------
Acciones del ratón
------------------------------------------------------------------------------
	En OpenCity, en cualquier lado donde pulses el BDR (Botón Derecho del Ratón),
abrirás el menú de herramientas circular. Pulsa el BDR de nuevo para cerrarlo.
Estamos intentando hacer todo más simple y sencillo pero algunas veces necesitarás
el teclado.

	¡OpenCity también soporta la rueda del ratón! Hazla girar hacia delante para 
acercar el zoom (tecla 'insertar') y gírala hacia atrás para alejar el
zoom (tecla 'suprimir').

	Cuando quedes pulsado la tecla 'Mayúsculas' y juegues con la rueda del ratón 
te darás cuenta de que el mapa se desliza en alguna dirección. Pulsando la tecla
'Ctrl' desliza el mapa en la otra dirección.


------------------------------------------------------------------------------
Archivo de configuración principal "opencity.xml"
------------------------------------------------------------------------------
	Mira los comentarios dentro del archivo para obtener más detalles.


------------------------------------------------------------------------------
Opciones de la línea de comandos
------------------------------------------------------------------------------
	En OpenCity, puedes sobrescribir las opciones del archivo de configuración
con las opciones de la línea de comandos. Algunas opciones están disponibles
tanto en método abreviado como en completo.

	-fs | --full-screen
	Habilita el modo pantalla completa

	-glv | --gl-version
	Muestra la información OpenGL de tu máquina y cierra

	-dd | --data-dir newDataPath
	Especifica la carpeta con los datos de OpenCity. Por ejemplo:
		Bajo Linux: --data-dir /home/foo/bar/share/opencity/
		Bajo Win32: --data-dir "C:/Archivos de programa/OpenCity/share"

	-cd | --conf-dir newConfigPath
	Especifica la carpeta con la configuración de OpenCity. Por ejemplo:
		Bajo Linux: --conf-dir /home/foo/bar/etc/opencity/
		Bajo Win32: --conf-dir "C:/Archivos de programa/OpenCity/etc"

	-na | --no-audio:
	Deshabilita el sistema de audio

	--generator-seed seed
	La semilla que será usada para generar el mapa

	--generator-map MAP-TYPE
		Tipo de terreno     : 0=plano (defecto), 1=colina, 2=montaña

	--generator-water WATER-TYPE
		Tipo de agua        : 0=seco, 1=lago (defecto), 2=costa

	--generator-map-shape MAP-SHAPE-TYPE
		Tipo de forma       : 0=ninguna (defecto), 1=isla, 2=volcán, 3=crater

	--generator-tree-density TREE-DENSITY-TYPE
		Densidad de árboles : 0=sparse (default), 1=normal, 2=dense

	--generator-height-map heightMapPicture
	Carga una imagen a escala de grises (PNG) como mapa de altura.
El nivel del agua es gris (128), cada nivel de mapa es un nivél de gris, es por
ello que la imagen completa debe estar en un gris medio, ya que sólo hay una
pequeña diferencia entre los píxeles. Puedes dibujar tu mismo el mapa de altura
o generarlo desde cualquier fuente de mapas. Esto puede ser interesante para
trabajar con datos DEM (de satélite). El generador de mapas de OpenCity 
cortará la imagen si es demasiado grande.


------------------------------------------------------------------------------
Dudas de Guardar/Cargar
------------------------------------------------------------------------------
	La siguiente carpeta es usada para guardar y cargar:

		* GNU/Linux:
			$HOME/.OpenCity/

		* Windows:
			C:\Documents and Settings\nombreusuario\Application Data\OpenCity\
			Si estás ejecutando una versión localizada (en otro idioma) de 
			Windows la dirección de tu carpeta podría ser diferente

		* Windows Vista:
			C:\Users\username\AppData\Roaming\OpenCity\


------------------------------------------------------------------------------
Solución de problemas
------------------------------------------------------------------------------
	1) Tengo una tarjeta gáfica ATI Radeon 9000 M en mi PC.
Cuando he probado el juego con la opción "EnablePageFlip" (mirar manual radeon)
el juego funcionaba mal. si encuentras el mismo problema, intenta deshabilitar 
esta opción en el servidor XFree (ver manual XF86Config)

	2) Con la tarjeta gráfica ATI Radeon 9000M, SmoothVision(tm) debe ser 
desactivado para poder jugar correctamente a OpenCity.

	3) Aquí está la versión de mi servidor X

 - XFree86 Versión 4.3.0
 - Fecha de lanzamiento: 9 May 2003
 - X Protocol Version 11, Revision 0, Release 6.6
 - Build Operating System: Linux 2.4.18-23mdksmp i686 [ELF]
 - Build Date: 10 September 2003

	Parece que el driver para mi ATI Radeon 9000 M tiene muchos fallos. Si
experimentas estos problemas, intenta escribir esto en la consola (bash)
antes de lanzar OpenCity.
		"export R200_NO_VTXFMT="

	O deberías probar una nueva versión de XFree.


------------------------------------------------------------------------------
Duong-Khang (neoneurone) NGUYEN
Traducción al Español by: Cristian Deluxe (djcristiandeluxe at gmail.com)