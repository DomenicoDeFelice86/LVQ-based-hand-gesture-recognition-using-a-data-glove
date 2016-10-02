// Codice Sorgente per la gestione del riconoscimento del gesto.
// Il processo di riconoscimento si basa su utilizzo di un
// algoritmo noto come Learning Vector Quantization(LVQ).
// Si tratta di un algoritmo di classificazione con Apprendimento 
// supervisionato.
#include "stdafx.h"

// Costruttore
// I costruttori sono utili per inizializzare le variabili 
// della classe o per allocare aree di memoria. 
Riconosci_Gesti::Riconosci_Gesti()
{// Inizializzo a 0
	prova[0] = 0.000000f,prova[1] = 0.000000f,prova[2] = 0.000000f,
	prova[3] = 0.000000f,prova[4] = 0.000000f,prova[5] = 0.000000f;
	CNT1 = 0,CNT2 = 0, CNT3 = 0, tipo_cnt = 0, tipo_ni  = 0,primo_avvio = 0;
	tipo_scelta = ' ';
}

void Riconosci_Gesti::ChiediTipoDiGuato()
{
	Pulisci_Seconda_Cornice();
	Spostati_Nella_Posizione(2,12);
	printf("Specifica il tipo di guanto su cui vuoi lavorare\n");
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

// Metodo per il riconoscimento del gesto statico
int Riconosci_Gesti::Riconoscimento(Project *proj,int gloveid1,int gloveid2)
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
	
	do
	{
		Pulisci_Seconda_Cornice();
		// Avvio il timer per controllare quanto tempo impiega per eseguire l'operazione.
		start = clock();

		// Avvio un apposito metodo per inizializzare le variabili utilizzate durante il funzionamento
		Inizializzatore();
		// Stampo un messaggio nella console per informare l'utente dell'inizio della scansione
		Spostati_Nella_Posizione(2,12);
		printf("Mi connetto al guanto per ottenere i dati...\n");

		// Eseguo un ciclo for per eseguire cinque scansioni
		for( int qw = 0; qw != 5; qw++ )
		{
			// Ottengo i valore dal DataGlove con ID 1
			if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"d") == 0 ) )
				dg1 = proj->GetDataGlove(gloveid1);
			// Ottengo i valore dal DataGlove con ID 2
			if ( (strcmp(tipo_guanto,"S") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) )
				dg2 = proj->GetDataGlove(gloveid2);

			// Per il guanto Destro

			// Devo realizzare due strutture differenti per la raccolta dei dati 
			// dal guanto perche' la posizione delle dita e' differente.
			// Quindi, non posso usare la stessa procedura per entrambi.
			if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"d") == 0 ) )
			{
				if ( (dg1->f[0] != 0) &&
					 (dg1->f[1] != 0) &&
			         (dg1->f[2] != 0) &&
			         (dg1->f[3] != 0) &&
			         (dg1->f[4] != 0) ) {
					if ( contatore <= 5 ) {
						// Salvo i dati in una matrice per successiva elaborazione
						for ( int gr = 0; gr <= 5; gr ++ ) {
							fMatrice[righ][gr] = dg1->f[gr];
						}
						contatore++;
						righ++; // Contatore righe
						if ( righ > 5 )
							righ = 0;
					}
					else if ( contatore > 5 ) {
						contatore = 0;
					}
				}// Fine IF
			}// Fine IF per il guanto destro
			
			// Per il guanto Sinistro
			if ( (strcmp(tipo_guanto,"S") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) )
			{
				// Eseguo controllo per verificare che le misure lette siano diverse da zero
				if ( (dg2->f[0] != 0) &&
			         (dg2->f[1] != 0) &&
			         (dg2->f[2] != 0) &&
			         (dg2->f[3] != 0) &&
			         (dg2->f[4] != 0) )
				{
					if ( contatore <= 5 )
					{
						// Salvo i dati in una matrice per successiva elaborazione
						// Per formulare i dati nella forma corretta, devo
						// posizione i parametri nella giusta posizione della matrice
						fMatrice[righ][0] = dg2->f[1];
						fMatrice[righ][1] = dg2->f[0];
						fMatrice[righ][2] = dg2->f[2];
						fMatrice[righ][3] = dg2->f[3];
						fMatrice[righ][4] = dg2->f[4];

						contatore++;
						righ++;
						if ( righ > 5 )
							righ = 0;
					}
					else if ( contatore > 5 ) {
						contatore = 0;
					}
				}// Fine IF
			}// Fine IF per il guanto Sinistro
		}// Fine For

		// Calcolo la media
		Calcola_Media();

		Spostati_Nella_Posizione(2,13);
		cout << "---Apro il file di Training per ottenere i dati---" << endl;

		// Conta il numero di file presenti nel file
		if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"d") == 0 ) )
		{// Per il guanto destro
			Conta_Elementi("glove.codf");
		}
		if ( (strcmp(tipo_guanto,"S") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) )
		{// Per il guanto sinistro
			Conta_Elementi("glove_s.codf");
		}
		
		// Inizializzo la matrice a 0 per evitare dati "corrotti"
		Inizializzazione_Matrice();
		// Tipo di file da aprire 
		// A seconda del tipo di guanto utilizzato viene scelto il relativo file
		if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"d") == 0 ) )
		{// Per il guanto destro
			if ( ( FileGlove = fopen("glove.codf","r") ) == NULL ) {
				printf("Attenzione! Il file di training non e' presente.\n");
				system("pause");
				exit(1);
			}
		}
		if ( (strcmp(tipo_guanto,"S") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) )
		{// Per il guanto sinistro
			if ( ( FileGlove = fopen("glove_s.codf","r") ) == NULL ) {
				printf("Attenzione! Il file di training non e' presente.\n");
				system("pause");
				exit(1);
			}
		}
		// Elevazione al quadrato del valore ottenuto
		for ( int ce = 0; ce < 5; ce++ ) {
			vettore_pow[ce] = pow(prova[ce],2);
		}
		// Fino alla fine del file
		while( !feof(FileGlove) ) {
			// Prelevo i dati in ingresso
			fgets(buf_dati,256,FileGlove);
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
			
			if ( vettore_corr[0] > 0 &&
				 vettore_corr[1] > 0 &&
			     vettore_corr[2] > 0 &&
			     vettore_corr[3] > 0 &&
			     vettore_corr[4] > 0 )
			{   // Per evitare dati non corretti, verifico che il valore sia maggiore di 0
				// Elevazione al quadrato mediante pow della riga corrrente
				for ( int ce = 0; ce < 5; ce++ ) {
					vettore_corr_pow[ce] = pow(vettore_corr[ce],2);
				}
				// Eseguo la differenza tra il vettore corrente in analisi
				// e le informazioni fornite dal data glove
				for ( int oz = 0; oz < 5; oz++ ) {
					val_min_global[oz] = abs(vettore_corr_pow[oz] - vettore_pow[oz]);
				}
				// Ottengo tutte le differenze
				for ( int hg = 0; hg < 5; hg++ )
				{
					//printf("%f,",val_min_global[hg]);
					Inserisci_Elem_Matr(val_min_global[hg]);
				}
				tipo_elm[CNT3] = buf_dati[45];
				CNT3++;
				Inserisci_Tipo_Matr(buf_dati[45]);
			}
		}
		// Chiudi il file
		fclose(FileGlove);
		// Calcola la somma delle disanze
		somma_distanze();
		// Metodo per cercare un gesto
		Trova_Gesto();
		// Cancella tutto quello che ha scritto
		Pulisci_Seconda_Cornice();
		Spostati_Nella_Posizione(2,14);
		cout << "Vuoi eseguire una scansione dati? (S/N) ";

		Spostati_Nella_Posizione(16,23);
		printf("  ");
		Spostati_Nella_Posizione(16,23);
		// Ottengo il valore della scelta fornita in input
		tipo_scelta = _getch();
		// Fine del ciclo do-while
   } while ( tipo_scelta != 'N' );
   // Chiude e ritorna al menu' principale
	return ESC;
}
// Questo metodo viene utilizzato per inizializzare la matrice a 0.
// In questo modo vengono eliminati tutti gli eventuali valori già
// presenti. Questo permette di evitare problemi nel caso in cui 
// la matrice venga utilizzata più volte. Infatti, potrebbero essere
// presenti valori precedentemente utilizzati.
void Riconosci_Gesti::Inizializzazione_Matrice()
{
	for ( int ni = 0; ni < totline-2; ni++ ) {
		for ( int ju = 0; ju < COL; ju++ ) {
			mat[ni][ju] = 0;
		}
	}
}

