------------------------------------------------------------------------------
README per OpenCity 0.0.6dev
------------------------------------------------------------------------------
Last modified:
	$Id$
Last translated:
	2008/04/11 morpheus

Destinatari:
	# Utenti finali:
		Potrete trovare molte utili informazioni in questo file. Leggetelo!
	# Packagers:
		Questo file contiene informazioni utili sulle opzioni della linea di comando.
		Per le note su compilazione e installazione leggete il file INSTALL.

Descrizione:
	Questo è un simulatore di città. L'idea è semplice: devi costruire
una città con 3 tipi di "zone", residenziale, commerciale e industriale.
Durante il loro sviluppo le une dipendono dalle altre.
Prova a fornir loro ciò che serve e vedrai crescere la tua città.


------------------------------------------------------------------------------
INDICE
------------------------------------------------------------------------------
1)	Requisiti hardware
2)	Requisiti software
3)	Note sull'autopackage per Linux
4)	Note sui files binari per Windows
5)	Descrizione dei tasti
6)	Suono & musica
7)	Comandi del mouse
8)	File di configurazione principale "opencity.xml"
9)	Opzioni della linea di comando
10)	Mappe altimetriche
11)	Note sul salvataggio / caricamento
12)	Problemi noti


------------------------------------------------------------------------------
1) Requisiti hardware
------------------------------------------------------------------------------
Processore:		non importa

Velocità:		qualunque

Scheda video:		qualunque scheda con accellerazione 3D

Scheda audio:		qualunque scheda sia supportata da SDL_mixer

Rete:			supporto sperimentale


------------------------------------------------------------------------------
2) Requisiti software
------------------------------------------------------------------------------
Sistema operativo:
	Linux
	oppure Mac OS
	oppure Win32
	oppure Wine

Interfaccia grafica dell'utente:
	X Windows con DRI attivo (raccomandato)
	oppure Darwin
	oppure MS Windows
	oppure Wine

Librerie:
	GL (OpenGL)
	GLU
	SDL
	SDL_image
	SDL_net
	SDL_mixer (opzionale)


------------------------------------------------------------------------------
3) Note sull'autopackage per Linux
------------------------------------------------------------------------------
	Hai appena scaricato "opencity-x.y.zqualcosa.package"?
Abilita il permesso di esecuzione del file usando correttamente il comando "chmod"
(fai riferimento alla tua distribuzione Linux per altre informazioni sul comando).

	Adesso eseguilo dalla console o da un file browser come Konqueror. Opencity
verrà installato sul tuo computer e un'icona verrà creata all'interno del gruppo "Game".

	Dopo l'installazione, potrai avviare OpenCity cliccando sull'icona o
scrivendo "opencity" in un terminale.


------------------------------------------------------------------------------
4) Note sui files binari per Windows
------------------------------------------------------------------------------
	Prima di giocare, è necessario installare il gioco. Una volta fatto,
clicca sull'icona di OpenCity nel menù start per iniziare a giocare.


------------------------------------------------------------------------------
5) Descrizione dei tasti
------------------------------------------------------------------------------
Scorciatoie da tastiera:
		n:				strumento "vuoto"
		r:				zona residenziale
		c:				zona commerciale
		i:				zona industriale
		p:				strada
		l:				traliccio dell'alta tensione
		e: 				centrale nucleare
		x:				distruggi
		q:				interroga
		ctrl:			elimina l'azione dei tasti zona
		u / d:			alza / abbassa terreno

Esplorazione:
		su / giù / sinistra / destra:	scorri la mappa
		pagina sù / pagina giù:			ruota la mappa
		insert / delete:				zoom in / out
		alt:							moltiplica per 10 l'effetto del comando
		h:								torna alla cella iniziale
		escape:							menu principale

Opzioni:
		b:				costruzioni on / off
		g:				griglia on / off
		k:				barra di stato on / off
		f:				modalità wire frame on / off
		o:				prospettiva / proiezione ortogonale
		t:				terreno on / off
		w:				acqua on / off

Suono & musica:
		< / >:			riproduci brano percedente / successivo
		m:				musica on / off
		s:				suono on / off

Sperimentale:
		F2:				salva
		F6:				carica

Caratteristiche non documentate, provale a tuo richio:
		a: 				vari menù di test
		v:				menù di test MAS
		z:				connetti al server ZeN locale


------------------------------------------------------------------------------
6) Suono & musica
------------------------------------------------------------------------------
	OpenCity usa SDL_mixer per riprodurre suoni e musica, quindi supporta
tutti i formati che supporta SDL_mixer.

	Se non senti alcun suono prova a modificare il file "sound.m3u" e
setta il percorso conformemente all'eseguibile "opencity". Nota che OpenCity
mette automaticamente un prefisso a tutti i nomi dei files audio.
Il prefisso equivale alla posizione dei dati di OpenCity dopo l'installazione.

	In ogni caso puoi mettere un file chiamato "music.m3u" nella cartella "music"
e OpenCity riprodurrà la tua musica preferita. Se non trovi nessuna cartella
"music" puoi crearla e metterci dentro il file "music.m3u".

	Ti avviso che SDL_mixer non è stabile e potrebbe bloccare OpenCity
mentre riproduce certi file ".mp3". Prenditela con quelli di SDL_mixer,
non con me ;) . Se rilevi problemi disabilita la musica (tasto 'm').


------------------------------------------------------------------------------
7) Comandi del mouse
------------------------------------------------------------------------------
	In OpenCity, quando clicchi il tasto destro del mouse, apri l'ultimo
