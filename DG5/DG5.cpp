// Programma Principale (Main)
// 
//
#include "stdafx.h"

using namespace std;

// Definisco oggetto della classe Project
Project *proj;
// Definisco Variabili glove1 e glove2
int gloveid1, gloveid2;
// Definisco oggetto della classe GestioneFile
GestioneFile *gest_file;
// Definisco oggetto della classe Riconosci_Gesti
Riconosci_Gesti *gesti;
// Definisco oggetto della classe PowerPoint
PowerPoint *power;
// Definisco oggeto della classe Scarto
Calcola_Scarto *scarto;

int _tmain(int argc, _TCHAR* argv[])
{
	int risp_glove1,risp_glove2;
	// Inizializzazione del Software
	// Inizializzazione oggetto proj della classe Project 
	// In questo caso stiamo inizializzando il gestore del glove.
	proj = new Project();
	// Inizializzo oggetto GestioneFile, serve per gestire l'accesso ai files.
	gest_file = new GestioneFile();
	// Inizializzo il Riconoscitore
	scarto = new Calcola_Scarto();
	// Per procedere dobbiamo utilizzare due variabili: gloveid1 gloveid2
	// La funzione AddDataGlove(nome,comport,tipo) crea un nuovo data glove il cui nome e' "nome", 
	// è connesso alla porta seriale "comport" ed è del tipo "tipo" 
	// !!!ATTENZIONE!! in  tipo 0 indica il DATA GLOVE DESTRO
	//              mentre tipo 1 indica il DATA GLOVE SINISTRO
	
	// Aggiungo guanto destro su porta COM
	gloveid1 = proj->AddDataGlove("dataglove1",8,0);
	if ( gloveid1 == -1 )
		printf("Non e' possibile aggiungere il guanto destro\n");
	// Aggiungo guanto sinistro su porta COM
	gloveid2 = proj->AddDataGlove("dataglove2",9,1);
	if ( gloveid1 == -1 )
		printf("Non e' possibile aggiungere il guanto destro.\n");

	// Inizio a raccogliere i Dati!
	// La funzione StartStampling(gloveid) apre una specifica porta COM e inizia ad ottenere campioni di essa,
	// i dati vengono ricevuti dal glove e successivamente vengono memorizzati all'interno della struttura DataGlove
	// per fermare il campionamento è possibile usare la funzione StopSampling(gloveid)

	// Apro ricezione dati sul guanto destro
	risp_glove1 = proj->StartSampling(1);
	if ( risp_glove1 == -1 )
		printf("Il data glove destro non e' stato trovato\n");
	// Apro ricezione dati sul guanto sinitro
	risp_glove2 = proj->StartSampling(2);
	if ( risp_glove2 == -1 )
		printf("Il data glove sinistro non e' stato trovato\n");

	//Inizializzo
	gesti = new Riconosci_Gesti();
	power = new PowerPoint();

	// Permette di eseguire una scelta
	int Scelta;
	do
	{ 
		Maschera ("Un riconoscitore di gesti basato su Data Glove");
		Scelta = ScegliMenu ();
		switch (Scelta)
		{ 
		case 1: // 
			Addestramento();
			break;
		case 2: // Permette di rilevare un gesto
			Rileva_Gesto();
			break;
		case 3: // Permette di inviare dei comandi all'applicazione PowerPoint
			Demo_PowerPoint();
			break;
		case 4: // 
			Scarto();
			break;
		case 5: //
			
			break;
		case 6: //

			break;
		case 7: //
			break;
		}
	}
	while ( Scelta > 0 );
	Spostati_Nella_Posizione (20, 22); 
	TextColor (7);


	//proj->StopSampling(1);
	//delete gest_file;
	//delete proj;
}



void Pulisci_Schermo(void)
{
	COORD coord;
	DWORD written;
	CONSOLE_SCREEN_BUFFER_INFO info;
	coord.X = 0;
	coord.Y = 0;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	FillConsoleOutputCharacter (GetStdHandle(STD_OUTPUT_HANDLE), ' ',50*50, coord, &written);
	Spostati_Nella_Posizione(1,1);
}

