//
// Questa classe si occupa di calcolatore lo scarto per il training
//
#include "stdafx.h"

float somma_buffer[35];
int cnt_generale = 0,cnt_matr = 0;
float scarto = 0;


// I costruttori sono utili per inizializzare le variabili 
// della classe o per allocare aree di memoria. 
Calcola_Scarto::Calcola_Scarto()
{
	// Inizializzo i seguenti parametri a 0
	prova[0] = 0.000000f,prova[1] = 0.000000f,prova[2] = 0.000000f,
	prova[3] = 0.000000f,prova[4] = 0.000000f,prova[5] = 0.000000f;
	CNT1 = 0, CNT2 = 0, CNT3 = 0,tipo_cnt = 0,tipo_ni  = 0,primo_avvio = 0;
	CNT4 = 0, CNT5 = 0, CNT6 = 0;
	cont_righe = 0, cont_colonne = 0;
}

void Calcola_Scarto::ChiediTipoDiGuato()
{
	Pulisci_Seconda_Cornice();
	Spostati_Nella_Posizione(2,12);
	printf("Specifica il tipo di guanto su cui vui lavorare\n");
	Spostati_Nella_Posizione(2,13);
	printf("Inserisci D per guanto destro oppure\n");
	Spostati_Nella_Posizione(2,14);
	printf("Inserisci S per guanto sinistro.\n");
	Spostati_Nella_Posizione(2,15);
	printf("Scelta(D/S): ");
	// Prendi da input la scelta effettuata dall'utente
	scanf("%s",&tipo_guanto);
	Pulisci_Seconda_Cornice();
	Spostati_Nella_Posizione(16,23);
}

// Queso metodo si occupa di calcolare l'elevazione al quadrato dei centroidi
// che sono stati precedentemene ottenuti, leggendo le informazioni presenti
// sul file codebook.
void Calcola_Scarto::pow_Centroidi() {
	for ( int ni = 0; ni < totline2-1; ni++ ) {
		for ( int ju = 0; ju < COL; ju++ ) {
			if ( ju != 5 ) {
				matrice_Centroidi_pow[ni][ju] = pow(matrice_Centroidi[ni][ju],2);
			}
		}
	}
}

// Questo metodo si occupa di elevare al quadrato (mediante pow) i valore della 
// matrice contenente le informazioni riguardo il file di test.
void Calcola_Scarto::pow_Test() {
	for ( int ni = 0; ni < totline-2; ni++ ) {
		for ( int ju = 0; ju < 5; ju++ ) {
			matrice_Text_pow[ni][ju] = pow(matrice_Test[ni][ju],2);
		}
	}
}