void Riconosci_Gesti::Stampa_Matrice()
{
	// stampa la matrice
	for ( int ni = 0; ni < totline-2; ni++ ) {
		for ( int ju = 0; ju < COL; ju++ ) {
			//printf(" %f ",mat[ni][ju]);
		}
		//printf("\n");
	}
}
// Metodo per trovare il gesto.
// Il metodo non fa altro che confrontare la distanza minore con il valore del vettore
// per valutare a quale valore del vettore corrisponde.
void Riconosci_Gesti::Trova_Gesto()
{
	char imag_gesto[10] = "TipoA.jpg";
	float minimo = 9.999f;
	int gesto = 0;
	// Cerco la distanza minima
	for ( int yui = 0 ; yui < totline-2; yui++ ) {
		if ( distanze[yui] < minimo && distanze[yui] > 0 ) {
			minimo = distanze[yui];
			gesto = yui;
		}
	}
	// Fine della verifica del tempo
	finish = clock();
	// Stampo il risultato in output
	Spostati_Nella_Posizione(2,21);
	printf("Tempo impiegato %f Sec.\n", ( (double)(finish - start) ) / CLOCKS_PER_SEC );

	if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"d") == 0 ) ) {
		Spostati_Nella_Posizione(2,16);
		printf("Sto ricevendo le informazioni dal guanto destro\n");
	}
	if ( (strcmp(tipo_guanto,"S") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) ) {
		Spostati_Nella_Posizione(2,16);
		printf("Sto ricevendo le informazioni dal guanto sinistro\n");
	}
	Spostati_Nella_Posizione(2,17);
	printf("Il valore minimo e' %f,IL VALORE DEL GESTO E' %d\n",minimo,gesto);

	Spostati_Nella_Posizione(2,18);
	printf("----> IL DATA GLOVE si trova in posizione: ---> %c <---.\n",tipo_elm[gesto]);
	Spostati_Nella_Posizione(2,19);
	
	imag_gesto[4] = tipo_elm[gesto];

	Spostati_Nella_Posizione(2,20);
	printf("Sto aprendo il file %s\n",imag_gesto);
	IplImage *img = cvLoadImage(imag_gesto);
	if ( !img ) 
		printf("Error: Couldn't open the image file.\n");
	// Mostra l'immagine.
	cvShowImage("Gesto Riconosciuto:", img);
	// Aspetta fino a quando l'utente non preme un tasto nella GUI window.
	cvWaitKey(0);
	// Libera le risorse utilizzate.
	cvDestroyWindow("Image:");
	cvReleaseImage(&img);
}