/*
  Permette di disegnare una cornice nella console, che avra' H righe e 
  W colonne vuote (il contenuto è cancellato).
  Parametri in input:
      X: angolo in alto a sinistra
	  W: numero di colonne vuote
	  H: numero di righe vuote
*/
void Crea_Cornice( int X,int Y,int W,int H )
{ 
	int i, j;
	TextColor (13);
	Spostati_Nella_Posizione (X-1, Y-1);
	printf("%c",218); 
	for ( i = 1; i <= W; i++ )
		printf("%c",196); 
	printf("%c",191);
	
	for ( j = 0; j < H; j++ ) {
		Spostati_Nella_Posizione (X-1, Y+j);
		printf("%c",179); 
		for ( i = 1; i <= W; i++ )
			printf(" "); 
		printf("%c",179);
	}
	Spostati_Nella_Posizione (X-1, Y+H);
	printf("%c",192); 
	for ( i = 1; i <= W; i++ )
		printf("%c",196); 
	printf("%c",217);
}


/*
  Permette la stampa della lista delle opzioni disponibili.
  Parametri in input:
     X e Y: Coordinate dove andare a stampare la frase
	 C: condizione: 0 normale, 1 evidenziato
*/
void Crea_Lista_Opzioni(char *Frase,int X,int Y,int C)
{ 
	int i;
	char Buff[60];
	for ( i = 0; i < 56 && Frase[i];  i++ )
		Buff[i] = Frase[i];
	Buff[i] = NULL;
	Crea_Cornice (X, Y, 76, 1);
	TextColor (C?10:7);
	Spostati_Nella_Posizione (X, Y);
	printf("%-25s", Buff);
}

/*
  Posiziona il cursore della console alle coordinate x,y.
  x: Indice di colonna
  y: indice di riga
*/
void Spostati_Nella_Posizione( int x,int y ) {
	COORD Cur = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

/*
  Funzione per la lettura di una stringa da tastiera.
    *Stringa Rappresenta il puntatore della stringa
    LenMax lunghezza massima dello spazio allocato
*/
void LeggiStringa (char *Stringa,int LenMax)
{ 
	int i;
	fgets ( Stringa, LenMax, stdin );
	i = strlen(Stringa) -1;
	if (Stringa[i] < 32) 
		Stringa[i] = 0;
	fflush( stdin );
}


/*
  Crea una maschera standard per il software:
*/
void Maschera( char *Titolo )
{ 
	char Rigo[96];
	int d, i, n;
	if ( (n=strlen(Titolo)) > 78) 
		n = 78; // tronca il titolo lungo
	sprintf_s (Rigo, 81, "%80s", " ");
	for ( d = (80-n)/2, i = 0; i<n ; Rigo[d++] = Titolo[i++] );

	Pulisci_Schermo(); // Pulisce lo schermo
	Spostati_Nella_Posizione(0, 0); 
	TextColor (0x9A);// 0xC0
	printf(Rigo);
	Spostati_Nella_Posizione(0,24); 
	TextColor (0x9A);// Colore in basso
	printf(Rigo);
	Spostati_Nella_Posizione(0, 0); 
	TextColor(7);
}

/*
  Utilizzando la funzione SetConsoleTextAttribute(HANDLE,WORD)
  cambia il colore del testo da stampare in console.
  Riceve in input il codice numerico del colore
*/
void TextColor(unsigned short Colore) {
	HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );
	SetConsoleTextAttribute ( h, Colore );
}

