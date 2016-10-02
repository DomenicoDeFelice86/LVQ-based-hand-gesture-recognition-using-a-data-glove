//
// Il codice sorgente in questo file si occupa principalmente di raccogliere
// dei campioni forniti dal data glove e di elaborarli in modo tale da poter
// essere successivamene utilizzati per l'addestramento di una rete neurale
// LVQ.
// Infatti questo file finale ci permettera' di "addestrare" la rete LVQ,
// in modo tale da ottenere come risultato finale i centroidi.
// Questi centroidi poi possono essere utilizzati per calcolare la distanza
// minima tra i parametri forniti dal data glove per una determinata
// scansione e la lista dei centroidi, mediante distanza euclidea.
// Il centroide che possiede la minore distanza equivale al gesto che si sta 
// cercando di identificare.
#include "stdafx.h"

int giaVerificato = 0,giaVerificato2 = 0,giaVerificato3 = 0;
int nm = 0;

// Riconoscimento gesto multiplo
int ric_gesto_multiplo = 0;
int num_scansioni;


// Questo metodo estrae i valori dal Data Glove ed 
// esegue il salvataggio dei dati sul file.
int GestioneFile::ShowValues( Project *proj, int gloveid1, int gloveid2 )
{
	// Variabile per la scelta
	char tipo_scelta = ' ';

	// Avvia il menù scelta da cui poter scegliere l'operazione da eseguire
	MenuScelta();

	// Ciclo ripetitivo per il do-while
	do
	{
		// Ottengo il valore della scelta fornita in input
		tipo_scelta = _getch();
		
		// Numero di campioni presenti nel database
		if ( tipo_scelta == 'A' || tipo_scelta == 'a' ) {
			Pulisci_Seconda_Cornice();
			Conta_Scansioni(0);
		}
		// Genera file di test e training non normalizzato
		else if ( tipo_scelta == 'B' || tipo_scelta == 'b'  ) {
			Pulisci_Seconda_Cornice();
			Salva_File_Ordinato();
		}
		// Verifica la presenza di doppioni
		else if ( tipo_scelta == 'C' || tipo_scelta == 'c'  ) {
			Pulisci_Seconda_Cornice();
			if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"d") == 0 ) )
			{
				Cerca_Doppioni("Training.db");
				Cerca_Doppioni("Test.db");
			}
			if ( (strcmp(tipo_guanto,"S") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) )
			{
				Cerca_Doppioni("Training_s.db");
				Cerca_Doppioni("Test_s.db");
			}
		}
		// Trova il valore minimo e massimo dell'insieme di esempi
		else if ( tipo_scelta == 'D' || tipo_scelta == 'd'  ) {
			// Per il guanto destro
			if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"d") == 0 ) )
			{
				Pulisci_Seconda_Cornice();
				Cerca_ValoreMinMax("Training.db");
				Cerca_ValoreMinMax("Test.db");
			}
			// Per il guanto sinistro
			if ( (strcmp(tipo_guanto,"S") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ))
			{
				Pulisci_Seconda_Cornice();
				Cerca_ValoreMinMax("Training_s.db");
				Cerca_ValoreMinMax("Test_s.db");
			}
		}
		// Genera file di test e training normalizzato
		else if ( tipo_scelta == 'E' || tipo_scelta == 'e'  ) {
			if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"d") == 0 ) )
			{// Guanto Destro
				Pulisci_Seconda_Cornice();
				Crea_File_Normalizzato("Training.db");
				Crea_File_Normalizzato("Test.db");
			}
			if ( (strcmp(tipo_guanto,"S") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) )
			{// Guanto Sinistro
				Pulisci_Seconda_Cornice();
				Crea_File_Normalizzato("Training_s.db");
				Crea_File_Normalizzato("Test_s.db");
			}
		}
		// Esegui una scansione singola dei dati
		else if ( tipo_scelta == 'F' || tipo_scelta == 'f'  )
		{
			// Ciclo for per ottenere le scansioni dal data glove
			// Vengono eseguite cinque scansioni
			Scarica_Info_DataGlove(proj,gloveid1,gloveid2);
			// Pulisco lo schermo e stampo la matrice con i dati ottenuti
			Pulisci_Seconda_Cornice();
			Stampa_Matrice();
			// Calcolo la media
			Calcola_Media();
		}
		// Esegui una scansione multipla dei dati
		else if ( tipo_scelta == 'G' || tipo_scelta == 'g'  )
		{
			Pulisci_Seconda_Cornice();
			ric_gesto_multiplo = 1;
			Spostati_Nella_Posizione(2,12);
			printf("Inserisci il numero di scansioni multiple che vuoi eseguire: ");
			scanf("%d",&num_scansioni);
			// Richiede all'utente il tipo di gesto eseguito
			Spostati_Nella_Posizione(2,13);
			cout << "Indicare il tipo di gesto per il riconoscimento multiplo: ";
			scanf("%s",&tipo_gesto_multiplo);
			Spostati_Nella_Posizione(2,14);
			printf("Eseguo %d scansioni del gesto %s.\n",num_scansioni,tipo_gesto_multiplo);
			// Inizio la procedura
			do
			{
				// Ciclo for per ottenere le scansioni dal data glove
				// Vengono eseguite cinque scansioni
				Scarica_Info_DataGlove(proj, gloveid1, gloveid2);
				// Pulisco lo schermo e stampo
				Pulisci_Seconda_Cornice();
				Stampa_Matrice();
				// Calcolo la media
				Calcola_Media();
			} while ( num_scansioni != 0 );
			ric_gesto_multiplo = 0;
		}//////////////////
		// Fine scelta opzioni disponibili
	} while ( tipo_scelta != ESC);
	// Fine
	return ESC;
}