// Questo metodo si occupa di eseguire la somma delle distanze euclidee.
// Precedentemente era stata eseguita la differenza tra ogni singolo
// elemento dei due vettori. Uno contente il valore ottenuto dal data
// glove e l'altro contenente il valore del codevecotor.
// Il risultato di tutte queste differenze viene sommato in un unico
// valore che rappresenta la distanza che sarà utilizzata per riconoscere
// un gesto.
void Riconosci_Gesti::somma_distanze()
{
	float somma = 0;
	for ( int ni = 0; ni < totline-2; ni++ ) {
		for ( int ju = 0; ju < 5; ju++ ) {
			somma += mat[ni][ju];
		}
		distanze[ni] = somma;
		somma = 0;
	}
}

// Questo metodo si occupa di inserire un elemento all'interno
// della matrice.
void Riconosci_Gesti::Inserisci_Elem_Matr(float elemento)
{
	if ( CNT1 >= 0 && CNT1 <= 32 && CNT2 >= 0 && CNT2 <= 6 )
		mat[CNT1][CNT2] = elemento;
	if ( CNT2 >= 4 ) {
		CNT1++;
		CNT2 = 0;
	}
	else {
		CNT2++;
	}
}

// Questo metodo si occupa di inserire la tipologia di centroide
// che si sta utilizzando, quindi ad esempio, per una determinata
// quintupla esiste il valore A, B, C,ecc.
void Riconosci_Gesti::Inserisci_Tipo_Matr(float elemento)
{
	mat[tipo_cnt][5] = elemento;
	tipo_cnt++;
}