/*
  Permette all'utente di fare una scelta tra un elenco di voci.
*/
int ScegliMenu (void)
{
	char *Menu_opzioni[] = { " ", "1. Raccolta e gestione dati.",
		                      "2. Riconoscimento gesto utilizzando un data glove.",
		                      "3. Utilizza il PowerPoint mediante data glove",
							  "4. Calcola lo scarto",
		                      "0. Exit", };
	int numero_opzioni = 5, // Numero di opzioni disponibili
		Rigo = 5, // voce selezionata
		fine_op = 1, // Stabilisce la fine 0 per finito, 1 in funzione
		scelta_op, // Tipo di scelta eseguita
		i;   // Contatore
	while (fine_op)
	{
		for ( i = 1; i <= 5; i++ )
			Crea_Lista_Opzioni(Menu_opzioni[i], 2, 3*i, (i==Rigo));
		Crea_Cornice ( 2, 18, 76, 4); 
		TextColor(10);
		switch (Rigo)
		{
		case 1: // Raccolta dati per il database
			Spostati_Nella_Posizione(2,18);
			printf("Per poter utilizzare il data glove e' necessario raccogliere un numero ");
			Spostati_Nella_Posizione(2,19); 
			printf("sufficiente di campioni. Dopo aver raccolto i campioni e' possibile");
			Spostati_Nella_Posizione(2,20);
			printf("creare un database per stabilire se i dati rilevati dal data glove");
			Spostati_Nella_Posizione(2,21);
			printf("corrispondono ad un gesto noto. ");
			break;
		case 2: // Riconoscimento del gesto
			Spostati_Nella_Posizione(2,18);
			printf("Dopo aver acquisito un database, e' possibile identificare un gesto.");
			Spostati_Nella_Posizione(2,19);
			printf("L'identificazione del gesto avviene semplicemente analizzando i dati");
			Spostati_Nella_Posizione(2,20);
			printf("forniti in tempo reale dal data glove mediate un apposito algoritmo.");
			break;
		case 3: // Utilizza powerpoint con il data glove
			Spostati_Nella_Posizione(2,18);
			printf("E' possibile utilizzare i due guanti per eseguire alcune operazioni");
			Spostati_Nella_Posizione(2,19);
			printf("fondamentali con l'applicazione powerpoint.");
			Spostati_Nella_Posizione(2,20);
			printf("Utilizzando un solo guanto o entrambi si possono eseguire specifiche");
			Spostati_Nella_Posizione(2,21);
			printf("operazioni quali: scrivere un testo o mandare avanti o indietro le slides.");
			break;
		case 4: // Permette di calcolare lo scarto
			Spostati_Nella_Posizione(2,18);
			printf("Permette di calcolare lo scarto.");
			Spostati_Nella_Posizione(2,19);
			printf("");
			Spostati_Nella_Posizione(2,20);
			printf("");
			Spostati_Nella_Posizione(2,21);
			printf("");
			break;
		case 5: 
			// Uscita
			Spostati_Nella_Posizione(2,18);
			printf("Conclude il programma. ");
			break;
		}
		// Ottengo la scelta eseguita
		scelta_op = _getch();
		switch (scelta_op)
		{
		case 224: // sposta menu con le freccette del cursore
			scelta_op = _getch();
			if (scelta_op == 80) 
				Rigo = Rigo < numero_opzioni ? Rigo+1 : 1;
			else if (scelta_op == 72) 
				Rigo = Rigo > 1 ? Rigo-1 : numero_opzioni;
			break;
		case '1': // converte ASCII in numero;
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7': Rigo = scelta_op - '0';
			break;
		case '0': // '0' o ESC termine procedura
		case ESC: Rigo = numero_opzioni;
			break;
		case INVIO: // conferma il rigo selezionato
			fine_op = 0;
			break;
		}
	}
	// l'ultimo rigo diventa Zero
	return (Rigo%numero_opzioni);
}