// Questo metodo procede a stabilire la connessione con il data glove mediante SDK.
// Una volta stabilita la connessione si procede a scaricare le informazioni.
// Le informazioni vengono verificate ( se sono maggiori di 0 ) e si procede
// con il salvataggio dei parametri in una matrice per successiva elaborazioine.
void GestioneFile::Scarica_Info_DataGlove(Project *proj, 
	                                       int gloveid1,
										   int gloveid2)
{
	// Dichiarazione variabile file
	FILE *File1;
	// Contatore
	int contatore = 0,righ = 0;

	// Ottengo cinque scansioni in successione
	for( int qw = 0; qw != 5; qw++ )
	{
		// Ottengo i valore dal DataGlove con ID 1
		if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"d") == 0 ))
			dg1 = proj->GetDataGlove(gloveid1);
		// Ottengo i valore dal DataGlove con ID 2
		if ( (strcmp(tipo_guanto,"S") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ))
			dg2 = proj->GetDataGlove(gloveid2);

		// Per salvare i valori appena letti abbiamo bisogno di un file
		// Verifico se il file esiste già
		if ( giaVerificato == 0 )
		{
			giaVerificato = 1;
			if ( (FileExists("dati_originali.db") ) == true ) {
				// Non fa nulla qui
			}
			else if ( (FileExists("dati_originali.db") ) == false )
			{// Se il file non esiste ne creo uno!
				cout << "Il file non esiste." << endl;
				File1 = fopen("dati_originali.db","w");
				if( File1 == NULL )
					cout << "\n Errore in apertura file d'uscita!" << endl;
				fprintf(File1,"Questo file contiene i dati originali, cosi come sono stati ricevuti\n");
				fclose(File1);
			}
		}
		// Apertura del file per salvare le informazioni ottenute
		File1 = fopen("dati_originali.db","a");
		if( File1 == NULL )
			cout << "\n Errore in apertura file d'uscita!" << endl;
		
		/// INIZIO IF ///
		// Verifico se i valori sono diversi da 0 , poichè se sono uguali a 0 molto probabilmente  
		// il data glove non sta inviando alcun segnale oppure i dati sono corrotti.
		if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"d") == 0 ) )
		{ // Per il guanto destro
			if ( (dg1->f[0] != 0) && 
			     (dg1->f[1] != 0) && 
			     (dg1->f[2] != 0) && 
			     (dg1->f[3] != 0) && 
			     (dg1->f[4] != 0) )
			{// Contatore per verificare se si raggiunge il numero massimo 5
				if ( contatore <= 5 )
				{
					// Salvo su file
					fprintf(File1,"%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f",
						dg1->f[0],dg1->f[1],dg1->f[2],dg1->f[3],dg1->f[4],
						dg1->roll,dg1->pitch,dg1->ax,dg1->ay,dg1->az);
					fprintf(File1,"\n");
				
					// Salvo i dati in una matrice per successiva elaborazione
					for ( int gr = 0; gr <= 5; gr ++ )
					{
						fMatrice[righ][gr] = dg1->f[gr];
					}
					contatore++;
					righ++;// Contatore righe
					if ( righ > 5 )
						righ = 0;// Se arriva alla quinta riga,resetta a 0
				}
				else if ( contatore > 5 )
				{// Il contatore viene resettato
					contatore = 0;
				}
			}
		}
		// Per il guanto sinistro
		if ( (strcmp(tipo_guanto,"S") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) )
		{// Verifico che i parametri siano diversi da zero.
			if ( (dg2->f[0] != 0) &&
			     (dg2->f[1] != 0) &&
			     (dg2->f[2] != 0) &&
			     (dg2->f[3] != 0) &&
			     (dg2->f[4] != 0) )
			{
				if ( contatore <= 5 )
				{// Salvo su file dati non normalizzati (cosi come sono stati ricevuti).
				 // Il guanto sinistro invia le informazioni riguardo i parametri
				 // delle dita in senso inverso rispetto a quelli del guanto destro.
					fprintf(File1,"%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f",
						dg2->f[4],dg2->f[3],dg2->f[2],dg2->f[1],dg2->f[0],
						dg2->roll,dg2->pitch,dg2->ax,dg2->ay,dg2->az);
					fprintf(File1,"\n");
				
					// Salvo i dati in una matrice per successiva elaborazione.
					// Il contatore è inverso rispetto alla versione 
					// precedente, vado da 4 verso 0.
					fMatrice[righ][0] = dg2->f[1];
					fMatrice[righ][1] = dg2->f[0];
					fMatrice[righ][2] = dg2->f[2];
					fMatrice[righ][3] = dg2->f[3];
					fMatrice[righ][4] = dg2->f[4];

					contatore++;
					righ++; // Contatore righe
					if ( righ > 5 )
						righ = 0;// Se arriva alla quinta riga,resetta a 0
				}
				else if ( contatore > 5 ) {
					contatore = 0;
				}
			}
		}
		/// FINE IF   /// 
		// Chiudo il File
		fclose(File1);
		// Mette in pausa per evitare di eseguire scansioni troppo ravvicinate
		Sleep(100);
	}
}