toolcircle [menù a forma di cerchio, NdT]. Clicca di nuovo per nasconderlo.
Voglio rendere tutto semplice e facile da usare, ma ogni tanto hai ancora bisogno
della tastiera.

	OpenCity supporta anche la rotella del mouse! Se la ruoti in avanti
zoommerai in avanti (come con il tasto 'Insert'), altrimenti zoommerai indietro
(come con il tasto 'Delete').

	Quando premi 'Shift' e ruoti la rotella del mouse noterai che la mappa
si sposterà in una direzione. Se premi 'Ctrl' la mappa si sposterà lungo un altro asse.


------------------------------------------------------------------------------
8) File di configurazione principale "opencity.xml"
------------------------------------------------------------------------------
	Vedi i commenti in-line per maggiori informazioni.


------------------------------------------------------------------------------
9) Opzioni della linea di comando
------------------------------------------------------------------------------
	In OpenCity, puoi bypassare le opzioni del file di configurazione
con le opzioni a riga di comando. Alcune opzioni possono essere espresse
sia in forma estesa sia in forma compatta.

-fs | --fullscreen
	Abilita la modalità a pieno schermo.

-glv | --gl-version
	Mostra informazioni sull'implementazione di OpenGL della tua macchina ed esce.

-dd | --datadir newDataPath
	Specifica una nuova cartella per i dati di OpenCity.
	Per esempio:
	Sotto linux: --datadir /home/foo/bar/share/opencity/"
	Sotto win32: --datadir "C:/Programmi/OpenCity/share"

-cd | --confdir newConfigPath
	Specifica una nuova cartella per la configurazione di OpenCity.
	Per esempio:
	Sotto linux: --confdir /home/foo/bar/etc/opencity/"
	Sotto win32: --confdir "C:/Programmi/OpenCity/etc"

-na | --no-audio	
	Disabilita l'audio.

--generator-seed seed	
	Il seed è un numero intero casuale usato dal generatore di mappe.

--generator-map MAP-TYPE
	MAP-TYPE: 0 = pianura (default), 1 = collina, 2 = montagna

--generator-water WATER-TYPE
	WATER-TYPE: 0 = arido, 1 = lago (default), 2 = costa

--generator-map-shape MAP-SHAPE-TYPE
	MAP-SHAPE-TYPE: 0 = nessuno (default), 1 = isola, 2 = vulcano, 3 = cratere

--generator-tree-density TREE-DENSITY-TYPE
	TREE-DENSITY-TYPE: 0 = scarsa (default), 1 = normale, 2 = densa

--generator-height-map heightMapPicture
	Carica un'immagine a scala di grigi per la mappa (PNG). Il livello
	dell'acqua è un grigio a livello 128, ogni livello superiore è un
	livello di grigio in più. Per questo motivo l'immagine sembra
	totalmente grigia, perchè c'è solo una minima differenza tra i pixels.
	Puoi disegnare tu stesso la mappa o generarla partendo dalla sorgente
	che vuoi. Dei dati interessanti per sfruttare questa funzionalità sono
	i dati satellitari DEM. Il generatore di mappe di OpenCity taglierà
	l'immagine se è troppo grande.


------------------------------------------------------------------------------
10) Mappe altimetriche
------------------------------------------------------------------------------
	OpenCity è distribuito con alcune mappe altimetriche generate da
dati satellitari. Questa è la lista completa:

	Athenes-1024x1024.png
	Athenes-51x51.png
	Dakar-51x51.png
	Ibiza-101x101.png
	Istanbul-1025x1025.png
	Mallorca-93x61.png
	Palermo-51x51.png
	Sao_Tome-101x101.png
	Strasbourg-1025x1025.png
	Woodstock-51x51.png

	Per caricare queste mappe, devi usare un'opzione della linea di comando.
Per esempio:

	opencity --generator-height-map map/heightmap/Athenes-51x51.png

	Attualmente, il caricamento di queste mappe è una caratteristica ancora
sperimentale, provala a tuo richio e pericolo.


------------------------------------------------------------------------------
11) Note sul salvataggio / caricamento 
------------------------------------------------------------------------------
	La seguente directory è usata per salvare/caricare

GNU/Linux:		$HOME/.OpenCity/
Windows 2000/XP:	C:\Documents and Settings\username\Dati applicazioni\OpenCity\
Windows Vista:		C:\Utenti\username\AppData\Roaming\OpenCity\

	Se esegui una versione localizzata di Windows il percorso potrebbe
essere diverso.


------------------------------------------------------------------------------
12) Problemi noti
------------------------------------------------------------------------------
	1) Ho una ATI Radeon 9000M. Quando provo il gioco con "EnablePageFlip"
(vedi man radeon) il gioco viene eseguito stranamente. Se riscontri lo stesso
problema prova a disabilitare questa funzionalità del server Xfree
(vedi man XF86Config)

	2) Con la ATI Radeon 9000M bisogna disattivare SmoothVision(tm) per
eseguire OpenCity correttamente.

	3) Questa è la mia versione del server X

 - XFree86 versione 4.3.0
 - Data di rilascio: 9 Maggio 2003
 - X Protocol versione 11, revisione 0, release 6.6
 - Build del sistema operativo: Linux 2.4.18-23mdksmp i686 [ELF]
 - Data di build: 10 Settembre 2003

	Sembrerebbe che i driver per la mia ATI Radeon 9000M siano pieni di bugs.
Se dovessi avere problemi prova a scrivere quanto sotto in una shell (bash) prima di giocare.
		"export R200_NO_VTXFMT="

	Se non funziona puoi provare con una nuova versione di XFree.


------------------------------------------------------------------------------
www.opencity.info

Duong-Khang (neoneurone) NGUYEN

Translated by Jacopo (morpheus) SIRIANNI