// Metodo per il riconoscimento del gesto statico
int Calcola_Scarto::Riconoscimento(Project *proj,int gloveid1,int gloveid2 )
{
	ChiediTipoDiGuato();

riparte:
	float somme_distanze = 0.0f;
	Pulisci_Seconda_Cornice();

	Inizializzatore();

	// Genero una matrice contenente le informazioni riguardo i centroidi
	Genera_Matrice_Centroidi();
	// Eleva al quadrato i centroidi
	pow_Centroidi();
	// Genero la matrice anche per il file di test
	Genera_Matrice_Test();
	// Elevo al quadrato i dati test
	pow_Test();

	// Devo verificare un singolo elemento della matrice contenente le info riguardo
	// il file test elevato al quadrato con quella contenente i centroidi.
	// Poi salvare il risultato ( true / false ) in un'altra matrice.
	float vettore_buffer[5];
	
	int jeo = 0;
	int numero_true  = 0;
	int numero_false = 0;
	float distanze_buffer[5];


	for ( int cnt_grande = 0; cnt_grande < totline-2; cnt_grande++ )
	{
		char gest_id;
		int io;

		io = matrice_Test[cnt_grande][5];
		gest_id = (char) io;

		for ( int jun = 0; jun < 5; jun++ ) 
		{
			vettore_buffer[jun] = matrice_Text_pow[cnt_grande][jun];
			Spostati_Nella_Posizione(2,15);
			printf("Elementi analizzati: %d",cnt_grande);
			Spostati_Nella_Posizione(2,16);
			printf("Sto inserendo nel vettore %f\n",vettore_buffer[jun]);
		}


		// Creo un altro ciclo
		for ( int ni = 0; ni < totline2-1; ni++ )
		{
			for ( int ju = 0; ju < 5; ju++ )
			{
				distanze_buffer[ju] = abs(matrice_Centroidi_pow[ni][ju] - vettore_buffer[ju]);
				Spostati_Nella_Posizione(2,17+ju);
				printf("La differenza tra %f e %f e' pari a %f",matrice_Centroidi_pow[ni][ju],vettore_buffer[ju],distanze_buffer[ju]);
			}
			// Adesso somme le distanze
			somme_distanze = 0;
			for ( int mji = 0; mji < 5; mji++ ) {
				somme_distanze += distanze_buffer[mji];
			}
			// Salvo il valore delle distanze somma in un apposito vettore
			somma_buffer[ni] = somme_distanze;
		}
		//
		char prova;
		prova = Trova_Gesto();

		Spostati_Nella_Posizione(29,14);
		printf("Il valore su file test e': %c",gest_id);

		if ( prova == gest_id ) {
			numero_true++;
			Spostati_Nella_Posizione(29,15);
			printf("VERI: %d",numero_true);
			matrice_Finale[cnt_grande][1] = 1;
		}
		if ( prova != gest_id ) {
			numero_false++;
			Spostati_Nella_Posizione(45,15);
			printf("FALSI: %d",numero_false);
			matrice_Finale[cnt_grande][1] = 0;
		}
	}
	// Contatore generico che viene utilizzato per trovare il gesto
	// alla fine deve essere resettato per evitare conflitti.
	cnt_matr = 0;
	Scarto();
	

	// Procedura terminata, stampo il risultato
	Pulisci_Seconda_Cornice();
	Spostati_Nella_Posizione(2,14);
	printf(" Ho trovato %d positivi %d falsi\n",numero_true,numero_false);
	Spostati_Nella_Posizione(2,15);
	printf("Il valore dello scarto calcolato e' pari a : %f",scarto);
	Spostati_Nella_Posizione(2,16);
	cout << "Vuoi eseguire una scansione dati? (S/N) ";
	Spostati_Nella_Posizione(16,23);
	printf("  ");
	Spostati_Nella_Posizione(16,23);
	scanf("%s",&tipo_scelta);
	if ( (strcmp (tipo_scelta,"N") == 0 ) || (strcmp (tipo_scelta,"n")  == 0) )
		return 27;
	if ( (strcmp (tipo_scelta,"S") == 0 ) || (strcmp (tipo_scelta,"s")  == 0) )
		goto riparte;
	

	return 0;
}

void Calcola_Scarto::Scarto()
{
	float val = 0,fmax = 0,temp = 0,percentuale = 0;
	// Devo trovare il valore massimo presente nelle distanze(fmax)
	for ( int cnt_matrice = 0; cnt_matrice < totline-2; cnt_matrice++ )
	{
		val = matrice_Finale[cnt_matrice][0];
		if ( val > fmax )
			fmax = val;
	}
	scarto = fmax;
	// Trovato il valore fmax devo verificare se ci sono dei valori
	// "falsi" (0), minori di esso.
	for ( int cnt_matrice = 0; cnt_matrice < totline-2; cnt_matrice++ )
	{
		if (  matrice_Finale[cnt_matrice][1] == 0 )
		{
			if ( matrice_Finale[cnt_matrice][1] < fmax )
			{
				// Se sono presenti dei valori inferiori nei "falsi" abbasso il
				// valore di scarto di una certa percentuale, il 10%.
				temp = scarto * 1;
				percentuale = temp / 100;
				scarto = scarto - percentuale;
				fmax = scarto;
			}
		}
	}
}