// Permette di stampare la matrice ottenuta dai precedenti calcoli
void GestioneFile::Stampa_Matrice()
{
	int incremento = 2;
	int posiz = 14;
	Spostati_Nella_Posizione(2,12);
	printf("Scansione dati rilevati dal Data Glove,eseguo cinque scansioni:");
	// Stampo la matrice dove sono stati salvati i dati
	for ( int bn = 0; bn <= 4; bn++ ) {
		for ( int bm = 0; bm <= 4; bm++ ) {
			Spostati_Nella_Posizione(incremento,posiz);
			printf( " %f ",fMatrice[bn][bm] );
			incremento += 9;
			if ( bm == 4 ) {
				incremento = 2;
				printf("\n");
			}
		}
		posiz++;// Incremento il contatore della posizione, viene utilizzato per la console.
	}
}

// Permette di scegliere il tipo di operazione da eseguire
void GestioneFile::MenuScelta()
{
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

// Questo metodo calcola la media dei dati ottenuti precedentemente
void GestioneFile::Calcola_Media()
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
		// Sto sommando somma_4 e fMatrice[gt][3]);
		somma_4 = somma_4 + fMatrice[gt][3];
	}
	media_4 = somma_4 / 5;

	/// Calcolo media dito Mignolo ///
	for (int gt = 0; gt <= 4; gt++ ) {
		// Sto sommando somma_5 e fMatrice[gt][4]);
		somma_5 = somma_5 + fMatrice[gt][4];
	}
	media_5 = somma_5 / 5;
	// Invoco il metodo per normalizzare i dati ottenuti
	Normalizzazione();
}

// In senso lato con la normalizzazione si intende un qualsiasi processo
// che modifica un oggetto per renderlo più normale, il che significa 
// tipicamente renderlo più conforme a qualche criterio di regolarità, 
// oppure di rendere alcune sue caratteristiche più vicine a quelle 
// che si riscontrano mediamente, oppure di farlo ritornare ad una 
// situazione più usuale dopo un evento che l'ha portato ad uno 
// stato anormale.
// La normalizzazione in matematica consiste nel dividere tutti i termini 
// di un'espressione per uno stesso fattore.
void GestioneFile::Normalizzazione()
{
	int giaVerificato2 = 0;
	FILE *database;
	valore = 0.001000f;
	norm_1  = ( media_1 / 100 ) - valore;
	norm_2  = ( media_2 / 100 ) - valore;
	norm_3  = ( media_3 / 100 ) - valore;
	norm_4  = ( media_4 / 100 ) - valore;
	norm_5  = ( media_5 / 100 ) - valore;

	// Verifico se il file esiste già
	if ( giaVerificato2 == 0 ) {
		giaVerificato2 = 1;
		if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"d") == 0 ) )
		{
			if ( (FileExists("dataglove.db") ) == true ) {
			}
			else if ( (FileExists("dataglove.db") ) == false )
			{
				cout << "Il file non esiste" << endl;
				database = fopen("dataglove.db","w");
				if( database == NULL )
					cout <<"\n Errore in apertura file d'uscita!"<<endl;
				fprintf(database,"5 lvq\n");
				fclose(database);
			}
		}// Fine if
		if ( (strcmp(tipo_guanto,"S") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) )
		{
			if ( (FileExists("dataglove_s.db") ) == true ) {
			}
			else if ( (FileExists("dataglove_s.db") ) == false ) {
				cout << "Il file non esiste" << endl;
				database = fopen("dataglove_s.db","w");
				if( database == NULL )
					cout <<"\n Errore in apertura file d'uscita!"<<endl;
				fprintf(database,"5 lvq\n");
				fclose(database);
			}
		}// Fine if
	}
	/// Dopo aver verificato l'esistenza del file procedo a salvare
	/// i dati.
	if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"d") == 0 ) ) {
		database = fopen("dataglove.db","a");
		if( database == NULL )
			cout <<"\n Errore in apertura file d'uscita!"<<endl;
	}
	if ( (strcmp(tipo_guanto,"S") == 0 ) || (strcmp(tipo_guanto,"s") == 0 )) {
		database = fopen("dataglove_s.db","a");
		if( database == NULL )
			cout <<"\n Errore in apertura file d'uscita!"<<endl;
	}
	char tipo[2];
	// Richiede all'utente il tipo di gesto eseguito
	if ( ric_gesto_multiplo == 0 )
	{
		Spostati_Nella_Posizione(2,20);
		cout << "Indicare il tipo di posizione del DataGlove: ";
		scanf("%s",&tipo);
	}
	if ( ric_gesto_multiplo == 1 ){// Copio il tipo di gesto multiplo in tipo
		strcpy(tipo,tipo_gesto_multiplo);
		num_scansioni--;
	}
	if ( norm_1,norm_2,norm_3,norm_4,norm_5 > 0 ) {
		fprintf(database,"   %f   %f   %f   %f   %f   %s",norm_1,norm_2,norm_3,norm_4,norm_5,tipo);
		fprintf(database,"\n");
	}
	fclose(database);
	// Cancello tutti i dati ancora presenti
	flush(cout);
	Spostati_Nella_Posizione(16,23);
	printf("  ");
	Spostati_Nella_Posizione(16,23);
}