// Questo metodo gestisce la fase di addestramento
// con le relative opzioni che possono essere
// scelte.
void Addestramento(void)
{
	int scelta_op = 0;
	// Nome per la finestra
	Maschera ("Acquisizione Insieme di Esempi dei Gesti");
	// Crea una Cornice per i messaggi
	Crea_Cornice (2, 2, 76, 8);
	// Stabilisce il colore
	TextColor(46);
	Spostati_Nella_Posizione(2,2);
	// Stampo le opzioni disponibili
	printf("A) Numero di campioni presenti nel database.");
	Spostati_Nella_Posizione(2,3); 
	printf("B) Genera file di test e training non normalizzato.");
	Spostati_Nella_Posizione(2,4);
	printf("C) Verifica la presenza di doppioni.");
	Spostati_Nella_Posizione(2,5);
	printf("D) Trova il valore minimo e massimo nel database.");
	Spostati_Nella_Posizione(2,6);
	printf("E) Genera file di test e training normalizzato.");
	Spostati_Nella_Posizione(2,7);
	printf("F) Eseguire una scansione singola dei dati.");
	Spostati_Nella_Posizione(2,8);
	printf("G) Esegui una scansione multipla dei dati.");
	Spostati_Nella_Posizione(2,9);
	printf("ESC) Torna al menu' principale");
	Spostati_Nella_Posizione(2,10);
	// Crea una nuova cornice
	Crea_Cornice (2, 12, 76, 10);
	Spostati_Nella_Posizione(2,23);
	printf("La tua scelta: ");
	// Ciclo per ricevere i parametri da tastiera
	do
	{ 
		if (_kbhit() )
			scelta_op = _getch();
		// Verifica il tipo di scelta effettuato
		if ( scelta_op =='A' || scelta_op == 'a')
			scelta_op = gest_file->ShowValues(proj, gloveid1, gloveid2);
		else if (scelta_op =='B' || scelta_op == 'b')
			gest_file->ShowValues(proj, gloveid1, gloveid2);
		else if (scelta_op =='C' || scelta_op == 'c')
			gest_file->ShowValues(proj, gloveid1, gloveid2);
		else if (scelta_op =='D' || scelta_op == 'd')
			gest_file->ShowValues(proj, gloveid1, gloveid2);
		else if (scelta_op =='E' || scelta_op == 'e')
			gest_file->ShowValues(proj, gloveid1, gloveid2);
		else if (scelta_op =='F' || scelta_op == 'f')
			gest_file->ShowValues(proj, gloveid1, gloveid2);
		else if (scelta_op =='G' || scelta_op == 'g')
			gest_file->ShowValues(proj, gloveid1, gloveid2);
		else if (scelta_op == ESC || scelta_op == ESC )// Serve per il riconoscimento del comando ESC
			;
		// Permette di fermare l'output
		else if (scelta_op == SPAZIO ) {
			for ( scelta_op = 0; scelta_op < 1;) {
				if (_kbhit() ) 
					scelta_op = _getch();
			}
		}
	}
	while (scelta_op != ESC);
}

// Questo metodo permette di avviare la procedura per il
// riconoscimento di un gesto.
void Rileva_Gesto(void)
{
	int scelta = 0;
	// Nome per la finestra
	Maschera ("Riconoscimento dei Gesti");
	// Crea una Cornice per i messaggi
	Crea_Cornice (2, 2, 76, 8);
	// Stabilisce il colore
	TextColor(46);

	Spostati_Nella_Posizione(2,2);
	printf("A) Per riconoscere un gesto.");
	Spostati_Nella_Posizione(2,3);
	printf("N) Torna al menu' principale");
	// Crea una nuova cornice
	Crea_Cornice (2, 12, 76, 10);
	Spostati_Nella_Posizione(2,23);
	printf("La tua scelta: ");
	// Ciclo per ricevere i parametri da tastiera
	do
	{
		if ( _kbhit() )
			scelta = _getch();
		// Verifica il tipo di scelta effettuato
		if ( scelta =='A' || scelta == 'a')
			scelta = gesti->Riconoscimento(proj, gloveid1, gloveid2);
		else if (scelta =='N' || scelta == 'n')
			scelta = ESC;
		// Permette di fermare l'output
		else if (scelta == SPAZIO ) {
			for ( scelta = 0; scelta < 1;) {
				if (_kbhit() ) 
					scelta = _getch();
			}
		}
	}
	while (scelta != ESC);
}