// Questo metodo si occupa di generare la matrice test, le informazioni per poter
// formulare questa matrice vengono estratte mediante la lettura del file Test.dbm,
// per il guanto destro, e Test_s.dbm, per il guanto sinistro.
// Dopo l'apertura del file si procede ad estrarre le informazioni di interesse,
// le informazioni dopo essere state appositamente elaborate vengono salvate in un
// apposita matrice.
void Calcola_Scarto::Genera_Matrice_Test()
{
	Inizializzatore();
	char *nomefileTest_Sinistro;
	char *nomefileTest_Destro;
	nomefileTest_Sinistro = "Test_s.dbm";
	nomefileTest_Destro   = "Test.dbm";

	// Metodo per inizializzare a 0 la matrice, serve per evitare dati corrotti.
	Inizializzazione_Matrice(matrice_Test);

	// Verifico su quale guanto devo lavorare ( sinistro O destro)

	// Conta il numero di file presenti nel file
	if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"d") == 0 ))
		// Per il guanto destro
		Conta_Elementi(nomefileTest_Destro,0);
	if ( (strcmp(tipo_guanto,"S") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) )
		// Per il guanto sinistro
		Conta_Elementi(nomefileTest_Sinistro,0);

	if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"d") == 0 )) {
		// Per il guanto destro
		if ( ( FileGlove = fopen(nomefileTest_Destro,"r") ) == NULL ) {
			printf("Attenzione! Il file di training non e' presente.\n");
			system("pause");
			exit(1);
		}
	}
	if ( (strcmp(tipo_guanto,"S") == 0 ) || (strcmp(tipo_guanto,"s") == 0 )) {
		// Per il guanto sinistro
		if ( ( FileGlove = fopen(nomefileTest_Sinistro,"r") ) == NULL ) {
			printf("Attenzione! Il file di training non e' presente.\n");
			system("pause");
			exit(1);
		}
	}

	int zx = 3, pi = 14;
	int ty = 25,qp = 36;
	int kj = 47;


	// leggo il file fino alla fine.
	while( !feof(FileGlove) )
	{
		// Prelevo i dati in ingresso
		fgets(buf_dati,256,FileGlove);
		// Primo valore numerico Float
		for ( int qa = 0; qa <= 7; qa++ ) {
			numf[qa] = buf_dati[zx];
			zx++;
		}
		numf[8] = '\0';
		zx = 3;
		val1 = (float)atof(numf);
		vettore_corr[0] = val1;
		memset(numf, 0, 8);
		// Secondo valore numerico float
		for ( int qa = 0; qa <= 7; qa++ ) {
			numf[qa] = buf_dati[pi];
			pi++;
		}
		numf[8] = '\0';
		pi = 14;
		val2 = (float)atof(numf);
		vettore_corr[1] = val2;
		memset(numf, 0, 8);
		// Terzo valore numerico float
		for ( int qa = 0; qa <= 7; qa++ ) {
			numf[qa] = buf_dati[ty];
			ty++;
		}
		numf[8] = '\0';
		ty = 25;
		val3 = (float)atof(numf);
		vettore_corr[2] = val3;
		memset(numf, 0, 8);
		// Quarto valore numerico float
		for ( int qa = 0; qa <= 7; qa++ ) {
			numf[qa] = buf_dati[qp];
			qp++;
		}
		numf[8] = '\0';
		qp = 36;
		val4 = (float)atof(numf);
		vettore_corr[3] = val4;
		memset(numf, 0, 8);
		// Quinto valore numerico float
		for ( int qa = 0; qa <= 7; qa++ ) {
			numf[qa] = buf_dati[kj];
			kj++;
		}
		numf[8] = '\0';
		kj = 47;
		val5 = (float)atof(numf);
		vettore_corr[4] = val5;
		memset(numf, 0, 8);

		// Verifico che i valori siano maggiori di 0 per evitare dati corrotti, chiaramente
		// se sono minori di 0 qualcosa deve essere andato storto durante la comunicazione
		// con il data glovee.
		if ( vettore_corr[0] > 0 && vettore_corr[1] > 0 &&
			 vettore_corr[2] > 0 && vettore_corr[3] > 0 &&
			 vettore_corr[4] > 0 )
		{
			// Per evitare dati non corretti, verifico che il valore sia maggiore di 0
			// Elevazione al quadrato mediante pow
			// della riga corrrente
			Inserisci_Valore_Matrice(vettore_corr[0]);
			Inserisci_Valore_Matrice(vettore_corr[1]);
			Inserisci_Valore_Matrice(vettore_corr[2]);
			Inserisci_Valore_Matrice(vettore_corr[3]);
			Inserisci_Valore_Matrice(vettore_corr[4]);
	
			//printf("La tipologia di gesto e' %c\n",buf_dati[45]);
			tipo_elm2[CNT6] = buf_dati[58];
			CNT6++;
			Inserisci_Tipo_Matr(matrice_Test,buf_dati[58]);
		}
	}
	// Chiudi il file
	fclose(FileGlove);
}

void Calcola_Scarto::Inserisci_Valore_Matrice(float valore)
{
	if ( cont_colonne == 5 ) {
		cont_colonne = 0;
		cont_righe++;
	}
	if ( cont_righe < totline-2 && cont_colonne < 5)
	{
		matrice_Test[cont_righe][cont_colonne] = valore;
		cont_colonne++;
	}
}