//
// Questo metodo verifica se il file in input esiste
// o meno.
bool GestioneFile::FileExists(string strFilename) 
{
	struct stat stFileInfo;
	bool blnReturn;
	int intStat;
	// Cerca di ottenere gli attributi di un file 
	intStat = stat(strFilename.c_str(),&stFileInfo);
	if( intStat == 0 ) {
		// Dato che siamo in grado di ottenere gli attributi del file
		// il file deve esistere.
		blnReturn = true;
	} 
	else {
		// Non siamo stati in grado di ottenere gli attributi del file
		// Questo può indicare che non abbiamo i permessi per accedere
		// al file.  
		// Per verificare basta semplicemente analizzare i valori
		// restituiti in modo da sapere il risultato.
		blnReturn = false;
	}
	return(blnReturn);
}

// Questo metodo legge il file data glove contando il tipo di scansione che è stata 
// effettuata, ad esempio, per ogni linea di file che trova legge la tipologia associata
// che è indicata da una lettera.
void GestioneFile::Conta_Scansioni(int no_output)
{
	// Inizializzo tutti i contatori a 0
	contaA = 0,contaB = 0,contaC = 0,contaD = 0,contaE = 0,contaF = 0;
	contaG = 0,contaH = 0,contaI = 0,contaL = 0,contaM = 0,contaN = 0;
	contaO = 0,contaP = 0,contaQ = 0,contaR = 0,contaS = 0,contaT = 0;
	contaU = 0,contaV = 0,contaZ = 0,contaJ = 0,contaK = 0,contaW = 0;
	contaX = 0,contaY = 0,conta3 = 0,conta4 = 0,conta5 = 0;
	// Variabili per la gestione del file
	FILE *InFile;
	// Dichiarazione stringhe su cui lavorare
	char stringa[256];
	// Dichiarazione delle altre variabili
	int i;
	// File da usare per il guanto destro
	if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"d") == 0 ) ) {
		// apertura file e controllo d'ingresso
		InFile = fopen("dataglove.db","r");
		// Controllo dell'avvenuta apertura del file
		if( InFile == NULL )
			cout << "\n Errore in apertura file!" << endl;
	}
	// File da usare per il guanto sinistro
	if ( (strcmp(tipo_guanto,"S") == 0 ) || (strcmp(tipo_guanto,"s") == 0 )) {
		// apertura file e controllo d'ingresso
		InFile = fopen("dataglove_s.db","r");
		// Controllo dell'avvenuta apertura del file
		if( InFile == NULL )
			cout << "\n Errore in apertura file!" << endl;
	}
	totline  = 0;
	// Ci dice se il file è stato aperto del tutto
	while( !feof(InFile) ) {
		// Controllo di una stringa del file di ingresso
		fgets(stringa,256,InFile);
		// Conta 
		i = 0;
		// Controlla se non è finita la stringa
		while( (stringa[i] != '\0') && ( stringa[i] != '\n' ) ) {
			i++;
			if      (stringa[i] == 'A')
				contaA++;
			else if (stringa[i] == 'B')
				contaB++;
			else if (stringa[i] == 'C')
				contaC++;
			else if (stringa[i] == 'D')
				contaD++;
			else if (stringa[i] == 'E')
				contaE++;
			else if (stringa[i] == 'F')
				contaF++;
			else if (stringa[i] == 'G')
				contaG++;
			else if (stringa[i] == 'H')
				contaH++;
			else if (stringa[i] == 'I')
				contaI++;
			else if (stringa[i] == 'L')
				contaL++;
			else if (stringa[i] == 'M')
				contaM++;
			else if (stringa[i] == 'N')
				contaN++;
			else if (stringa[i] == 'O')
				contaO++;
			else if (stringa[i] == 'P')
				contaP++;
			else if (stringa[i] == 'Q')
				contaQ++;
			else if (stringa[i] == 'R')
				contaR++;
			else if (stringa[i] == 'S')
				contaS++;
			else if (stringa[i] == 'T')
				contaT++;
			else if (stringa[i] == 'U')
				contaU++;
			else if (stringa[i] == 'V')
				contaV++;
			else if (stringa[i] == 'Z')
				contaZ++;
			else if (stringa[i] == 'J')
				contaJ++;
			else if (stringa[i] == 'K')
				contaK++;
			else if (stringa[i] == 'W')
				contaW++;
			else if (stringa[i] == 'X')
				contaX++;
			else if (stringa[i] == 'Y')
				contaY++;
		}
		memset(stringa, 0, 256);
		totline++;
	}
	// Chiudo il file
	fclose(InFile);
	//////////////////////// Conto solo i numeri ///////////////////////////////////////
	char ch;
	char buffer[256];
	int  cnt = 0;
	FILE *num;
	// Per il guanto destro
	if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"d") == 0 ) ) {
		if ( ( num = fopen("dataglove.db","r") ) == NULL ) {
			printf("Cannot open file.\n");
			exit(1);
		}
	}
	// Per il guanto sinistro
	if ( (strcmp(tipo_guanto,"S") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) ) {
		if ( ( num = fopen("dataglove_s.db","r") ) == NULL ) {
			printf("Cannot open file.\n");
			exit(1);
		}
	}
	// Continua a prelevare dati fino alla fine del file
	while ( ( ch = fgetc( num ) ) != EOF ) {
		cnt++;
		if ( cnt == 256 )
			cnt = 0;
		buffer[cnt] = ch;
		if ( buffer[cnt] == '\n'  ) {
			int testing;
			// Converto da char a int per poter eseguire il controllo
			testing = atoi(&buffer[cnt-1]);
			if ( testing > 0 )
			{
				if      ( buffer[cnt-1] == '3' )
					conta3++;// Incrementa il valore 3
				else if ( buffer[cnt-1] == '4' )
					conta4++;// incrementa il valore 4
				else if ( buffer[cnt-1] == '5' )
					conta5++; // Incrementa il valore  5
			}
		}
	}
	fclose( num );

	if ( no_output == 1 ) {
			Spostati_Nella_Posizione(2,12);
			printf("Nel database sono presenti %d gesti.\n",totline-2);
	}
	else if ( no_output == 0 ) {
		Spostati_Nella_Posizione(2,12);
		printf("Nel file sono presenti %d elementi del seguente tipo:\n",totline-2);
		Spostati_Nella_Posizione(2,13);
		printf("A: %d,B: %d,C: %d,D: %d,E: %d\n",contaA,contaB,contaC,contaD,contaE);
		Spostati_Nella_Posizione(2,14);
		printf("F: %d,G: %d,H: %d,L: %d,U: %d\n",contaF,contaG,contaH,contaL,contaU);
		Spostati_Nella_Posizione(2,15);
		printf("W: %d,Y: %d,4: %d\n",contaW,contaY,conta4);
		Spostati_Nella_Posizione(16,23);
		printf("  ");
		Spostati_Nella_Posizione(16,23);
	}
}