// Questo metodo permette di avviare la procedura di riconoscimento
// gesti per inviare comandi all'applicazione PowerPoint.
void Demo_PowerPoint(void)
{
	int scelta = 0;
	// Nome per la finestra
	Maschera ("Gestione PowerPoint con Data Glove");
	// Crea una Cornice per i messaggi
	Crea_Cornice (2, 2, 76, 8);
	// Stabilisce il colore
	TextColor(46);

	Spostati_Nella_Posizione(2,2);
	printf("A) Usa il data glove per inviare comandi PowerPoint.");
	Spostati_Nella_Posizione(2,3);
	printf("N) Torna al menu' principale");
	// Crea una nuova cornice
	Crea_Cornice (2, 12, 76, 10);
	Spostati_Nella_Posizione(2,23);
	printf("La tua scelta: ");
	// Ciclo per ricevere i parametri da tastiera
	do
	{
		if ( _kbhit() )
			scelta = _getch();
		// Verifica il tipo di scelta effettuato
		if ( scelta =='A' || scelta == 'a')
			scelta = power->Avvia_power(proj, gloveid1, gloveid2);
		else if (scelta =='N' || scelta == 'n')
			scelta = ESC;
		// Permette di fermare l'output
		else if (scelta == SPAZIO ) {
			for ( scelta = 0; scelta < 1;) {
				if (_kbhit() ) 
					scelta = _getch();
			}
		}
	}
	while (scelta != ESC);
}

void Scarto(void)
{
	int scelta = 0;
	// Nome per la finestra
	Maschera ("Calcolo dello Scarto");
	// Crea una Cornice per i messaggi
	Crea_Cornice (2, 2, 76, 8);
	// Stabilisce il colore
	TextColor(46);

	Spostati_Nella_Posizione(2,2);
	printf("A) Per calcolare lo scarto.");
	Spostati_Nella_Posizione(2,3);
	printf("N) Torna al menu' principale");
	// Crea una nuova cornice
	Crea_Cornice (2, 12, 76, 10);
	Spostati_Nella_Posizione(2,23);
	printf("La tua scelta: ");
	// Ciclo per ricevere i parametri da tastiera
	do
	{
		if ( _kbhit() )
			scelta = _getch();
		// Verifica il tipo di scelta effettuato
		if ( scelta =='A' || scelta == 'a')
			scelta = scarto->Riconoscimento(proj, gloveid1, gloveid2);
		else if (scelta =='N' || scelta == 'n')
			scelta = ESC;
		// Permette di fermare l'output
		else if (scelta == SPAZIO ) {
			for ( scelta = 0; scelta < 1;) {
				if (_kbhit() ) 
					scelta = _getch();
			}
		}
	}
	while (scelta != ESC);
}

// Questo metodo permette di pulire la cornice, infatti, dopo ogni scrittura
// quello che viene stampato in output risulta visibile finche' non viene
// cancellato.
void Pulisci_Seconda_Cornice()
{
	Spostati_Nella_Posizione(2,12);
	printf("                                                                            ");
	Spostati_Nella_Posizione(2,13);
	printf("                                                                            ");
	Spostati_Nella_Posizione(2,14);
	printf("                                                                            ");
	Spostati_Nella_Posizione(2,15);
	printf("                                                                            ");
	Spostati_Nella_Posizione(2,16);
	printf("                                                                            ");
	Spostati_Nella_Posizione(2,17);
	printf("                                                                            ");
	Spostati_Nella_Posizione(2,18);
	printf("                                                                            ");
	Spostati_Nella_Posizione(2,19);
	printf("                                                                            ");
	Spostati_Nella_Posizione(2,20);
	printf("                                                                            ");
	Spostati_Nella_Posizione(2,21);
	printf("                                                                            ");
}