void Calcola_Scarto::Genera_Matrice_Centroidi()
{
	Inizializzatore();
	char *nomefileCentroidi_Destro;
	char *nomefileCentroidi_Sinistro;
	nomefileCentroidi_Destro =   "glove.codf";
	nomefileCentroidi_Sinistro = "glove_s.codf";
	//
	// Inizializzo la matrice a 0 per evitare dati "corrotti"
	Inizializzazione_Matrice(matrice_Centroidi);
	
	// Conta il numero di file presenti nel file
	if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"d") == 0 ))
		// Per il guanto destro
		Conta_Elementi(nomefileCentroidi_Destro,1);
	if ( (strcmp(tipo_guanto,"S") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) )
		// Per il guanto sinistro
		Conta_Elementi(nomefileCentroidi_Sinistro,1);


	// Tipo di file da aprire ///
	// A seconda del tipo di guanto utilizzato viene scelto il relativo file
	if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"d") == 0 )) {
		// Per il guanto destro
		if ( ( FileGlove = fopen(nomefileCentroidi_Destro,"r") ) == NULL ) {
			printf("Attenzione! Il file di training non e' presente.\n");
			system("pause");
			exit(1);
		}
	}
	if ( (strcmp(tipo_guanto,"S") == 0 ) || (strcmp(tipo_guanto,"s") == 0 )) {
		// Per il guanto sinistro
		if ( ( FileGlove = fopen(nomefileCentroidi_Sinistro,"r") ) == NULL ) {
			printf("Attenzione! Il file di training non e' presente.\n");
			system("pause");
			exit(1);
		}
	}

	// 	// Fino alla fine del file
	while( !feof(FileGlove) )
	{   // Prelevo i dati in ingresso
		fgets(buf_dati,256,FileGlove);
		//printf("Sto leggendo %s\n",buf_dati);
		// Primo valore numerico Float
		for ( int qa = 0; qa <= 7; qa++ ) {
			numf[qa] = buf_dati[qa];
		}
		numf[8] = '\0';
		val1 = (float)atof(numf);
		vettore_corr[0] = val1;
		memset(numf, 0, 8);
			
		// Secondo valore numerico float
		int bv = 0;
		for ( int qn = 9; qn <= 16; qn++ ) {
			numf[bv] = buf_dati[qn];
			bv++;
		}
		numf[8] = '\0';
		val2 = (float)atof(numf);
		vettore_corr[1] = val2;
		memset(numf, 0, 8);
			
		// Terzo valore numerico float
		int bt = 0;
		for ( int qn = 18; qn <= 25; qn++ ) {
			numf[bt] = buf_dati[qn];
			bt++;
		}
		numf[8] = '\0';
		val3 = (float)atof(numf);
		vettore_corr[2] = val3;
		memset(numf, 0, 8);
		// Quarto valore numerico float
		int wr = 0;
		for ( int qp = 27; qp <= 34; qp++ ) {
			numf[wr] = buf_dati[qp];
			wr++;
		}
		numf[8] = '\0';
		val4 = (float)atof(numf);
		vettore_corr[3] = val4;
		memset(numf, 0, 8);
		// Quinto valore numerico float
		int as = 0;
		for ( int qa = 36; qa <= 43; qa++ ) {
			numf[as] = buf_dati[qa];
			as++;
		}
		numf[8] = '\0';
		val5 = (float)atof(numf);
		vettore_corr[4] = val5;
		memset(numf, 0, 8);

		if ( vettore_corr[0] > 0 && vettore_corr[1] > 0 &&
			 vettore_corr[2] > 0 && vettore_corr[3] > 0 &&
			 vettore_corr[4] > 0 )
		{
			// Per evitare dati non corretti, verifico che il valore sia maggiore di 0
			// Elevazione al quadrato mediante pow
			// della riga corrrente
			for ( int hg = 0; hg < 5; hg++ ) {
				Inserisci_Elem_Matr(matrice_Centroidi,vettore_corr[hg]);
			}
	
			tipo_elemento[CNT3] = buf_dati[45];
			CNT3++;
			Inserisci_Tipo_Matr(matrice_Centroidi,buf_dati[45]);
		}
	}
	// Chiudi il file
	fclose(FileGlove);
}

void Calcola_Scarto::Inizializzazione_Vettore(float vet[MAXRIGHE])
{
	for ( int ni = 0; ni < totline2-1; ni++ ) {
		 vet[ni] = 0;
	}
}