// Questo metodo permette di salvare su file le informazioni ottenute,
// prima di procedere al salvataggio dei dati, questi vengono ordinati.
void GestioneFile::Salva_File_Ordinato()
{
	contatore_A = 0,contatore_B = 0,contatore_C = 0,contatore_F = 0,contatore_L = 0,
	contatore_P = 0,contatore_U = 0,contatore_V = 0,contatore_W = 0,contatore_Y = 0;
	// Per prima cosa conto le scansioni
	Conta_Scansioni(1);
	// Variabili per la gestione del file
	FILE *FileTraining,*FileTest;

	/////////////////////// Apertura file Training ///////////////////
	if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"d") == 0 ) ) {
		FileTraining = fopen("Training.db","w");
		if ( FileTraining == NULL )
			cout <<"\n Errore in apertura file d'uscita!" << endl;
		fprintf(FileTraining,"5 lvq\n");
		Spostati_Nella_Posizione(2,13);
		printf("Ho generato con successo il file Training.db...\n");
	}
	if ( (strcmp(tipo_guanto,"S") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) ) {
		FileTraining = fopen("Training_s.db","w");
		if( FileTraining == NULL )
			cout <<"\n Errore in apertura file d'uscita!"<<endl;
		fprintf(FileTraining,"5 lvq\n");
		Spostati_Nella_Posizione(2,13);
		printf("Ho generato con successo il file Training_s.db...\n");
	}
	////////////////////// Apertura file Test ///////////////////////
	if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"d") == 0 ) ) {
		FileTest = fopen("Test.db","w");
		if ( FileTraining == NULL )
			cout <<"\n Errore in apertura file d'uscita!"<<endl;
		fprintf(FileTest,"5 lvq\n");
		Spostati_Nella_Posizione(2,14);
		printf("Ho generato con successo il file Test.db...\n");
	}
	if ( (strcmp(tipo_guanto,"S") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) ) {
		FileTest = fopen("Test_s.db","w");
		if ( FileTraining == NULL )
			cout <<"\n Errore in apertura file d'uscita!"<<endl;
		fprintf(FileTest,"5 lvq\n");
		Spostati_Nella_Posizione(2,14);
		printf("Ho generato con successo il file Test_s.db...\n");
	}

	fclose(FileTraining);
	fclose(FileTest);

	Controllo(FileTraining,FileTest,'A',contaA,0);
	Controllo(FileTraining,FileTest,'B',contaB,0);
	Controllo(FileTraining,FileTest,'C',contaC,0);
	Controllo(FileTraining,FileTest,'D',contaD,0);
	Controllo(FileTraining,FileTest,'E',contaE,0);
	Controllo(FileTraining,FileTest,'F',contaF,0);
	Controllo(FileTraining,FileTest,'G',contaG,0);
	Controllo(FileTraining,FileTest,'H',contaH,0);
	Controllo(FileTraining,FileTest,'L',contaL,0);
	Controllo(FileTraining,FileTest,'U',contaU,0);
	Controllo(FileTraining,FileTest,'W',contaW,0);
	Controllo(FileTraining,FileTest,'Y',contaY,0);
	Controllo(FileTraining,FileTest,'4',conta4,1);

	Spostati_Nella_Posizione(16,23);
	printf("  ");
	Spostati_Nella_Posizione(16,23);
}

