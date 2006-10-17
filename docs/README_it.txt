------------------------------------------------------------------------------
LEGGIMI per OpenCity 0.0.5beta
------------------------------------------------------------------------------
Last modified:
	$Id: README,v 1.26 2006/01/26 10:53:18 neoneurone Exp $
Last translated:
	2006/10/10 morpheus

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
6)	Autoscrolling
7)	Suono & musica
8)	Comandi del mouse
9)	File di configurazione principale "opencity.conf"
10)	Opzioni della linea di comando
11)	Note sul salvataggio / caricamento
12)	Problemi noti


------------------------------------------------------------------------------
1) Requisiti hardware
------------------------------------------------------------------------------
Processore:
	non importa

Velocità:
	qualunque

Scheda video:
	qualunque scheda con accellerazione 3D

Scheda audio:
	qualunque scheda sia supportata da SDL_mixer

Rete:
	supporto sperimentale


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
	Hai appena scaricato "qualcosa.package"? Installa OpenCity eseguendolo
dalla console o in un file browser come Konqueror. Opencity verrà installato
sul tuo computer e un'icona verrà creata all'interno del gruppo "Game".


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
		ctrl:				elimina l'azione dei tasti zona
		u / d:				alza / abbassa terreno

Esplorazione:
		su / giù / sinistra / destra:	scorri la mappa
		pagina sù / giù:		ruota la mappa
		insert / delete:		zoom in/out
		alt:				moltiplica per 10 l'effetto del comando
		h:				torna alla cella iniziale
		escape:				esci

Opzioni:
		g:				griglia on / off
		k:				bussola on / off
		f:				modalità wire frame on / off
		o:				prospettiva / proiezione ortogonale

Suono & musica:
		z / b:				riproduci brano percedente / successivo
		m:				musica on / off
		s:				suono on / off

Sperimentale:
		F2:				salva
		F6:				carica

Caratteristiche non documentate, provale a tuo richio:
		a: 				vari menù di test
		v:				menù di test MAS
		t:				connetti al server ZeN locale



------------------------------------------------------------------------------
6) Autoscrolling
------------------------------------------------------------------------------
	Un semplice sistema di autoscrolling (auto scorrimento) è stato incluso
in OpenCity. Basta portare il mouse vicino ai bordi della finestra e OpenCity
scorrerà automaticamente la mappa.

	É stato implementato anche l'autorotazione. Porta il cursore
negli angoli superiori destro e sinistro per vedere come funziona.

	Per il momento non puoi disabilitarli. Lo so, non a tutti piacciono
gli automatismi.



------------------------------------------------------------------------------
7) Suono & musica
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
non con me :) . Se rilevi problemi disabilita la musica (tasto 'm').


------------------------------------------------------------------------------
8) Comandi del mouse
------------------------------------------------------------------------------
	A partire da OpenCity 0.0.2, quando clicchi il tasto destro del mouse,
apri l'ultimo toolcircle (menù a forma di cerchio). Clicca di nuovo per nasconderlo.
Voglio rendere tutto semplice e facile da usare, ma ogni tanto hai ancora bisogno
della tastiera.

	OpenCity adesso supporta la rotella del mouse! Se la ruoti in avanti
zoommerai in avanti (come con il tasto 'Insert'), altrimenti zoommerai indietro
(come con il tasto 'Delete').

	Quando premi 'Shift' e ruoti la rotella del mouse noterai che la mappa
si sposterà in una direzione. Se premi 'Ctrl' la mappa si sposterà nell'altro senso.



------------------------------------------------------------------------------
9) File di configurazione principale "opencity.conf"
------------------------------------------------------------------------------
	Vedi i commenti in-line per maggiori informazioni.



------------------------------------------------------------------------------
10) Opzioni della linea di comando
------------------------------------------------------------------------------
NOTA: le opzioni della linea di comando bypassano il file di configurazione.

	--gl-version:
	Mostra informazioni sull'implementazione di OpenGL della tua macchina.

	--homedir newHomePath
	Specifica una cartella per i dati di OpenCity. Per esmpio:
		Sotto linux: --homedir /home/foo/bar/share/opencity/"
		Sotto win32: --homedir "C:/Programmi/OpenCity/share"

	--server: (sperimentale, caratteristica in via di sviluppo)
	Lancia la versione server di OpenCity. Premi Ctrl-C per uscire.
	Se vuoi testare il server sul tuo computer, lancia un'altra volta OpenCity
	senza "--server" e poi premi 't' per connetterti al server locale di OpenCity.



------------------------------------------------------------------------------
11) Note sul salvataggio / caricamento 
------------------------------------------------------------------------------
	La seguente directory è usata per salvare/caricare
		GNU/Linux:
			$HOME/.OpenCity/
		Windows:
			C:\Documents and Settings\username\Application Data\OpenCity\



------------------------------------------------------------------------------
12) Problemi noti
------------------------------------------------------------------------------
	1) Ho una ATI Radeon 9000M. Quando provo il gioco con "EnablePageFlip"
(vedi man radeon) il gioco viene eseguito stranamente. Se riscontri lo stesso
problema prova a disabilitare questa funzionalità del server Xfree
(vedi man XF86Config)

	2) Con la ATI Radeon 9000m bisogna disattivare SmoothVision(tm) per
eseguire OpenCity correttamente.

	3) Questa è la mia versione del server X

 - XFree86 versione 4.3.0
 - Data di rilascio: 9 Maggio 2003
 - X Protocol versione 11, revisione 0, release 6.6
 - Sistema operativo di build: Linux 2.4.18-23mdksmp i686 [ELF]
 - Data di build: 10 Settembre 2003

	Sembrerebbe che i driver per la mia ATI Radeon 9000M siano pieni di bugs.
Se dovessi avere problemi prova a scrivere quanto sotto in una shell (bash) prima di giocare.
		"export R200_NO_VTXFMT="

	Se non funziona puoi provare con una nuova versione di XFree.


------------------------------------------------------------------------------
www.opencity.info

Duong-Khang (neoneurone) NGUYEN

Translated by Jacopo (morpheus) SIRIANNI