void Calcola_Scarto::Inizializzazione_Matrice(float mat[MAXRIGHE][COL])
{
	// Inizializzazione la matrice a 0
	// Sostituisco RIGHE
	for ( int ni = 0; ni < totline-2; ni++ ) {
		for ( int ju = 0; ju < COL; ju++ ) {
			mat[ni][ju] = 0;
		}
	}
}

void Calcola_Scarto::Stampa_Matrice(float mat[MAXRIGHE][COL])
{
	int incre = 0;
	// stampa la matrice
	for ( int ni = 0; ni < totline-2; ni++ )
	{
		for ( int ju = 0; ju < COL; ju++ ) {
			if ( ju != 5 ) {
				printf(" %f ",mat[ni][ju]);
			}
			if ( ju == 5 ) {
				char ch;
				int i;
				i = mat[ni][ju];
				ch = (char) i;
				printf("%c",ch);
			}
		}
		printf("\n");
	}
}



// Metodo per trovare il gesto.
// Il metodo non fa altro che confrontare la distanza minore con il valore del vettore
// per valutare a quale valore del vettore corrisponde.
char Calcola_Scarto::Trova_Gesto()
{
	float minimo = 9.999f;
	int gesto = 0,kju;
	char gesto_id;

	for ( int yui = 0 ; yui < totline2-1; yui++ )
	{
		if ( somma_buffer[yui] < minimo && somma_buffer[yui] > 0 ) {
			minimo = somma_buffer[yui];
			gesto = yui;
		}
	}

	// Quando ho terminato la procedura otterò il valore del centroide
	// più piccolo. Questo valore può essere utilizzato per trovare il
	// gesto corrispondente e viene anche salvato nella matrice finale
	if ( cnt_matr < totline-2 ) {
		matrice_Finale[cnt_matr][0] = minimo;
		cnt_matr++;
	}

	// Devo controllare di che tipo di gesto si trattaa per fare ciò 
	// analizzo il valore della matrice nella posizione 5.
	kju = matrice_Centroidi[gesto][5];
	// Il valore è intero, per essere utilizzato deve essere
	// convertito in char.
	gesto_id = (char) kju;

	Spostati_Nella_Posizione(2,13);
	printf("Il valore minimo e' %f,IL VALORE DEL GESTO E' %d",minimo,gesto);
	Spostati_Nella_Posizione(2,14);
	printf("Il gesto rilevato e': %c.",gesto_id);

	// Restituisco il risultato
	return gesto_id;
}


// Questo metodo si occupa di inserire un elemento all'interno
// della matrice.
void Calcola_Scarto::Inserisci_Elem_Matr(float mat[MAXRIGHE][COL],float elemento)
{
	// Per la matrice centroidi
	if ( CNT4 >= 0 && CNT4 <= totline2-1 && CNT5 >= 0 && CNT5 <= 6)
		mat[CNT4][CNT5] = elemento;
	if ( CNT5 >= 4 ) {
		CNT4++;
		CNT5 = 0;
	}
	else {
		CNT5++;
	}
}

// Questo metodo si occupa di inserire la tipologia di centroide
// che si sta utilizzando, quindi ad esempio, per una determinata
// quintupla esiste il valore A,B,C,ecc.
void Calcola_Scarto::Inserisci_Tipo_Matr(float mat[MAXRIGHE][COL],float elemento)
{
	mat[tipo_cnt][5] = elemento;
	tipo_cnt++;
}

// Questo metodo si occupa di contare quanti elementi sono
// presenti nel file fornito come input.
void Calcola_Scarto::Conta_Elementi(char nomefile[25],int tipologia)
{
	// Variabili per la gestione del file
	FILE *InFile;
	// Dichiarazione stringhe su cui lavorare
	char stringa[256];
	// Dichiarazione delle altre variabili
	// apertura file e controllo d'ingresso
	InFile = fopen(nomefile,"r");
	// Controllo dell'avvenuta apertura del file
	if( InFile == NULL )
		cout << "\n Errore in apertura file!" << endl;
	if ( tipologia == 0 )
		totline  = 0;
	if ( tipologia == 1 )
		totline2 = 0;
	// Ci dice se il file e' stato aperto del tutto
	while( !feof(InFile) )
	{
		// Controllo di una stringa del file di ingresso
		fgets(stringa,256,InFile);
		// Pulisci il buffer
		memset(stringa, 0, 256);
		if ( tipologia == 0 )
			totline++;
		if ( tipologia == 1 )
			totline2++;
	}
	fclose(InFile);
	Spostati_Nella_Posizione(2,12);
	if ( tipologia == 0 )
		printf("Il numero di elementi presenti nel file training e' %d",totline-2);
	if ( tipologia == 1 )
		printf("Il numero di elementi presenti nel file training e' %d",totline2-1);
}