void GestioneFile::Controllo(FILE *FileTraining,FILE *FileTest,char let,int num,int numerico)
{
	FILE *FileDatabase;
	char buf_dati[256];
	int  cnt = 0;
	int y,contatore_num = 0;

	/// Apertura file database ///
	if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"d") == 0 ) ) {
		if ( ( FileDatabase = fopen("dataglove.db","r") ) == NULL ) {
			printf("Cannot open file.\n");
			exit(1);
		}
	}
	if ( (strcmp(tipo_guanto,"S") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) ) {
		if ( ( FileDatabase = fopen("dataglove_s.db","r") ) == NULL ) {
			printf("Cannot open file.\n");
			exit(1);
		}
	}
	////////////////////////////////////////////////////////////////
	if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"d") == 0 ) ) {
		FileTraining = fopen("Training.db","a");
		if( FileTraining == NULL )
			cout <<"\n Errore in apertura file d'uscita!"<<endl;
	}
	if ( (strcmp(tipo_guanto,"S") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) ) {
		FileTraining = fopen("Training_s.db","a");
		if( FileTraining == NULL )
			cout <<"\n Errore in apertura file d'uscita!"<<endl;
	}
	////////////////////////////////////////////////////////////////
	if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"d") == 0 ) ) {
		FileTest = fopen("Test.db","a");
		if( FileTraining == NULL )
			cout <<"\n Errore in apertura file d'uscita!"<<endl;
	}
	if ( (strcmp(tipo_guanto,"S") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) ) {
		FileTest = fopen("Test_s.db","a");
		if( FileTraining == NULL )
			cout <<"\n Errore in apertura file d'uscita!"<<endl;
	}
	////////// Leggo il file database fino alla fine ////////////////
	if ( numerico == 0 ) {
		while( !feof(FileDatabase) ) {
			// Controllo di una stringa del file di ingresso
			fgets(buf_dati,256,FileDatabase);
			// Conta 
			y = 0;
			// vede se non e' finita la stringa
			while( (buf_dati[y] != '\0') && ( buf_dati[y] != '\n' ) ) {
				y++;
				if ( buf_dati[y] == let )
				{
					// La salvo su file le stringhe di tipo A
					if ( contatore_num < (num/2) ) {
						contatore_num++;
						fprintf(FileTraining,"%s",buf_dati);
					}
					else {
						fprintf(FileTest,"%s",buf_dati);
					}
				}
			}
		}
	}
	else if ( numerico == 1 )
	{
		while( !feof(FileDatabase) )
		{
			// Controllo di una stringa del file di ingresso
			fgets(buf_dati,256,FileDatabase);
			// Conta 
			y = 0;
			// vede se non e' finita la stringa
			while( (buf_dati[y] != '\0') && ( buf_dati[y] != '\n' ) )
			{
				y++;
				if ( buf_dati[y] == '\n' )
				{
					if ( atoi(&buf_dati[y-1]) == atoi(&let) )
					{
						// Salvo su file le stringhe di tipo A
						if ( contatore_num < (num/2) ) {
							contatore_num++;
							fprintf(FileTraining,"%s",buf_dati);
						}
						else {
							fprintf(FileTest,"%s",buf_dati);
						}
					}
				}
			}
		}
	}
	fclose(FileDatabase);
	fclose(FileTraining);
	fclose(FileTest);
}