// Questo metodo si occupa di contare quanti elementi sono
// presenti nel file fornito come input.
void Riconosci_Gesti::Conta_Elementi(char nomefile[25])
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
	totline  = 0;
	// Ci dice se il file è stato aperto del tutto
	while( !feof(InFile) )
	{
		// Controllo di una stringa del file di ingresso
		fgets(stringa,256,InFile);
		// Pulisci il buffer
		memset(stringa, 0, 256);
		totline++;
	}
	fclose(InFile);
	Spostati_Nella_Posizione(2,14);
	printf("Il num di elementi presenti nel file training e' %d\n",totline-2);
}

// Questo metodo si occupa di calcolare la media 
// dei valori forniti.
void Riconosci_Gesti::Calcola_Media()
{
	// Inizializzo con valore 0 le variabili per le somme
	somma_1 = 0,somma_2 = 0,somma_3 = 0,somma_4 = 0,somma_5 = 0;
	// Inizializzo con valore 0 le variabili per le medie
	media_1 = 0,media_2 = 0,media_3 = 0,media_4 = 0,media_5 = 0;
	/// Calcolo media dito Pollice
	for (int gt = 0; gt <= 4; gt++ ) {
		somma_1 = somma_1 + fMatrice[gt][0];
	}
	media_1 = somma_1 / 5;
	/// Calcolo media dito Indice
	for (int gt = 0; gt <= 4; gt++ ) {
		somma_2 = somma_2 + fMatrice[gt][1];
	}
	media_2 = somma_2 / 5;
	/// Calcolo media dito Medio
	for (int gt = 0; gt <= 4; gt++ ) {
		somma_3 = somma_3 + fMatrice[gt][2];
	}
	media_3 = somma_3 / 5;
	/// Calcolo media dito Anulare
	for (int gt = 0; gt <= 4; gt++ ) {
		somma_4 = somma_4 + fMatrice[gt][3];
	}
	media_4 = somma_4 / 5;
	/// Calcolo media dito Mignolo
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
void Riconosci_Gesti::Normalizzazione()
{
	valore = 0.001000f;
	norm_1  = ( media_1 / 100 ) - valore;
	norm_2  = ( media_2 / 100 ) - valore;
	norm_3  = ( media_3 / 100 ) - valore;
	norm_4  = ( media_4 / 100 ) - valore;
	norm_5  = ( media_5 / 100 ) - valore;

	if ( norm_1,norm_2,norm_3,norm_4,norm_5 > 0 )
	{
		printf("   %f   %f   %f   %f   %f",norm_1/norma,
			                               norm_2/norma,
										   norm_3/norma,
										   norm_4/norma,
										   norm_5/norma);
		prova[0] = norm_1/norma;
		prova[1] = norm_2/norma;
		prova[2] = norm_3/norma;
		prova[3] = norm_4/norma;
		prova[4] = norm_5/norma;
		printf("\n");
	}
}
// Per elaborare i dati è necessario gestire un gran numero di parametri,
// questi parametri vanno riportati a zero dopo che ogni operazione che
// è stata eseguita.
// Questo metodo riporta tutti i parametri al valore 0 o minimo per il 
// rispettivo contesto.
void Riconosci_Gesti::Inizializzatore()
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