// Questo metodo si occupa di calcolare la media 
// dei valori forniti
void Calcola_Scarto::Calcola_Media()
{
	// Inizializzo con valore 0 le variabili per le somme
	somma_1 = 0,somma_2 = 0,somma_3 = 0,somma_4 = 0,somma_5 = 0;
	// Inizializzo con valore 0 le variabili per le medie
	media_1 = 0,media_2 = 0,media_3 = 0,media_4 = 0,media_5 = 0;

	/// Calcolo media dito Pollice ///
	for (int gt = 0; gt <= 4; gt++ ) {
		somma_1 = somma_1 + fMatrice[gt][0];
	}
	media_1 = somma_1 / 5;

	/// Calcolo media dito Indice ///
	for (int gt = 0; gt <= 4; gt++ ) {
		somma_2 = somma_2 + fMatrice[gt][1];
	}
	media_2 = somma_2 / 5;

	/// Calcolo media dito Medio ///
	for (int gt = 0; gt <= 4; gt++ ) {
		somma_3 = somma_3 + fMatrice[gt][2];
	}
	media_3 = somma_3 / 5;
	/// Calcolo media dito Anulare ///
	for (int gt = 0; gt <= 4; gt++ ) {
		somma_4 = somma_4 + fMatrice[gt][3];
	}
	media_4 = somma_4 / 5;

	/// Calcolo media dito Mignolo ///
	for (int gt = 0; gt <= 4; gt++ ) {
		somma_5 = somma_5 + fMatrice[gt][4];
	}
	media_5 = somma_5 / 5;
	// Metodo per la normalizzazione
	Normalizzazione();
}

// Questo metodo si occupa di eseguire la normalizzazione.
// La media precedentemente ottenuta viene divisa per 100 e
// successivamente viene sottratta da un valore.
void Calcola_Scarto::Normalizzazione()
{
	valore = 0.001000f;
	norm_1  = ( media_1 / 100 ) - valore;
	norm_2  = ( media_2 / 100 ) - valore;
	norm_3  = ( media_3 / 100 ) - valore;
	norm_4  = ( media_4 / 100 ) - valore;
	norm_5  = ( media_5 / 100 ) - valore;

	if ( norm_1,norm_2,norm_3,norm_4,norm_5 > 0 )
	{
		prova[0] = norm_1/norma;
		prova[1] = norm_2/norma;
		prova[2] = norm_3/norma;
		prova[3] = norm_4/norma;
		prova[4] = norm_5/norma;
	}
}

// Per elaborare i dati e' necessario gestire un gran numero di parametri
// questi parametri vanno riportati a zero dopo che ogni operazione
// e' stata eseguita.
// Questo metodo riporta tutti i parametri al valore 0 o minimo per il 
// rispettivo contesto
void Calcola_Scarto::Inizializzatore()
{
	contatore = 0,righ = 0;
	CNT1 = 0,CNT2 = 0,CNT3 = 0;
	tipo_cnt = 0,tipo_ni  = 0;
	primavolta = 0;
	val1 = 0.1f,val2 = 0.1f,val3 = 0.1f,
	val4 = 0.1f,val5 = 0.1f;

	vettore_pow[0] = 0.000000f,vettore_pow[1] = 0.000000f,vettore_pow[2] = 0.000000f,
	vettore_pow[3] = 0.000000f,vettore_pow[4] = 0.000000f;

	val_min_global[0] = 0.000000f,val_min_global[1] = 0.000000f,val_min_global[2] = 0.000000f,
	val_min_global[3] = 0.000000f,val_min_global[4] = 0.000000f;

	vettore_min[0] = 5.000000f,vettore_min[1] = 5.000000f,vettore_min[2] = 5.000000f,
    vettore_min[3] = 5.000000f,vettore_min[4] = 5.000000f;

	vettore_corr[0] = 0.000000f,vettore_corr[1] = 0.000000f,vettore_corr[2] = 0.000000f,
	vettore_corr[3] = 0.000000f,vettore_corr[4] = 0.000000f;

	vettore_corr_pow[0] = 0.000000f,vettore_corr_pow[1] = 0.000000f,vettore_corr_pow[2] = 0.000000f,
	vettore_corr_pow[3] = 0.000000f,vettore_corr_pow[4] = 0.000000f;
}