// Questo metodo non fa altro che scegliere una riga partendo dalla prima verso il basso.
// Questa riga rimane uguale finchè non viene raggiunta la fine del file, quando è stata
// raggiunta la fine del file, viene scelta la riga successiva e viene eseguita la
// scansione di tutto il file sempre fino alla fine, e via dicendo.
// Quando vengono trovete due stringhe uguali viene stampato in ouput un messaggio
// indicante la presenza di due righe uguali.
void GestioneFile::Cerca_Doppioni(char nomefile[25])
{
	char buf_dati[256];
	char daControllare[256];
	char attuale[256];
	int  turno = 0,PrimaVolta = 0,num_dop = 0;
	cnt_fine      = 2;
	FILE *File;

	do
	{
		cnt_duplicati = 0;
		// Apertura file
		File = fopen(nomefile,"r");
		if ( File == NULL )
			cout <<"\n Errore in apertura file d'uscita!"<<endl;
		
		Spostati_Nella_Posizione(2,12);
		printf("Cerco l'eventuale presenza di doppioni nel file %s.       \n",nomefile);
		
		// Fino alla fine del file
		while( !feof(File) )
		{
			// Prelevo i dati in ingresso
			fgets(buf_dati,256,File);
			cnt_duplicati++;
			if ( turno == 0 ) {
				if ( cnt_duplicati == cnt_fine )
				{
					turno = 1;
					strcpy(daControllare,buf_dati);
				}
			}
			else if ( turno == 1 ) {
				strcpy(attuale,buf_dati);
				if ( (strcmp (daControllare,attuale) == 0 ) ) {
					Spostati_Nella_Posizione(2,13+num_dop);
					printf("Stringa:%s\n",daControllare);
					num_dop++;
					Spostati_Nella_Posizione(2,13+num_dop);
					printf("Stringa:%s\n",attuale);
				}
			}
		}
		// Chiudi il file
		fclose(File);
		// Incrementa il contatore
		cnt_fine++;
		// Inizializza il turno a 0
		turno = 0;
	} while ( cnt_fine != cnt_duplicati-1 );
	
	Spostati_Nella_Posizione(16,23);
	printf("  ");
	Spostati_Nella_Posizione(16,23);
}
// Questo metodo si occupa di trovare il valore massimo e minimo.
void GestioneFile::Cerca_ValoreMinMax(char nomefile[25])
{
	FILE *File;
	char buf_dati[256];
	float valmin_1 = 0.123180f,valmin_2 = 0.123180f,valmin_3 = 0.123180f,valmin_4 = 0.123180f,valmin_5 = 0.123180f;
	float valmax_1 = 0.000000f,valmax_2 = 0.000000f,valmax_3 = 0.000000f,valmax_4 = 0.000000f,valmax_5 = 0.000000f;

	float val1 = 0.1f,val2= 0.1f,val3= 0.1f,val4= 0.1f,val5= 0.1f;
	int cambio;
	char numf[256];
	//
	File = fopen(nomefile,"r");
	if( File == NULL )
		cout <<"\n Errore in apertura file d'uscita!"<<endl;

	int zx = 3;
	int pi = 14;
	int ty = 25;
	int qp = 36;
	int kj = 47;
	// Fino alla fine del file
	while( !feof(File) )
	{
		// Prelevo i dati in ingresso
		fgets(buf_dati,256,File);
		//printf("Sto leggendo %s\n",buf_dati);
		// Primo valore numerico Float
		for ( int qa = 0; qa <= 7; qa++ ) {
			numf[qa] = buf_dati[zx];
			zx++;
		}
		numf[8] = '\0';
		zx = 3;
		val1 = (float)atof(numf);
		memset(numf, 0, 8);
		// Secondo valore numerico float
		for ( int qa = 0; qa <= 7; qa++ ) {
			numf[qa] = buf_dati[pi];
			pi++;
		}
		numf[8] = '\0';
		pi = 14;
		val2 = (float)atof(numf);
		memset(numf, 0, 8);
		// Terzo valore numerico float
		for ( int qa = 0; qa <= 7; qa++ ) {
			numf[qa] = buf_dati[ty];
			ty++;
		}
		numf[8] = '\0';
		ty = 25;
		val3 = (float)atof(numf);
		memset(numf, 0, 8);
		// Quarto valore numerico float
		for ( int qa = 0; qa <= 7; qa++ ) {
			numf[qa] = buf_dati[qp];
			qp++;
		}
		numf[8] = '\0';
		qp = 36;
		val4 = (float)atof(numf);
		memset(numf, 0, 8);
		// Quinto valore numerico float
		for ( int qa = 0; qa <= 7; qa++ ) {
			numf[qa] = buf_dati[kj];
			kj++;
		}
		numf[8] = '\0';
		kj = 47;
		val5 = (float)atof(numf);
		memset(numf, 0, 8);
		
		
		// Verifica del valore Minimo 1 (POLLICE)
		if ( val1 < valmin_1 && val1 > 0 ) {
			valmin_1 = val1;
		}
		else if ( val1 > valmax_1 && val1 > 0 ) {
			valmax_1 = val1;
		}
		// Verifica del valore minimo 2 (INDICE)
		if ( val2 < valmin_2 && val2 > 0 ) {
			valmin_2 = val2;
		}
		else if ( val2 > valmax_2 && val2 > 0 ) {
			valmax_2 = val2;
		}
		// Verifica del valore minimo 3 (MEDIO)
		if ( val3 < valmin_3 && val3 > 0 ) {
			valmin_3 = val3;
		}
		else if ( val3 > valmax_3 && val3 > 0 ) {
			valmax_3 = val3;
		}
		// Verifica del valore minimo 4 (ANULARE)
		if ( val4 < valmin_4 && val4 > 0 ) {
			valmin_4 = val4;
		}
		else if ( val4 > valmax_4 && val4 > 0 ) {
			valmax_4 = val4;
		}
		// Verifica del valore minimo 5 (MIGNOLO)
		if ( val5 < valmin_5 && val5 > 0 ) {
			valmin_5 = val5;
		}
		else if ( val5 > valmax_5 && val5 > 0 ) {
			valmax_5 = val5;
		}
	}
	if ( (strcmp(nomefile,"Test_s.db") == 0 ) )
		cambio = 2;
	if ( (strcmp(nomefile,"Training_s.db") == 0 ) )
		cambio = 39;
		
	// Stampo in output il risultato
	Spostati_Nella_Posizione(cambio,12);
	printf("%s:Minimo  POLLICE %f\n",nomefile,valmin_1);
	Spostati_Nella_Posizione(cambio,13);
	printf("%s:Massimo POLLICE %f\n",nomefile,valmax_1);
	Spostati_Nella_Posizione(cambio,14);
	printf("%s:Minimo   INDICE %f\n",nomefile,valmin_2);
	Spostati_Nella_Posizione(cambio,15);
	printf("%s:Massimo  INDICE %f\n",nomefile,valmax_2);
	Spostati_Nella_Posizione(cambio,16);
	printf("%s:Minimo    MEDIO %f\n",nomefile,valmin_3);
	Spostati_Nella_Posizione(cambio,17);
	printf("%s:Massimo   MEDIO %f\n",nomefile,valmax_3);
	Spostati_Nella_Posizione(cambio,18);
	printf("%s:Minimo  ANULARE %f\n",nomefile,valmin_4);
	Spostati_Nella_Posizione(cambio,19);
	printf("%s:Massimo ANULARE %f\n",nomefile,valmax_4);
	Spostati_Nella_Posizione(cambio,20);
	printf("%s:Minimo  MIGNOLO %f\n",nomefile,valmin_5);
	Spostati_Nella_Posizione(cambio,21);
	printf("%s:Massimo MIGNOLO %f\n",nomefile,valmax_5);

	fclose(File);
	Spostati_Nella_Posizione(16,23);
	printf("  ");
	Spostati_Nella_Posizione(16,23);
}

// Questo metodo apre il file input per creare il file normalizzato
// di test oppure training. Ogni valore viene diviso per un valore
// e poi viene salvato su un nuovo file.
// L'estensione del file termina con la lettera m.
void GestioneFile::Crea_File_Normalizzato(char nomefile[25])
{
	Conta_Scansioni(1);
	// Inizializzo le variabili
	float val1 = 0.1f,val2= 0.1f,val3= 0.1f,val4= 0.1f,val5= 0.1f;
	int zx = 3;
	int pi = 14;
	int ty = 25;
	int qp = 36;
	int kj = 47;
	// Contatore per il salvataggio su file
	int cnt_salvafile = 0;
	// Variabili per gestire il file
	FILE *File,*FileScrivere;
	// Buffer dati per salvare i dati
	char buf_dati[256];
	char numf[256];
	char file_da_creare[256];
	// Preparare il file su cui scrivere
	strcpy(file_da_creare,nomefile);
	// Aggiungo una m alla fine
	strcat(file_da_creare,"m");
	
	// Apre File di lettura
	File = fopen(nomefile,"r");
	if( File == NULL )
		cout <<"\n Errore in apertura file d'uscita!" << endl;
	// Apre File di Scrittura
	FileScrivere = fopen(file_da_creare,"w");
	if( FileScrivere == NULL )
		cout <<"\n Errore in apertura file d'uscita!"<<endl;
	fprintf(FileScrivere,"5 lvq\n");

	// Fino alla fine del file
	while( !feof(File) )
	{
		// Prelevo i dati in ingresso
		fgets(buf_dati,256,File);
		//printf("Sto leggendo %s\n",buf_dati);
		// Primo valore numerico Float
		for ( int qa = 0; qa <= 7; qa++ ) {
			numf[qa] = buf_dati[zx];
			zx++;
		}
		numf[8] = '\0';
		zx = 3;
		val1 = (float)atof(numf);
		memset(numf, 0, 8);
		// Secondo valore numerico float
		for ( int qa = 0; qa <= 7; qa++ ) {
			numf[qa] = buf_dati[pi];
			pi++;
		}
		numf[8] = '\0';
		pi = 14;
		val2 = (float)atof(numf);
		memset(numf, 0, 8);
		// Terzo valore numerico float
		for ( int qa = 0; qa <= 7; qa++ ) {
			numf[qa] = buf_dati[ty];
			ty++;
		}
		numf[8] = '\0';
		ty = 25;
		val3 = (float)atof(numf);
		memset(numf, 0, 8);
		// Quarto valore numerico float
		for ( int qa = 0; qa <= 7; qa++ ) {
			numf[qa] = buf_dati[qp];
			qp++;
		}
		numf[8] = '\0';
		qp = 36;
		val4 = (float)atof(numf);
		memset(numf, 0, 8);
		// Quinto valore numerico float
		for ( int qa = 0; qa <= 7; qa++ ) {
			numf[qa] = buf_dati[kj];
			kj++;
		}
		numf[8] = '\0';
		kj = 47;
		val5 = (float)atof(numf);
		memset(numf, 0, 8);

		if ( ( val1,val2,val3,val4,val5 > 0 ) && cnt_salvafile < ((totline-2)/2)  )
		{
			cnt_salvafile++;
			fprintf(FileScrivere,"   %f   %f   %f   %f   %f   %c"
				,val1/norma,val2/norma,val3/norma,val4/norma,val5/norma,buf_dati[58]);
			fprintf(FileScrivere,"\n");
		}
	}
	Spostati_Nella_Posizione(2,15);
	printf("Ho salvato su file %s i dati normalizzati\n",file_da_creare);
	fclose(File);
	fclose(FileScrivere);

	Spostati_Nella_Posizione(16,23);
	printf("  ");
	Spostati_Nella_Posizione(16,23);
}


