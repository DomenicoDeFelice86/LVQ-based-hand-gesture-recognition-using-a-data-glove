// Gestione comandi per il PowerPoint
// Questa classe si occupa principalmente della gestione della comunicazione
// tra il PowerPoint ed il data glove.
#include "stdafx.h"

char Nome_PP[59] = {".ppt [modalità compatibilità] - Microsoft PowerPoint"};
char nome_Power[52];

PowerPoint::PowerPoint()
{// Inizializzo i cinque parametri del vettore a 0
	dati_input[0] = 0.000000f,dati_input[1] = 0.000000f,dati_input[2] = 0.000000f,
	dati_input[3] = 0.000000f,dati_input[4] = 0.000000f,dati_input[5] = 0.000000f;

	dati_input2[0] = 0.000000f,dati_input2[1] = 0.000000f,dati_input2[2] = 0.000000f,
	dati_input2[3] = 0.000000f,dati_input2[4] = 0.000000f,dati_input2[5] = 0.000000f;
	destro = 0, sinistro = 0;//Vengono utilizzati per i gesti multipli
	tipo_cnt = 0,tipo_cnt2= 0;
}

void PowerPoint::DomandaTipoGuanto(void)
{
	Pulisci_Seconda_Cornice();
	Spostati_Nella_Posizione(2,12);
	// Scelta iniziale riguardo il guanto da utilizzare
	printf("Specifica il tipo di guanto su cui vui lavorare\n");
	Spostati_Nella_Posizione(2,13);
	printf("Inserisci D per guanto destro oppure\n");
	Spostati_Nella_Posizione(2,14);
	printf("Inserisci S per guanto sinistro.\n");
	Spostati_Nella_Posizione(2,15);
	printf("Inserisci E per usare Entrambi i guanti\n");
	Spostati_Nella_Posizione(2,16);
	printf("Scelta(D/S/E): ");
	// Prendi da input la scelta effettuata dall'utente
	scanf("%s",&tipo_guanto);
	Pulisci_Seconda_Cornice();
	Spostati_Nella_Posizione(16,23);
}

void PowerPoint::DomandaNomeFilePowerPoint(void)
{
	Pulisci_Seconda_Cornice();
	Spostati_Nella_Posizione(2,12);
	// L'utente deve inserire il nome del file PowerPoint
	// su cui si ha intenzione di operare
	printf("Inserisci il file su cui si vuole operare");
	Spostati_Nella_Posizione(2,13);
	printf("Nome File PowerPoint:");
	Spostati_Nella_Posizione(24,13);
	scanf("%s",&nome_Power);
	strcat(nome_Power,Nome_PP);
}
// Metodo per avviare la procedura di riconoscimento dei gesti e successivo
// invio di comando a PowerPoint
int PowerPoint::Avvia_power(Project *proj,int gloveid1,int gloveid2)
{
	Inizializzatore();

	DomandaTipoGuanto();
	DomandaNomeFilePowerPoint();
	// Conta il numero di file presenti nel file
	// Per il guanto destro
	if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"d") == 0 ))
		Conta_Elementi();
	// Per il guanto sinistro
	if ( (strcmp(tipo_guanto,"S") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ))
		Conta_Elementi();
	// Nel caso in cui si vogliano utilizzare entrambi i guanti
	if ( (strcmp(tipo_guanto,"E") == 0 ) || (strcmp(tipo_guanto,"e") == 0 ))
		Conta_Elementi();

	do
	{
		// Metto in pausa per rallentare la scansione
		Sleep(999);
		// Resetto i parametri
		Inizializzatore();
		// Questo metodo avvia la procedura per ottenere le informazioni dal guanto
		// e le salva in un apposita struttura dati (matrice) per poterle elaborare.
		ScaricaDatieSalvaSuMatrice(proj,gloveid1,gloveid2);
		// Calcola la distanza tra i centroidi
		Calcola_Distanza_Centroidi();
		// Sommo le distanze precedentemente calcolate
		somma_distanze();
	
		// Metodo per cercare un gesto
		// Se si utilizza un solo guanto alla volta, basta utilizzare la stessa
		// procedura per il singolo guanto
		if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"S") == 0 ) ||
		     (strcmp(tipo_guanto,"d") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) )
			 gesto = Trova_Gesto();
		// Il metodo è lo stesso anche se si vuole utilizzare entrambi i guanti
		// ma in questo caso esiste un meccanismo diverso.
		if ( (strcmp(tipo_guanto,"E") == 0 ) || (strcmp(tipo_guanto,"e") == 0 ))
			gesto = Trova_Gesto();

		Spostati_Nella_Posizione(2,13);
		// Invia il comando
		Cmd_PowerPoint(gesto);

		if ( gesto == 'C' )
			Riconosci_Gesti_Multipli(proj,gloveid1,gloveid2);
		else if ( gesto == '4') {
		
		}
		else if ( gesto == 'H' )
			Presentazione_A_Pieno_Schermo(proj,gloveid1,gloveid2);
	} while ( gesto != 'D' );

	// Pulisco lo schermo e ritorno alla posizione standard
	Pulisci_Seconda_Cornice();
	Spostati_Nella_Posizione(16,23);
	printf("  ");
	Spostati_Nella_Posizione(16,23);
	// Fine
	return 0;
}

////////////// IDENTIFICA GESTO /////////////////////////////////////////////////
char PowerPoint::Trova_Gesto( )
{
	float minimo = 9.999f,minimo2 = 9.999f;
	int gesto = 0;
	
	if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"S") == 0 ) ||
		 (strcmp(tipo_guanto,"d") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) )
	{
		for ( int yui = 0 ; yui < totline-2; yui++ ) {
			if ( distanze[yui] < minimo && distanze[yui] > 0 ) {
				minimo = distanze[yui];
				gesto = yui;
			}
		}
	}
	if ( (strcmp(tipo_guanto,"E") == 0 ) || (strcmp(tipo_guanto,"e") == 0 ) )
	{
		// Per il guanto destro
		for ( int yui = 0 ; yui < totline-2; yui++ ) {
			if ( distanze[yui] < minimo && distanze[yui] > 0 ) {
				minimo = distanze[yui];
				gesto = yui;
			}
		}
		// Per il guanto sinistro
		for ( int yui = 0; yui < totline2-2; yui++ ) {
			if ( distanze2[yui] < minimo2 && distanze2[yui] > 0 ) {
				minimo2 = distanze2[yui];
				gesto2  = yui;
			}
		}
	}
	// Per il guanto destro o sinistro SINGOLARMENTE 
	if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"S") == 0 ) ||
		 (strcmp(tipo_guanto,"d") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) )
	{
		Spostati_Nella_Posizione(2,14);
		printf("Valore minimo: %f,a cui e' associato l'elemento numero %d.",minimo,gesto);
		Spostati_Nella_Posizione(2,15);
		printf("----> Il data glove si trova nella posizione: ---> %c <---.",tipo_elm_gesto[gesto]);
		return tipo_elm_gesto[gesto];
	}
	// Nel caso in cui si utilizzano entrambi i guanti
	if ( (strcmp(tipo_guanto,"E") == 0 ) || (strcmp(tipo_guanto,"e") == 0 ))
	{
		Spostati_Nella_Posizione(2,14);
		printf("(destro)Il valore minimo e' %f, IL VALORE ASSOCIATO %d\n",minimo,gesto);
		Spostati_Nella_Posizione(2,15);
		printf("(sinistro)Il valore minimo e' %f, IL VALORE ASSOCIATO %d\n",minimo2,gesto);

		// Per il guanto destro
		if ( destro_non_connesso == 0 ) {
			Spostati_Nella_Posizione(2,16);
			printf("----> DATA GLOVE DESTRO:  -> %c <--               ",tipo_elm_gesto[gesto]);
		}
		else if ( destro_non_connesso == 1 ) {
			Spostati_Nella_Posizione(2,16);
			printf("Il data glove destro non invia segnale.");
		}
		// Per il guanto sinistro
		if ( sinistro_non_connesso == 0 ) {
			Spostati_Nella_Posizione(2,17);
			printf("----> DATA GLOVE SINISTRO:-> %c <--.              ",tipo_elm_gesto2[gesto2]);
		}
		else if ( sinistro_non_connesso == 1 ) {
			Spostati_Nella_Posizione(2,17);
			printf("Il data glove sinistro non invia alcun segnale.");
		}
		// Devo stabilire il risultato da restituire a seconda del guanto
		if ( (tipo_elm_gesto[gesto] == 'B'  ||  tipo_elm_gesto2[gesto2] != 'B' ) ) {
			if ( sinistro_non_connesso == 1 )
				return tipo_elm_gesto[gesto];
			else if ( sinistro_non_connesso == 0 ) {
				sinistro = 1;// Viene utilizzato per i gesti multipli
				destro = 0;
				return tipo_elm_gesto2[gesto2];
			}
		}
		else if ( (tipo_elm_gesto[gesto] != 'B'  ||  tipo_elm_gesto2[gesto2] == 'B' ) ) {
			if ( destro_non_connesso == 1 )
				return tipo_elm_gesto2[gesto2];
			else if ( destro_non_connesso == 0 ) {
				destro = 1;// Viene utilizzato per i gesti multipli
				sinistro = 0;
				return tipo_elm_gesto[gesto];
			}
		}
	}
	return 'X';
}
/// IDENTIFICA GESTO ///

// Questo metodo esegue la somma dei centroidi
void PowerPoint::somma_distanze()
{
	float somma = 0,somma2 = 0;
	
	if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"S") == 0 ) ||
		 (strcmp(tipo_guanto,"d") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) )
	{
		for (int ipu = 0; ipu < totline-2; ipu++) {
			for (int ju = 0; ju < 5; ju++) {
				somma += matrice_Centroidi[ipu][ju];
			}
			distanze[ipu] = somma;
			somma = 0;
		}
	}
	if ( (strcmp(tipo_guanto,"E") == 0 ) || (strcmp(tipo_guanto,"e") == 0 )) {
		// Per il guanto destro
		for (int ipu = 0; ipu < totline-2; ipu++) {
			for ( int ju = 0; ju < 5; ju++ ) {
				somma += matrice_Centroidi[ipu][ju];
			}
			distanze[ipu] = somma;
			somma = 0;
		}
		// Per il guanto sinistro
		for (int ipu = 0; ipu < totline2-2; ipu++) {
			for (int ju = 0; ju < 5; ju++) {
				somma2 += matrice_Centroidi2[ipu][ju];
			}
			distanze2[ipu] = somma2;
			somma2 = 0;
		}
	}
}

/// INSERISCI ELEMENTO ALL'INTERNO DELLA MATRICE ///
// Questo metodo si occupa di inserire un elemento all'interno della
// matrice.
void PowerPoint::Inserisci_Elem_Matr(float elemento,char tipguanto[2])
{
	// Il valore di tipo float viene inserito nella posizione riga/colonna
	// elemento     rappresenta l'elemento da inserire
	// righe_conta  INDICA le righe
	// ju           INDICA le colonne
	if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"S") == 0 ) ||
		 (strcmp(tipo_guanto,"d") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) )
	{
		if ( righe_conta >= 0 && righe_conta <= totline-2 && ju >= 0 && ju <= 6 )
			matrice_Centroidi[righe_conta][ju] = elemento;
		if ( ju >= 4 ) {
			righe_conta++;
			ju = 0;
		}
		else {
			ju++;
		}
	}
	if ( (strcmp(tipo_guanto,"E") == 0 ) || (strcmp(tipo_guanto,"e") == 0 ) ) {
		// Per il guanto Destro
		if ( (strcmp(tipguanto,"D") == 0 ) ) {
			if ( righe_conta >= 0 && righe_conta <= totline-2 && ju >= 0 && ju <= 6 )
				matrice_Centroidi[righe_conta][ju] = elemento;
			if ( ju >= 4 ) {
				righe_conta++;
				ju = 0;
			}
			else {
				ju++;
			}
		}//Fine Destro
		// Per il guanto Sinistro
		if ( (strcmp(tipguanto,"S") == 0 ) ) {
			if ( righe_conta2 >= 0 && righe_conta2 <= totline2-2 && ju2 >= 0 && ju2 <= 6 )
				matrice_Centroidi2[righe_conta2][ju2] = elemento;
			if ( ju2 >= 4 ) {
				righe_conta2++;
				ju2 = 0;
			}
			else {
				ju2++;
			}
		}// Fine Sinistro
	}
}
/// INSERISCI ELEMENTO ALL'INTERNO DELLA MATRICE ///

// Questo metodo si occupa di inserire la tipologia di centroide
// che si sta utilizzando, quindi ad esempio, per una determinata
// quintupla esiste il valore A,B,C,ecc.
void PowerPoint::Inserisci_Tipo_Matr(float elemento,char tipguanto[2])
{
	tipo_cnt = 0,tipo_cnt2= 0;
	if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"S") == 0 ) ||
		 (strcmp(tipo_guanto,"d") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) )
	{// Sto inserendo l'elemento all'interno della matrice
		matrice_Centroidi[tipo_cnt][5] = elemento;
		tipo_cnt++;
	}
	if ( (strcmp(tipo_guanto,"E") == 0 ) || (strcmp(tipo_guanto,"e") == 0 ) )
	{
		// Inserisco l'elemento per il guanto destro
		matrice_Centroidi[tipo_cnt][5] = elemento;
		tipo_cnt++;
		// Inserisco l'elemento per il guanto sinistro
		matrice_Centroidi2[tipo_cnt2][5] = elemento;
		tipo_cnt2++;
	}
}

// Questo metodo si occupa di calcolare la media degli elementi ottenuti
// dal data glove. La procedura serve per ottenere dati piu' uniformi.
// L'operazione viene eseguita sugli ultimi 5 elementi scaricati dal
// data glove.
void PowerPoint::Calcola_Media()
{
	somma2_1 = 0;
	// Se si utilizza solo un guanto alla volta, viene utilizzato il
	// seguente frammento di codice
	if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"S") == 0 ) ||
		 (strcmp(tipo_guanto,"d") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) )
	{
		/// Calcolo media dito Pollice ///
		for ( int gt = 0; gt <= 4; gt++ ) {
			somma_1 = somma_1 + matrice_Glove[gt][0];
		}
		media_1 = somma_1 / 5;
		/// Calcolo media dito Indice ///
		for ( int gt = 0; gt <= 4; gt++ ) {
			somma_2 = somma_2 + matrice_Glove[gt][1];
		}
		media_2 = somma_2 / 5;
		/// Calcolo media dito Medio ///
		for ( int gt = 0; gt <= 4; gt++ ) {
			somma_3 = somma_3 + matrice_Glove[gt][2];
		}
		media_3 = somma_3 / 5;
		/// Calcolo media dito Anulare ///
		for ( int gt = 0; gt <= 4; gt++ ) {
			somma_4 = somma_4 + matrice_Glove[gt][3];
		}
		media_4 = somma_4 / 5;
		/// Calcolo media dito Mignolo ///
		for ( int gt = 0; gt <= 4; gt++ ) {
			somma_5 = somma_5 + matrice_Glove[gt][4];
		}
		media_5 = somma_5 / 5;
	}

	// Nel caso in cui si utlizzano entrambi i guanti
	if ( (strcmp(tipo_guanto,"E") == 0 ) || (strcmp(tipo_guanto,"e") == 0 ) )
	{
		/// Calcolo media dito Pollice ///
		for ( int gt = 0; gt <= 4; gt++ )
			somma_1 = somma_1 + matrice_Glove[gt][0];
		media_1 = somma_1 / 5;
		/// Calcolo media dito Indice ///
		for ( int gt = 0; gt <= 4; gt++ )
			somma_2 = somma_2 + matrice_Glove[gt][1];
		media_2 = somma_2 / 5;
		/// Calcolo media dito Medio ///
		for ( int gt = 0; gt <= 4; gt++ )
			somma_3 = somma_3 + matrice_Glove[gt][2];
		media_3 = somma_3 / 5;
		/// Calcolo media dito Anulare ///
		for ( int gt = 0; gt <= 4; gt++ )
			somma_4 = somma_4 + matrice_Glove[gt][3];
		media_4 = somma_4 / 5;
		/// Calcolo media dito Mignolo ///
		for ( int gt = 0; gt <= 4; gt++ )
			somma_5 = somma_5 + matrice_Glove[gt][4];
		media_5 = somma_5 / 5;

		// Frammento di codice per il guanto sinistro

		/// Calcolo media dito Pollice ///
		for ( int gt = 0; gt <= 4; gt++)
			somma2_1 = somma2_1 + matrice_Glove_2[gt][0];
		media2_1 = somma2_1 / 5;
		/// Calcolo media dito Indice ///
		for ( int gt = 0; gt <= 4; gt++)
			somma2_2 = somma2_2 + matrice_Glove_2[gt][1];
		media2_2 = somma2_2 / 5;
		/// Calcolo media dito Medio ///
		for ( int gt = 0; gt <= 4; gt++)
			somma2_3 = somma2_3 + matrice_Glove_2[gt][2];
		media2_3 = somma2_3 / 5;
		/// Calcolo media dito Anulare ///
		for ( int gt = 0; gt <= 4; gt++)
			somma2_4 = somma2_4 + matrice_Glove_2[gt][3];
		media2_4 = somma2_4 / 5;
		/// Calcolo media dito Mignolo ///
		for ( int gt = 0; gt <= 4; gt++)
			somma2_5 = somma2_5 + matrice_Glove_2[gt][4];
		media2_5 = somma2_5 / 5;
	}
	// Metodo per la normalizzazione
	Normalizzazione();
}// Fine Calcolo Media

// I valore ottenuti devono essere normalizzati per evitare informazioni
// non perfettamente "formulate" o la presenza di errori.
void PowerPoint::Normalizzazione()
{
	valore = 0.001000f;
	// Nel caso in cui si usi singolarmente un guanto
	if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"S") == 0 ) ||
		 (strcmp(tipo_guanto,"d") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) )
	{
		norm_1  = ( media_1 / 100 ) - valore;
		norm_2  = ( media_2 / 100 ) - valore;
		norm_3  = ( media_3 / 100 ) - valore;
		norm_4  = ( media_4 / 100 ) - valore;
		norm_5  = ( media_5 / 100 ) - valore;

		if ( norm_1,norm_2,norm_3,norm_4,norm_5 > 0 )
		{
			Spostati_Nella_Posizione(2,17);
			printf("   %f   %f   %f   %f   %f",norm_1 / norma,
				                               norm_2 / norma,
											   norm_3 / norma,
											   norm_4 / norma,
											   norm_5 / norma);
			dati_input[0] = norm_1 / norma;
			dati_input[1] = norm_2 / norma;
			dati_input[2] = norm_3 / norma;
			dati_input[3] = norm_4 / norma;
			dati_input[4] = norm_5 / norma;
		}
	}
	// Nel caso in cui si utilizzano entrambi i guanti
	if ( (strcmp(tipo_guanto,"E") == 0 ) || (strcmp(tipo_guanto,"e") == 0 ) )
	{
		// Normalizzazione per il guanto destro
		norm_1  = ( media_1 / 100 ) - valore;
		norm_2  = ( media_2 / 100 ) - valore;
		norm_3  = ( media_3 / 100 ) - valore;
		norm_4  = ( media_4 / 100 ) - valore;
		norm_5  = ( media_5 / 100 ) - valore;
		// Normalizzazione per il guanto sinistro
		norm2_1 = ( media2_1 / 100 ) - 0.087872f;
		norm2_2 = ( media2_2 / 100 ) - valore;
		norm2_3 = ( media2_3 / 100 ) - valore;
		norm2_4 = ( media2_4 / 100 ) - valore;
		norm2_5 = ( media2_5 / 100 ) - valore;

		if (  norm_1, norm_2, norm_3, norm_4, norm_5 > 0 &&
			 norm2_1,norm2_2,norm2_3,norm2_4,norm2_5 > 0)
		{
			Spostati_Nella_Posizione(2,18);
			if (norm_1 > 0 && norm_2 > 0 && norm_3 > 0 && norm_4 > 0 && norm_5 > 0 ) {
				destro_non_connesso = 0;
				Spostati_Nella_Posizione(2,18);
				printf("   %f   %f   %f   %f   %f\n",norm_1/norma,
					                                 norm_2/norma,
													 norm_3/norma,
													 norm_4/norma,
													 norm_5/norma);
			}
			else {
				destro_non_connesso = 1;
				Spostati_Nella_Posizione(2,18);
				printf("Attenzione! Il guanto destro non e' connesso!                          ");
			}
			Spostati_Nella_Posizione(2,19);
			if (norm2_1 > 0 && norm2_2 > 0 && norm2_3 > 0 && norm2_4 > 0 && norm2_5 > 0) {
				sinistro_non_connesso = 0;
				Spostati_Nella_Posizione(2,19);
				printf("   %f   %f   %f   %f   %f\n",norm2_1/norma,
					                                 norm2_2/norma,
													 norm2_3/norma,
													 norm2_4/norma,
													 norm2_5/norma);
			}
			else {
				sinistro_non_connesso = 1;
				Spostati_Nella_Posizione(2,19);
				printf("Attenzione! Il guanto sinistro non e' connesso!                         ");
			}
			Spostati_Nella_Posizione(2,21);
			dati_input[0] = norm_1 / norma;
			dati_input[1] = norm_2 / norma;
			dati_input[2] = norm_3 / norma;
			dati_input[3] = norm_4 / norma;
			dati_input[4] = norm_5 / norma;
			// Operazione di divisione per l'altro guanto
			dati_input2[0] = norm2_1 / norma;
			dati_input2[1] = norm2_2 / norma;
			dati_input2[2] = norm2_3 / norma;
			dati_input2[3] = norm2_4 / norma;
			dati_input2[4] = norm2_5 / norma;
		}
	}
}// Fine Normalizzazione

// Questo metodo stabilisce la connessione per scaricare i dati dal glove.
// Le informazioni vengono salvate su un'apposita matrice.
void PowerPoint::ScaricaDatieSalvaSuMatrice(Project *proj,int gloveid1,int gloveid2)
{
	for( int qw = 0; qw != 5; qw++)
	{   // Ottengo i valori dal DataGlove con ID 1
		if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"d") == 0 ))
			dg1 = proj->GetDataGlove(gloveid1);
		// Ottengo i valori dal DataGlove con ID 2
		if ( (strcmp(tipo_guanto,"S") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ))
			dg2 = proj->GetDataGlove(gloveid2);
		if ( (strcmp(tipo_guanto,"E") == 0 ) || (strcmp(tipo_guanto,"e") == 0 )) {
			// In questo caso utilizzo entrambi i guanti
			dg1 = proj->GetDataGlove(gloveid1);
			dg2 = proj->GetDataGlove(gloveid2);
		}
		// Per il guanto Destro singolarmente
		if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"d") == 0 ))
		{// Verifico che il valore ottenuto sia diverso da 0
			if ( (dg1->f[0] != 0) &&
			     (dg1->f[1] != 0) &&
			     (dg1->f[2] != 0) &&
			     (dg1->f[3] != 0) &&
			     (dg1->f[4] != 0) )  {
				if ( contatore <= 5 )
				{// Salvo i dati in una matrice per successiva elaborazione
					for ( int gr = 0; gr <= 5; gr ++ ) {
						matrice_Glove[righe][gr] = dg1->f[gr];
					}
					contatore++;
					righe++;
					if ( righe > 5 )
						righe = 0;
				}
				else if ( contatore > 5 )
					contatore = 0;
			}
		}// Fine if per il guanto destro
		// Per il guanto Sinistro singolarmente
		if ( (strcmp(tipo_guanto,"S") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ))
		{// Verifico che il valore ottenuto sia diverso da 0
			if ( ( dg2->f[0] != 0 ) &&
			     ( dg2->f[1] != 0 ) &&
			     ( dg2->f[2] != 0 ) &&
			     ( dg2->f[3] != 0 ) &&
			     ( dg2->f[4] != 0 ) )
			{
				if ( contatore <= 5 )
				{// Salvo i dati in una matrice per successiva elaborazione
					matrice_Glove[righe][0] = dg2->f[1];
					matrice_Glove[righe][1] = dg2->f[0];
					matrice_Glove[righe][2] = dg2->f[2];
					matrice_Glove[righe][3] = dg2->f[3];
					matrice_Glove[righe][4] = dg2->f[4];
					// Contatori per gestire l'inserimento
					contatore++;
					righe++;
					if ( righe > 5 )
						righe = 0;
				}
				else if ( contatore > 5 )
					contatore = 0;
			}
		}// Fine If per il guanto sinistro singolarmente

		// Nel caso in cui si utilizzano entrambi i guanti
		if ( (strcmp(tipo_guanto,"E") == 0 ) || (strcmp(tipo_guanto,"e") == 0 ) )
		{
			if ( ( dg1->f[0] != 0 ) &&
			     ( dg1->f[1] != 0 ) &&
			     ( dg1->f[2] != 0 ) &&
			     ( dg1->f[3] != 0 ) &&
			     ( dg1->f[4] != 0 ) ) {
				if ( contatore <= 5 )
				{// Salvo i dati in una matrice per successiva elaborazione
					for ( int gr = 0; gr <= 5; gr ++ ) {
						matrice_Glove[righe][gr] = dg1->f[gr];
					}
					contatore++;
					righe++;
					if ( righe > 5 )
						righe = 0;
				}
				else if ( contatore > 5 )
					contatore = 0;
			}
			// Prelevo i dati per il guanto sinistro
			// Le informazioni ottenute vengono salvate
			// all'interno di un altra matrice
			if ( ( dg2->f[0] != 0 ) &&
			     ( dg2->f[1] != 0 ) &&
			     ( dg2->f[2] != 0 ) &&
			     ( dg2->f[3] != 0 ) &&
			     ( dg2->f[4] != 0 ) ) {
				if ( contatore2 <= 5 )
				{// Salvo i dati in una matrice per successiva elaborazione
					matrice_Glove_2[righe2][0] = dg2->f[1];
					matrice_Glove_2[righe2][1] = dg2->f[0];
					matrice_Glove_2[righe2][2] = dg2->f[2];
					matrice_Glove_2[righe2][3] = dg2->f[3];
					matrice_Glove_2[righe2][4] = dg2->f[4];

					contatore2++;
					righe2++;
					if ( righe2 > 5 )
						righe2 = 0;
				}
				else if ( contatore2 > 5 )
					contatore2 = 0;
			}
		}// Fine caso in cui si utilizzano entrambi i guanti
	}
	// Calcolo la media
	Calcola_Media();
}

// Inizializzazione matrice
void PowerPoint::Inizializzazione_Matrice(void)
{// Per evitare dati "corrotti" conviene inizializzare a 0 tutta la matrice.
 // In questo modo elimino eventuali valori di un precedente utilizzo
	for ( int neo = 0; neo < totline-2; neo++ ) {
		for ( int ju = 0; ju < COL; ju++ ) {
			matrice_Centroidi[neo][ju] = 0;
		}
	}
	for ( int neo = 0; neo < totline2-2; neo++ ) {
		for ( int ju = 0; ju < COL; ju++ ) {
			matrice_Centroidi2[neo][ju] = 0;
		}
	}
}

// METODO PER IL CALCOLO DELLA DISTANZA EUCLIDEA
// In questo metodo non faccio altro che calcolare la distanza euclidea
// tra il valore che ho ottenuto con la scansione dal data glove ed
// il file che contiene i centroidi.
// Il file è stato precedentemente generato ed esso contiene tutte le info
// per identificare il gesto.
void PowerPoint::Calcola_Distanza_Centroidi(void)
{
	// Descrittore per il file
	FILE *FileGlove, *FileGlove2;
	// Inizializzo a zero il contatore
	elo = 0, elo2= 0;
	// Inizializzazione matrice a 0
	Inizializzazione_Matrice();

	// File da aprire per il training ( guanto destro )
	if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"d") == 0 ) )
	{
		// Apro il file in modalita' sola lettura
		if ( ( FileGlove = fopen("glove.codf","r") ) == NULL ) {
			// Se si verifica un problema eseguo avviso errore
			printf("Attenzione! Il file di training non e' presente.\n");
			system("pause");
			exit(1);
		}
	}
	// File da aprire per il training ( guanto sinistro )
	if ( (strcmp(tipo_guanto,"S") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) )
	{
		// Apro il file in modalita' sola lettura
		if ( ( FileGlove = fopen("glove_s.codf","r") ) == NULL ) {
			// Se si verifica un problema eseguo avviso errore
			printf("Attenzione! Il file di training non e' presente.\n");
			system("pause");
			exit(1);
		}
	}
	// Se utilizzo entrambi i guanti devo aprire due files
	if ( (strcmp(tipo_guanto,"E") == 0 ) || (strcmp(tipo_guanto,"e") == 0 )) {
		if ( ( FileGlove = fopen("glove.codf","r") ) == NULL ) {
			printf("Attenzione! Il file di training non e' presente.\n");
			system("pause");
			exit(1);
		}
		if ( ( FileGlove2 = fopen("glove_s.codf","r") ) == NULL ) {
			printf("Attenzione! Il file di training non e' presente.\n");
			system("pause");
			exit(1);
		}
	}
	// Eseguo l'elevazione al quadrato del valore
	if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"S") == 0 ) ||
		 (strcmp(tipo_guanto,"d") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) ) {
		for ( int ce = 0; ce < 5; ce++ ) {
			vettore_pow[ce] = pow(dati_input[ce],2);
		}
	}
	if ( (strcmp(tipo_guanto,"E") == 0 ) || (strcmp(tipo_guanto,"e") == 0 ) ) {
		// Per il guanto destro
		for ( int ce = 0; ce < 5; ce++ ) {
			vettore_pow[ce] = pow(dati_input[ce],2);
		}
		// Per il guanto sinistro
		for ( int ce = 0; ce < 5; ce++ ) {
			vettore_pow2[ce] = pow(dati_input2[ce],2);
		}
	}
	// Eseguo un ciclo while Fino alla fine del file
	while( !feof(FileGlove) ) {
		// Prelevo i dati in ingresso mediate fgets e salvo le info in un 
		// buffer dati per successiva elaborazione
		if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"S") == 0 ) ||
			 (strcmp(tipo_guanto,"d") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) )
			 fgets(buf_dati,256,FileGlove);
		if ( (strcmp(tipo_guanto,"E") == 0 ) || (strcmp(tipo_guanto,"e") == 0 )) {
			fgets(buf_dati ,256,FileGlove);
			fgets(buf_dati2,256,FileGlove2);
		}
		// Primo valore numerico Float
		for ( int qa = 0; qa <= 7; qa++ ) {
			if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"S") == 0 ) ||
				 (strcmp(tipo_guanto,"d") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) )
				numf[qa] = buf_dati[qa];
			if ( (strcmp(tipo_guanto,"E") == 0 ) || (strcmp(tipo_guanto,"e") == 0 )) {
				numf[qa]  = buf_dati[qa];
				numf2[qa] = buf_dati2[qa];
			}
		}// Fine for
		if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"S") == 0 ) ||
			 (strcmp(tipo_guanto,"d") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) ) {
			numf[8] = '\0';
			val1 = (float)atof(numf);
			vettore_corr[0] = val1;
			memset(numf, 0, 8);
		}
		if ( (strcmp(tipo_guanto,"E") == 0 ) || (strcmp(tipo_guanto,"e") == 0 ) ) {
			// Per il guanto destro
			numf[8] = '\0';
			val1 = (float)atof(numf);
			vettore_corr[0] = val1;
			memset(numf, 0, 8);
			// Per il guanto sinistro
			numf2[8] = '\0';
			val2_1 = (float)atof(numf2);
			vettore_corr2[0] = val2_1;
			memset(numf2, 0, 8);
		}// Fine Primo valore numerico Float

		// Secondo valore numerico float
		int bv = 0;
		int bv2= 0;
		for ( int qn = 9; qn <= 16; qn++ ) {
			if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"S") == 0 ) ||
				 (strcmp(tipo_guanto,"d") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) ) {
				numf[bv] = buf_dati[qn];
				bv++;
			}
			if ( (strcmp(tipo_guanto,"E") == 0 ) || (strcmp(tipo_guanto,"e") == 0 )) {
				// Per il guanto destro
				numf[bv] = buf_dati[qn];
				bv++;
				// Per il guanto sinistro
				numf2[bv2] = buf_dati2[qn];
				bv2++;
			}
		} // Fine for

		if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"S") == 0 ) ||
			 (strcmp(tipo_guanto,"d") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) ) {
			numf[8] = '\0';
			val2 = (float)atof(numf);
			vettore_corr[1] = val2;
			memset(numf, 0, 8);
		}
		if ( (strcmp(tipo_guanto,"E") == 0 ) || (strcmp(tipo_guanto,"e") == 0 )) {
			// Per il guanto destro
			numf[8] = '\0';
			val2 = (float)atof(numf);
			vettore_corr[1] = val2;
			memset(numf, 0, 8);
			// Per il guanto sinistro
			numf2[8] = '\0';
			val2_2 = (float)atof(numf2);
			vettore_corr2[1] = val2_2;
			memset(numf2, 0, 8);
		}// Fine Secondo valore numerico float

		// Terzo valore numerico float
		int bt = 0;
		int bt2= 0;
		for ( int qn = 18; qn <= 25; qn++ ) {
			if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"S") == 0 ) ||
				 (strcmp(tipo_guanto,"d") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) ) {
				numf[bt] = buf_dati[qn];
				bt++;
			}
			if ( (strcmp(tipo_guanto,"E") == 0 ) || (strcmp(tipo_guanto,"e") == 0 ) ) {
				// Per il guanto destro
				numf[bt] = buf_dati[qn];
				bt++;
				// Per il guanto sinistro
				numf2[bt2] = buf_dati2[qn];
				bt2++;
			}
		}// Fine for
		if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"S") == 0 ) ||
			 (strcmp(tipo_guanto,"d") == 0 ) || (strcmp(tipo_guanto,"s") == 0 )) {
			numf[8] = '\0';
			val3 = (float)atof(numf);
			vettore_corr[2] = val3;
			memset(numf, 0, 8);
		}
		if ( (strcmp(tipo_guanto,"E") == 0 ) || (strcmp(tipo_guanto,"e") == 0 )) {
			// Per il guanto destro
			numf[8] = '\0';
			val3 = (float)atof(numf);
			vettore_corr[2] = val3;
			memset(numf, 0, 8);
			// Per il guanto sinistro
			numf2[8] = '\0';
			val2_3 = (float)atof(numf2);
			vettore_corr2[2] = val2_3;
			memset(numf2, 0, 8);
		}// Fine Terzo valore numerico float

		// Quarto valore numerico float
		int wr = 0;
		int wr2= 0;
		for ( int qp = 27; qp <= 34; qp++ ) {
			if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"S") == 0 ) ||
				 (strcmp(tipo_guanto,"d") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) ) {
				numf[wr] = buf_dati[qp];
				wr++;
			}
			if ( (strcmp(tipo_guanto,"E") == 0 ) || (strcmp(tipo_guanto,"e") == 0 )) {
				// per il guanto Destro
				numf[wr] = buf_dati[qp];
				wr++;
				// Per il guanto Sinistro
				numf2[wr2] = buf_dati2[qp];
				wr2++;
			}
		}// Fine for
		
		if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"S") == 0 ) ||
			 (strcmp(tipo_guanto,"d") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) ) {
			numf[8] = '\0';
			val4 = (float)atof(numf);
			vettore_corr[3] = val4;
			memset(numf, 0, 8);
		}
		if ( (strcmp(tipo_guanto,"E") == 0 ) || (strcmp(tipo_guanto,"e") == 0 )) {
			// Per il guanto destro
			numf[8] = '\0';
			val4 = (float)atof(numf);
			vettore_corr[3] = val4;
			memset(numf, 0, 8);
			// Per il guanto sinistro
			numf2[8] = '\0';
			val2_4 = (float)atof(numf2);
			vettore_corr2[3] = val2_4;
			memset(numf2, 0, 8);
		}// Fine quarto valore numerico float

		// Quinto valore numerico float
		int as = 0;
		int as2= 0;
		for ( int qa = 36; qa <= 43; qa++ ) {
			if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"S") == 0 ) ||
				 (strcmp(tipo_guanto,"d") == 0 ) || (strcmp(tipo_guanto,"s") == 0 )) {
				numf[as] = buf_dati[qa];
				as++;
			}
			if ( (strcmp(tipo_guanto,"E") == 0 ) || (strcmp(tipo_guanto,"e") == 0 )) {
				// Per il guanto destro
				numf[as] = buf_dati[qa];
				as++;
				// Per il guanto sinistro
				numf2[as2] = buf_dati2[qa];
				as2++;
			}
		}// Fine for

		if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"S") == 0 ) ||
			 (strcmp(tipo_guanto,"d") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) ) {
			numf[8] = '\0';
			val5 = (float)atof(numf);
			vettore_corr[4] = val5;
			memset(numf, 0, 8);
		}
		if ( (strcmp(tipo_guanto,"E") == 0 ) || (strcmp(tipo_guanto,"e") == 0 )) {
			// Per il guanto destro
			numf[8] = '\0';
			val5 = (float)atof(numf);
			vettore_corr[4] = val5;
			memset(numf, 0, 8);
			// Per il guanto sinistro
			numf2[8] = '\0';
			val2_5 = (float)atof(numf2);
			vettore_corr2[4] = val2_5;
			memset(numf2, 0, 8);
		}

		// Per evitare falsi dati , nel caso in cui non vengano correttamente
		// ricevute le informazioni oppure il glove e' stato spento ecc,
		// Mi assicuro che i dati letti sia maggiori di 0
		// Dato che i valori devono obbligatoriamente essere presenti
		// Se si ottengono altri valori ci deve essere un problema
		if ( vettore_corr[0] > 0 &&
			 vettore_corr[1] > 0 &&
			 vettore_corr[2] > 0 &&
			 vettore_corr[3] > 0 &&
			 vettore_corr[4] > 0 )
		{
			// Elevazione al quadrato mediante pow
			// della riga corrrente
			if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"S") == 0 ) ||
				 (strcmp(tipo_guanto,"d") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) ) {
				for ( int ce = 0; ce < 5; ce++ ) {
					vettore_corr_pow[ce] = pow(vettore_corr[ce],2);
				}
			}
			if ( (strcmp(tipo_guanto,"E") == 0 ) || (strcmp(tipo_guanto,"e") == 0 )) {
				// Per il guanto destro
				for ( int ce = 0; ce < 5; ce++ ) {
					vettore_corr_pow[ce] = pow(vettore_corr[ce],2);
				}
				// Per il guanto sinistro
				for ( int ce = 0; ce < 5; ce++ ) {
					vettore_corr_pow2[ce] = pow(vettore_corr2[ce],2);
				}
			}
			// Eseguo la differenza tra il vettore corrente in analisi
			// ed il glove 
			if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"S") == 0 ) ||
				 (strcmp(tipo_guanto,"d") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) ) {
				for ( int oz = 0; oz < 5; oz++ ) {
					val_min_global[oz] = abs(vettore_corr_pow[oz] - vettore_pow[oz]);
				}
			}
			if ( (strcmp(tipo_guanto,"E") == 0 ) || (strcmp(tipo_guanto,"e") == 0 )) {
				// Per il guanto destro
				for ( int oz = 0; oz < 5; oz++ ) {
					val_min_global[oz] = abs(vettore_corr_pow[oz] - vettore_pow[oz]);
				}
				// Per il guanto sinistro
				for ( int oz = 0; oz < 5; oz++ ) {
					val_min_global2[oz] = abs(vettore_corr_pow2[oz] - vettore_pow2[oz]);
				}
			}
			// Ottengo tutte le differenze
			if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"S") == 0 ) ||
				 (strcmp(tipo_guanto,"d") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) ) {
				for ( int hg = 0; hg < 5; hg++ ) {
					Inserisci_Elem_Matr(val_min_global[hg],tipo_guanto);
				}
				tipo_elm_gesto[elo] = buf_dati[45];
				elo++;
				Inserisci_Tipo_Matr(buf_dati[45],tipo_guanto);
			}
			if ( (strcmp(tipo_guanto,"E") == 0 ) || (strcmp(tipo_guanto,"e") == 0 )) {
				// Per il guanto destro
				for ( int hg = 0; hg < 5; hg++ ) {
					Inserisci_Elem_Matr(val_min_global[hg],"D");
				}
				tipo_elm_gesto[elo] = buf_dati[45];
				elo++;
				Inserisci_Tipo_Matr(buf_dati[45],"D");
				// Per il guanto sinistro
				for ( int hg = 0; hg < 5; hg++ ) {
					Inserisci_Elem_Matr(val_min_global2[hg],"S");
				}
				tipo_elm_gesto2[elo2] = buf_dati2[45];
				elo2++;
				Inserisci_Tipo_Matr(buf_dati2[45],"S");
			}
		}
	}
	fclose(FileGlove);
}

void PowerPoint::Stampa_Matrice()
{// Stampa la matrice
	for ( int bvc = 0; bvc < totline-2; bvc++ ) {
		for ( int ju = 0; ju < COL; ju++ )
			printf(" %f ",matrice_Centroidi[bvc][ju]);
		printf("\n");
	}
}

void PowerPoint::Conta_Elementi()
{
	// Variabili per la gestione del file
	FILE *FileDest, *FileSinistra;
	char guanto_destra[14]   = "glove.codf";
	char guanto_sinistra[14] = "glove_s.codf";
	// Dichiarazione stringhe su cui lavorare
	char stringa[256];
	// Dichiarazione delle altre variabili
	// apertura file e controllo d'ingresso
	if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"d") == 0 ) ) {
		FileDest     = fopen(guanto_destra,"r");
		// Controllo dell'avvenuta apertura del file
		if ( FileDest == NULL )
			cout << "\n Errore in apertura file!" << endl;
	}
	if ( (strcmp(tipo_guanto,"S") == 0 ) || (strcmp(tipo_guanto,"s") == 0 ) ) {
		FileSinistra = fopen(guanto_sinistra,"r");
		if ( FileSinistra == NULL )
			cout << "\n Errore in apertura file!" << endl;
	}
	if ( (strcmp(tipo_guanto,"E") == 0 ) || (strcmp(tipo_guanto,"e") == 0 ) ) {
		FileDest     = fopen(guanto_destra,"r");
		FileSinistra = fopen(guanto_sinistra,"r");
		if( FileDest == NULL )
			cout << "\n Errore in apertura file!" << endl;
		if( FileSinistra == NULL )
			cout << "\n Errore in apertura file!" << endl;
	}
	// Inizializzazione a 0
	totline  = 0,totline2 = 0;
	// Ciclo while fino alla fine del file
	// Esistono due versioni perchè nel caso in cui si utilizzano
	// entrambi i guanti dobbiamo caricare il numero di centroidi
	// per ogni file
	if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"d") == 0 )) {
		while( !feof(FileDest) ) {
			// Controllo di una stringa del file di ingresso
			fgets(stringa,256,FileDest);
			// Pulisci il buffer
			memset(stringa, 0, 256);
			totline++;
		}
		fclose(FileDest);
	}
	if ( (strcmp(tipo_guanto,"S") == 0 ) || (strcmp(tipo_guanto,"s") == 0 )) {
		while( !feof(FileSinistra) ) {
			// Controllo di una stringa del file di ingresso
			fgets(stringa,256,FileSinistra);
			// Pulisci il buffer
			memset(stringa, 0, 256);
			totline++;
		}
		fclose(FileSinistra);
	}
	
	if ( (strcmp(tipo_guanto,"E") == 0 ) || (strcmp(tipo_guanto,"e") == 0 )) {
		while( !feof(FileDest) ) {
			// Controllo di una stringa del file di ingresso
			fgets(stringa,256,FileDest);
			// Pulisci il buffer
			memset(stringa, 0, 256);
			totline++;
		}
		fclose(FileDest);
		while( !feof(FileSinistra) ) {
			// Controllo di una stringa del file di ingresso
			fgets(stringa,256,FileSinistra);
			// Pulisci il buffer
			memset(stringa, 0, 256);
			totline2++;
		}
		fclose(FileSinistra);
	}
	Pulisci_Seconda_Cornice();
	Spostati_Nella_Posizione(2,12);
	printf("Il numero di elementi presenti nel file training: %d.",totline-2);
}

// Questo metodo invia un comando unico.
// Si tratta di gesti di default ai quali sono state associate specifiche operazioni
// Il windowHandle specifica a quale applicazione dobbiamo inviare il comando
void PowerPoint::Cmd_PowerPoint(char t_gesto)
{
	//HWND windowHandle = FindWindow(0,"ASD-lez7.ppt [modalità compatibilità] - Microsoft PowerPoint");
	HWND windowHandle = FindWindow(0,(LPCWSTR)nome_Power);
	printf("Il nome %s\n",nome_Power);

	if      ( t_gesto == 'A' )
		Invia_PageUp(windowHandle);
	else if ( t_gesto == 'F' )
		Invia_PageDown(windowHandle);
	else if ( t_gesto == 'L' )
		Invia_MenuOpzioni(windowHandle);
	else if ( t_gesto == 'U' )
		Invia_ChiudiPowerPoint(windowHandle);
	else if ( t_gesto == 'E' )
		Invia_SalvaSuFileCorrente(windowHandle);
	else if ( t_gesto == 'Y' )
		Invia_GUIDA(windowHandle);
	else if ( t_gesto == 'W' )
		Invia_TAB(windowHandle);
	else if ( t_gesto == 'G' )
		Invia_ESC(windowHandle);
}

// RICONOSCIMENTO GESTI MULTIPLI
// Questo metodo si occupa di inviare un comando all'applicazione nel caso
// in cui si vogliano riconoscere gesti multipli.
void PowerPoint::Cmd_PowerPoint_Multiplo( )
{// massimo_cnt,lettera
	HWND windowHandle = FindWindow(0,(LPCWSTR)"ASD-lez7.ppt [modalità compatibilità] - Microsoft PowerPoint");

	if ( lettera == 'E' ) {
		if      ( massimo_cnt == 2 )
			Invia_Lettera_A_Grande(windowHandle);
		else if ( massimo_cnt == 3 )
			Invia_Lettera_A_Piccolo(windowHandle);
		else if ( massimo_cnt == 4 )
			Invia_Lettera_B_Grande(windowHandle);
		else if ( massimo_cnt == 5 )
			Invia_Lettera_B_Piccolo(windowHandle);
		else if ( massimo_cnt == 6 )
			e_accentata(windowHandle);
		else if ( massimo_cnt == 7 )
			u_accentata(windowHandle);
		else if ( massimo_cnt == 8 )
			o_accentata(windowHandle);
	} // Operazione da eseguire nel caso di gesto multiplo di tipo F
	else if ( lettera == 'F' ) {
		if      ( massimo_cnt == 2 )
			Invia_Lettera_C_Grande(windowHandle);
		else if ( massimo_cnt == 3 )
			Invia_Lettera_C_Piccolo(windowHandle);
		else if ( massimo_cnt == 4 )
			Invia_Lettera_D_Grande(windowHandle);
		else if ( massimo_cnt == 5 )
			Invia_Lettera_D_Piccolo(windowHandle);
		else if ( massimo_cnt == 6 )
			Invia_Lettera_E_Grande(windowHandle);
		else if ( massimo_cnt == 7 )
			Invia_Lettera_E_Piccolo(windowHandle);
	}
	else if ( lettera == 'H' ) {
		if      ( massimo_cnt == 2 )
			Invia_Lettera_F_Grande(windowHandle);
		else if ( massimo_cnt == 3 )
			Invia_Lettera_F_Piccolo(windowHandle);
		else if ( massimo_cnt == 4 )
			Invia_Lettera_G_Grande(windowHandle);
		else if ( massimo_cnt == 5 )
			Invia_Lettera_G_Piccolo(windowHandle);
		else if ( massimo_cnt == 6 )
			Invia_Lettera_H_Grande(windowHandle);
		else if ( massimo_cnt == 7 )
			Invia_Lettera_H_Piccolo(windowHandle);
	}
	else if ( lettera == 'U' ) {
		if      ( massimo_cnt == 2 )
			Invia_Lettera_I_Grande(windowHandle);
		else if ( massimo_cnt == 3 )
			Invia_Lettera_I_Piccolo(windowHandle);
		else if ( massimo_cnt == 4 )
			Invia_Lettera_L_Grande(windowHandle);
		else if ( massimo_cnt == 5 )
			Invia_Lettera_L_Piccolo(windowHandle);
		else if ( massimo_cnt == 6 )
			Invia_Lettera_M_Grande(windowHandle);
		else if ( massimo_cnt == 7 )
			Invia_Lettera_M_Piccolo(windowHandle);
	}
	else if ( lettera == 'W' ) {
		if      ( massimo_cnt == 2 )
			Invia_Lettera_N_Grande(windowHandle);
		else if ( massimo_cnt == 3 )
			Invia_Lettera_N_Piccolo(windowHandle);
		else if ( massimo_cnt == 4 )
			Invia_Lettera_O_Grande(windowHandle);
		else if ( massimo_cnt == 5 )
			Invia_Lettera_O_Piccolo(windowHandle);
		else if ( massimo_cnt == 6 )
			Invia_Lettera_P_Grande(windowHandle);
		else if ( massimo_cnt == 7 )
			Invia_Lettera_P_Piccolo(windowHandle);
	}
	else if ( lettera == 'Y' ) {
		if      ( massimo_cnt == 2 )
			Invia_Lettera_Q_Grande(windowHandle);
		else if ( massimo_cnt == 3 )
			Invia_Lettera_Q_Piccolo(windowHandle);
		else if ( massimo_cnt == 4 )
			Invia_Lettera_R_Grande(windowHandle);
		else if ( massimo_cnt == 5 )
			Invia_Lettera_R_Piccolo(windowHandle);
		else if ( massimo_cnt == 6 )
			Invia_Lettera_S_Grande(windowHandle);
		else if ( massimo_cnt == 7 )
			Invia_Lettera_S_Piccolo(windowHandle);
	}
	else if ( lettera == 'A' ) {
		if      ( massimo_cnt == 2 )
			Invia_Lettera_T_Grande(windowHandle);
		else if ( massimo_cnt == 3 )
			Invia_Lettera_T_Piccolo(windowHandle);
		else if ( massimo_cnt == 4 )
			Invia_Lettera_U_Grande(windowHandle);
		else if ( massimo_cnt == 5 )
			Invia_Lettera_U_Piccolo(windowHandle);
		else if ( massimo_cnt == 6 )
			Invia_Lettera_V_Grande(windowHandle);
		else if ( massimo_cnt == 7 )
			Invia_Lettera_V_Piccolo(windowHandle);
		else if ( massimo_cnt == 8 )
			Invia_Lettera_Z_Grande(windowHandle);
		else if ( massimo_cnt == 9 )
			Invia_Lettera_Z_Piccolo(windowHandle);
	}
	else if ( lettera == '4' ) {
		if      ( massimo_cnt == 2 )
			Invia_Spazio(windowHandle);
		else if ( massimo_cnt == 3 )
			Invia_Return(windowHandle);
		else if ( massimo_cnt == 4 )
			Invia_Cancella(windowHandle);
		else if ( massimo_cnt == 5 )
			Invia_Virgola(windowHandle);
		else if ( massimo_cnt == 6 )
			Invia_Punto(windowHandle);
	}
}

// Riconoscimento gesti multipli del guanto sinistro
int PowerPoint::Cmd_PowerPoint_Multiplo_Sinistro()
{// massimo_cnt,lettera
	HWND windowHandle = FindWindow(0,(LPCWSTR)"ASD-lez7.ppt [modalità compatibilità] - Microsoft PowerPoint");

	if ( lettera == 'E' )
	{
		if      ( massimo_cnt == 2 )
			Invia_Grassetto(windowHandle);
		else if ( massimo_cnt == 3 )
			Invia_Corsivo(windowHandle);
		else if ( massimo_cnt == 4 )
			Invia_SottoLineato(windowHandle);
		else if ( massimo_cnt == 5 )
			Invia_ControlloOrtografico(windowHandle);
		else if ( massimo_cnt == 6 )
			Chiudi_visualizzazione_Schema_Stampanti(windowHandle);
		else if ( massimo_cnt == 7 )
			Imposta_pagina_Schema_stampanti(windowHandle);
		else if ( massimo_cnt == 8 )
			Orientamento_Schema_stampanti(windowHandle);
		else if ( massimo_cnt == 9 )
			Orientamento_Diapositiva_Schema_stampanti(windowHandle);
		else if ( massimo_cnt == 10)
			Diapositive_per_pagina_Schema_stampanti(windowHandle);
		else if ( massimo_cnt == 11)
			Temi_schema_stampanti(windowHandle);
		else if ( massimo_cnt == 12)
			Stili_Sfondo_Schema_stampanti(windowHandle);
		else if ( massimo_cnt == 13)
			Data_Schema_Stampanti(windowHandle);
	} // Operazione da eseguire nel caso di gesto multiplo di tipo F
	else if ( lettera == 'F' )
	{
		if      ( massimo_cnt == 2 )
			Invia_Ricerche(windowHandle);
		else if ( massimo_cnt == 3 )
			Invia_ZOOM(windowHandle);
		else if ( massimo_cnt == 4 )
			Visualizzazione_Normale(windowHandle);
		else if ( massimo_cnt == 5 )
			Sequenza_diapositive(windowHandle);
		else if ( massimo_cnt == 6 )
			Pagina_note(windowHandle);
		else if ( massimo_cnt == 7 )
			Visualizzazione_di_lettura(windowHandle);
		else if ( massimo_cnt == 8 )
			Intestazionne_Schema_Stampanti(windowHandle);
		else if ( massimo_cnt == 9 )
			Pie_di_pagina_schema_Stampanti(windowHandle);
		else if ( massimo_cnt == 10 )
			Numero_di_pagina_scherma_stampanti(windowHandle);
		else if ( massimo_cnt == 11 )
			Colori_Schema_Stampanti(windowHandle);
		else if ( massimo_cnt == 12 )
			Effetti_Scherma_Stampanti(windowHandle);
		else if ( massimo_cnt == 13 ) {
			Tipi_Di_carattere_stampanti(windowHandle);
			Tipi_Di_carattere_stampanti(windowHandle);
		}
	}
	else if ( lettera == 'H' )
	{
		if      ( massimo_cnt == 2 )
			Scherma_diapositiva(windowHandle);
		else if ( massimo_cnt == 3 )
			Scherma_stampati(windowHandle);
		else if (massimo_cnt == 4)
			Note(windowHandle);
		else if (massimo_cnt == 5)
			Righello(windowHandle);
		else if (massimo_cnt == 6)
			Griglia(windowHandle);
	}
	else if ( lettera == 'U' )
	{
		if      ( massimo_cnt == 2 )
			Guide(windowHandle);
		else if ( massimo_cnt == 3 )
			TipoTransizioniSlides(windowHandle);
		else if ( massimo_cnt == 4 )
			Stili_Sfondo(windowHandle);
		else if ( massimo_cnt == 5 ) {
			TipiDiCarattere(windowHandle);
			TipiDiCarattere(windowHandle);
		}
		else if ( massimo_cnt == 6 )
			ScegliColori(windowHandle);
		else if ( massimo_cnt == 7 )
			ScegliEffetti(windowHandle);
	}
	else if ( lettera == 'W' )
	{
		if      ( massimo_cnt == 2 )
			Scelta_Tema(windowHandle);
		else if ( massimo_cnt == 3 )
			Orientamento_Diapositiva(windowHandle);
		else if ( massimo_cnt == 4 )
			Imposta_Pagina(windowHandle);
		else if ( massimo_cnt == 5 )
			Nuova_Diapositiva(windowHandle);
		else if ( massimo_cnt == 6 )
			Elenchi_Puntati(windowHandle);
		else if ( massimo_cnt == 7 )
			Elenchi_Numerati(windowHandle);
	}
	else if ( lettera == 'Y' )
	{
		if      ( massimo_cnt == 2 )
			FrecciaSu(windowHandle);
		else if ( massimo_cnt == 3 )
			FrecciaGiu(windowHandle);
		else if ( massimo_cnt == 4 )
			FrecciaDestra(windowHandle);
		else if ( massimo_cnt == 5 )
			FrecciaSinistra(windowHandle);
		else if ( massimo_cnt == 6 ) {
			CambiaTipiDiCarattere(windowHandle);
			CambiaTipiDiCarattere(windowHandle);
		}
		else if ( massimo_cnt == 7 )
			Cambia_Dimensione_Carattere(windowHandle);
	}
	else if ( lettera == 'A' )
	{
		if      ( massimo_cnt == 2 )
			Incrementa_dimcaratt(windowHandle);
		else if ( massimo_cnt == 3 )
			Decrementa_dimcaratt(windowHandle);
		else if ( massimo_cnt == 4 )
			Inserisci_Immagine(windowHandle);
		else if ( massimo_cnt == 5 )
			Crea_Tabella(windowHandle);
		else if ( massimo_cnt == 6 ) {
			Inserisci_Grafico(windowHandle);
			Inserisci_Grafico(windowHandle);
		}
		else if ( massimo_cnt == 7 )
			Intestazione_pie_Pagina(windowHandle);
		else if ( massimo_cnt == 8 )
			Inserisci_Forme(windowHandle);
		else if ( massimo_cnt == 9 )
			Invia_F9(windowHandle);
		else if ( massimo_cnt == 10)
			Invia_F10(windowHandle);
		else if ( massimo_cnt == 11)
			Invia_F11(windowHandle);
		else if ( massimo_cnt == 12)
			Invia_F12(windowHandle);
	}
	else if ( lettera == '4' )
	{
		if      ( massimo_cnt == 2 )
			Invia_TAB(windowHandle);
		else if ( massimo_cnt == 3 )
			PassaAdAltraFinestra(windowHandle);
		else if ( massimo_cnt == 4 )
			Invia_Return(windowHandle);
		else if ( massimo_cnt == 5 )
			Sostituisci(windowHandle);
		else if ( massimo_cnt == 6 )
			Trova_Nel_Testo(windowHandle);
		else if ( massimo_cnt == 7 )
			Invia_ESC(windowHandle);
		else if ( massimo_cnt == 8 )
			Invia_F1(windowHandle);
		else if ( massimo_cnt == 9 )
			Invia_F2(windowHandle);
		else if ( massimo_cnt == 10)
			Invia_F3(windowHandle);
		else if ( massimo_cnt == 11)
			Invia_F4(windowHandle);
		else if ( massimo_cnt == 12)
			Invia_F5(windowHandle);
		else if ( massimo_cnt == 13)
			Invia_F6(windowHandle);
		else if ( massimo_cnt == 14)
			Invia_F7(windowHandle);
		else if ( massimo_cnt == 15)
			Invia_F8(windowHandle);
	}
	else if ( lettera == 'L' )
	{
		if      ( massimo_cnt == 2 )
			Inserisci_Schema_diapositiva(windowHandle);
		else if ( massimo_cnt == 3 )
			Chiudi_Schema_diapositiva(windowHandle);
		else if ( massimo_cnt == 4 )
			Inserisci_Layout(windowHandle);
		else if ( massimo_cnt == 5 )
			Elimina_Diapositiva(windowHandle);
		else if ( massimo_cnt == 6 )
			Rinomina_Diapositiva(windowHandle);
		else if ( massimo_cnt == 7 )
			Inserisci_Segnaposto(windowHandle);
		else if ( massimo_cnt == 8 )
			Modifica_tema(windowHandle);
		else if ( massimo_cnt == 9 )
			Stili_sfondo(windowHandle);
		else if ( massimo_cnt == 10 )
			Imposta_Pagina(windowHandle);
		else if ( massimo_cnt == 11 )
			Orientamento_Diapositiva(windowHandle);
		else if ( massimo_cnt == 12 )
			Titolo_Diapositiva(windowHandle);
		else if ( massimo_cnt == 13 )
			Pie_di_pagina(windowHandle);
		else if ( massimo_cnt == 14 )
			Nascondi_grafico_di_sfondo(windowHandle);
		else if ( massimo_cnt == 15 )
			Cambia_Colore(windowHandle);
		else if ( massimo_cnt == 16 )
			Tipi_Di_carattere_diapositiva(windowHandle);
	}
	else if ( lettera == 'G' )
	{//  Metodi per la scherma note
		if      ( massimo_cnt == 2 )
			Intestazione_Note(windowHandle);
		if      ( massimo_cnt == 3 )
			Immagine_Diapositiva(windowHandle);
		if      ( massimo_cnt == 4 )
			Pie_di_pagina_Note(windowHandle);
		if      ( massimo_cnt == 5 )
			Data_Note(windowHandle);
		if      ( massimo_cnt == 6 )
			Corpo_note(windowHandle);
		if      ( massimo_cnt == 7 )
			Numero_di_pagina(windowHandle);
		if      ( massimo_cnt == 8 )
			Orientamento_pagina_note(windowHandle);
		if      ( massimo_cnt == 9 )
			Orientamento_diapositiva_note(windowHandle);
		if      ( massimo_cnt == 10)
			Temi_scherma_note(windowHandle);
		if      ( massimo_cnt == 11)
			Chiudi_Visualizzazione_Scherma_Note(windowHandle);
		if      ( massimo_cnt == 12)
			Imposta_Pagina_Scherma_Note(windowHandle);
		if      ( massimo_cnt == 13)
			Colore_Scherma_Note(windowHandle);
		if      ( massimo_cnt == 14) {
			Carattere_Scherma_Note(windowHandle);
		}
		if      ( massimo_cnt == 15)
			Effetti_Scherma_note(windowHandle);
		if      ( massimo_cnt == 16)
			Sfondo_Colore_Scherma_Note(windowHandle);
	}
	else if ( lettera == 'C' ) {
		if      ( massimo_cnt == 2 )
			return 0;
	}
	return 0;
}// Fine riconoscimento gesti multipli per il guanto sinistro

// Questo metodo si occupa di resettare i contatori del numero di gesti inviati
// Infatti, dopo aver raggiunto il numero di posizioni di cui si ha bisogno
// per identificare un gesto, questi devono essere obbligatoriamente resettati
// per un corretto funzionamento dell'applicazione.
void PowerPoint::Resetta_contatori()
{
	cntA = 0,cntB = 0,cntC = 0,cntE = 0,cntF = 0,
	cntG = 0,cntH = 0,cntL = 0,cntU = 0,cntW = 0,
	cntY = 0,cnt4 = 0,massimo_cnt = 0;
}
//
// Questo metodo si occupa del riconoscimento di gesti multipli
// La funzione è organizzata nel seguente modo , esiste un ciclo
// costante durante il quale vengono eseguite le seguenti operazioni
// 1) Reset di alcuni contatori che vengono utilizzati durante il
//    funzionamento dell'applicazione
// 2) Download informazioni dal dataglove (questa op. viene eseguita in un altro metodo)
// 2) I dati ottenuti dal data glove vengono salvati su un opportuna
//    matrice ( questa op. viene seguita in un altro metodo)
// 3) Calcola la distanza dei centroidi (viene eseguita in un altro metodo)
// 4) Somma le distanze ( per la distanza euclidea )
// 5) Trova gesto invocando un apposito metodo.
//    Il trova metodo restituisce un char che viene utilizzato per identificare
//    in che posizione si trova la mano, tra le posizioni disponibili che possono
//    essere riconosciute.
int PowerPoint::Riconosci_Gesti_Multipli(Project *proj,int gloveid1,int gloveid2)
{
	char gesto_Multiplo;
	int valore_restituito = 0;
	cntA = 0,cntB = 0,cntC = 0,cntE = 0,cntF = 0,cntG = 0,
	cntH = 0,cntL = 0,cntU = 0,cntW = 0,cntY = 0,cnt4 = 0;
	// ENTRO IN MODALITA' RICONOSCIMENTO GESTI MULTIPLI
	do 
	{
		Sleep(900);
		Inizializzatore();
		Pulisci_Seconda_Cornice();

		ScaricaDatieSalvaSuMatrice(proj,gloveid1,gloveid2);
		Calcola_Distanza_Centroidi();
		somma_distanze();

		gesto_Multiplo = Trova_Gesto();

		Spostati_Nella_Posizione(2,12);
		printf("Riconoscimento Gesti Multipli.");
		// Il gesto D viene utilizzato come blocco per fermare il riconoscimento
		// di gesti multipli
		if ( gesto_Multiplo == 'D' )
		{
			// Ottenuto il comando D eseguo scansione dei comandi ricevuti
			// viene trovato il numero di gesti massimo, cioe' se sono stati
			// trovati 5 gesti Y e 2 gesti W viene scelto Y
			Trova_Massimo_gesti_Multipli();
			// Il comando viene inviato a seconda del tipo di
			// guanto utilizzato.
			if ( (strcmp(tipo_guanto,"D") == 0 ) || (strcmp(tipo_guanto,"d") == 0 )) {
				// Per il guanto destro
				Cmd_PowerPoint_Multiplo();
			}
			if ( (strcmp(tipo_guanto,"S") == 0 ) || (strcmp(tipo_guanto,"s") == 0 )) {
				// Per il guanto sinistro
				valore_restituito = Cmd_PowerPoint_Multiplo_Sinistro();
			}
			if ( (strcmp(tipo_guanto,"E") == 0 ) || (strcmp(tipo_guanto,"e") == 0 )) {
				if ( destro == 1 && sinistro == 0) {
					destro = 0;
					Cmd_PowerPoint_Multiplo();
				}
				if ( sinistro == 1 && destro == 0) {
					sinistro = 0;
					Cmd_PowerPoint_Multiplo_Sinistro();
				}
			}
			// Completata questa fase il cmd e' stato invato con successo al PP
			// Mi metto in attesa di nuovi messaggi per questo motivo devo resettare
			// tutto con un apposito metodo.
			Resetta_contatori();
		}
		
		if ( gesto_Multiplo == 'A' ) {
			cntA++;
			Trova_Massimo_gesti_Multipli();
		}
		if ( gesto_Multiplo == 'C' ) {
			cntC++;
			Trova_Massimo_gesti_Multipli();
		}
		if ( gesto_Multiplo == 'E' ) {
			cntE++;
			Trova_Massimo_gesti_Multipli();
		}
		if ( gesto_Multiplo == 'F' ) {
			cntF++;
			Trova_Massimo_gesti_Multipli();
		}
		if ( gesto_Multiplo == 'G' ) {
			cntG++;
			Trova_Massimo_gesti_Multipli();
		}
		if ( gesto_Multiplo == 'H' ) {
			cntH++;
			Trova_Massimo_gesti_Multipli();
		}
		if ( gesto_Multiplo == 'L' ) {
			cntL++;
			Trova_Massimo_gesti_Multipli();
		}
		if ( gesto_Multiplo == 'U' ) {
			cntU++;
			Trova_Massimo_gesti_Multipli();
		}
		if ( gesto_Multiplo == 'W' ) {
			cntW++;
			Trova_Massimo_gesti_Multipli();
		}
		if ( gesto_Multiplo == 'Y' ) {
			cntY++;
			Trova_Massimo_gesti_Multipli();
		}
		if ( gesto_Multiplo == '4' ) {
			cnt4++;
			Trova_Massimo_gesti_Multipli();
		}
		Spostati_Nella_Posizione(2,20);
		printf("Ho contato A:%d B:%d C:%d E:%d F:%d G:%d \n",cntA,cntB,cntC,cntE,cntF,cntG);

		Spostati_Nella_Posizione(2,21);
		printf("Ho contato H:%d L:%d U:%d W:%d Y:%d 4:%d \n",cntH,cntL,cntU,cntW,cntY,cnt4);
	} while ( valore_restituito != 100);

	Pulisci_Seconda_Cornice();
	Spostati_Nella_Posizione(16,23);
	return 0;
}
// POWERPOINT - PRESENTAZIONE A PIENO SCHERMO 
// Questo metodo gestisce la presentazione a pieno schermo
int PowerPoint::Presentazione_A_Pieno_Schermo(Project *proj,int gloveid1,int gloveid2)
{
	char gesto_Multiplo;
	cntA = 0,cntB = 0,cntC = 0,cntE = 0,cntF = 0,
	cntG = 0,cntH = 0,cntL = 0,cntU = 0,cntW = 0,
	cntY = 0,cnt4=0;

	HWND windowHandle = FindWindow(0,(LPCWSTR)"ASD-lez7.ppt [modalità compatibilità] - Microsoft PowerPoint");
	Invia_F5(windowHandle);
	// Quando si entra in una presentazione a pieno schermo viene creata un'altra finestra
	// Per questo motivo e' necessario cambiare l'intestazione
	HWND windowHandle2 = FindWindow(0,(LPCWSTR)"Presentazione di PowerPoint - [ASD-lez7.ppt [modalità compatibilità]]");
	// ENTRO IN MODALITA' PRESENTAZIONE A PIENO SCHERMO
	do
	{
		Sleep(900);
		Inizializzatore();

		ScaricaDatieSalvaSuMatrice(proj,gloveid1,gloveid2);
		Calcola_Distanza_Centroidi();
		somma_distanze();

		gesto_Multiplo = Trova_Gesto();
		// Il gesto D viene utilizzato come blocco per fermare la presentazione
		// a pieno schermo
		if ( gesto_Multiplo == 'D' ) {
	
		}
		if ( gesto_Multiplo == 'A' )
			// Vai avanti con la slide successiva
			Invia_PageUp(windowHandle2);
		if ( gesto_Multiplo == 'F' )
			// Vai indietro con la slide precedente
			Invia_PageDown(windowHandle2);
	} while ( gesto_Multiplo != 'E');
	// Esci dalla presentazione a pieno schermo
	Invia_ESC(windowHandle2);
	Avvia_power(proj,gloveid1,gloveid2);
	return 0;
}// FINE PRESENTAZIONE A PIENO SCHERMO

// Questo metodo si occupa di trovare il numero massimo di gesti per un
// opportuno gesto, si tratta di un semplice ricerca del massimo con una
// serie di if per trovare il gesto che ha ottenuto piu' valori.
void PowerPoint::Trova_Massimo_gesti_Multipli()
{
	if      ( cntA > massimo_cnt ) {
		massimo_cnt = cntA;
		lettera = 'A';
	}
	if      ( cntB > massimo_cnt ) {
		massimo_cnt = cntB;
		lettera = 'B';
	}
	if ( cntC > massimo_cnt ) {
		massimo_cnt = cntC;
		lettera = 'C';
	}
	if ( cntE > massimo_cnt ) {
		massimo_cnt = cntE;
		lettera = 'E';
	}
	if ( cntF > massimo_cnt ) {
		massimo_cnt = cntF;
		lettera = 'F';
	}
	if ( cntG > massimo_cnt ) {
		massimo_cnt = cntG;
		lettera = 'G';
	}
	if ( cntH > massimo_cnt ) {
		massimo_cnt = cntH;
		lettera = 'H';
	}
	if ( cntL > massimo_cnt ) {
		massimo_cnt = cntL;
		lettera = 'L';
	}
	if ( cntU > massimo_cnt ) {
		massimo_cnt = cntU;
		lettera = 'U';
	}
	if ( cntW > massimo_cnt ) {
		massimo_cnt = cntW;
		lettera = 'W';
	}
	if ( cntY > massimo_cnt ) {
		massimo_cnt = cntY;
		lettera = 'Y';
	}
	if ( cnt4 > massimo_cnt ) {
		massimo_cnt = cnt4;
		lettera = '4';
	}
	Spostati_Nella_Posizione(31,21);
	printf("Valore max %d che corrisponde a %c\n",massimo_cnt,lettera);
}



void PowerPoint::Inizializzatore()
{
	vettore_corr[0] = 0.000000f,vettore_corr[1] = 0.000000f,vettore_corr[2] = 0.000000f,
	vettore_corr[3] = 0.000000f,vettore_corr[4] = 0.000000f,vettore_corr[5] = 0.000000f;

	vettore_corr2[0] = 0.000000f,vettore_corr2[1] = 0.000000f,vettore_corr2[2] = 0.000000f,
	vettore_corr2[3] = 0.000000f,vettore_corr2[4] = 0.000000f,vettore_corr2[5] = 0.000000f;

	vettore_pow[0] = 0.000000f,vettore_pow[1] = 0.000000f,vettore_pow[2] = 0.000000f,
	vettore_pow[3] = 0.000000f,vettore_pow[4] = 0.000000f,vettore_pow[5] = 0.000000f;

	vettore_pow2[0] = 0.000000f,vettore_pow2[1] = 0.000000f,vettore_pow2[2] = 0.000000f,
	vettore_pow2[3] = 0.000000f,vettore_pow2[4] = 0.000000f,vettore_pow2[5] = 0.000000f;

	val_min_global[0] = 0.000000f,val_min_global[1] = 0.000000f,val_min_global[2] = 0.000000f,
	val_min_global[3] = 0.000000f,val_min_global[4] = 0.000000f,val_min_global[5] = 0.000000f;

	val_min_global2[0] = 0.000000f,val_min_global2[1] = 0.000000f,val_min_global2[2] = 0.000000f,
	val_min_global2[3] = 0.000000f,val_min_global2[4] = 0.000000f,val_min_global2[5] = 0.000000f;

	vettore_min[0] = 5.000000f,vettore_min[1] = 5.000000f,vettore_min[2] = 5.000000f,
	vettore_min[3] = 5.000000f,vettore_min[4] = 5.000000f,vettore_min[5] = 5.000000f;

	vettore_corr_pow[0] = 0.000000f,vettore_corr_pow[1] = 0.000000f,vettore_corr_pow[2] = 0.000000f,
	vettore_corr_pow[3] = 0.000000f,vettore_corr_pow[4] = 0.000000f,vettore_corr_pow[5] = 0.000000f;

	vettore_corr_pow2[0] = 0.000000f,vettore_corr_pow2[1] = 0.000000f,vettore_corr_pow2[2] = 0.000000f,
	vettore_corr_pow2[3] = 0.000000f,vettore_corr_pow2[4] = 0.000000f,vettore_corr_pow2[5] = 0.000000f;

	dati_input[0] = 0.000000f,dati_input[1] = 0.000000f,dati_input[2] = 0.000000f,
	dati_input[3] = 0.000000f,dati_input[4] = 0.000000f,dati_input[5] = 0.000000f;

	val1   = 0.1f,val2   = 0.1f, val3   = 0.1f, val4   = 0.1f, val5   = 0.1f;
	val2_1 = 0.1f,val2_2 = 0.1f, val2_3 = 0.1f, val2_4 = 0.1f, val2_5 = 0.1f;
	// Inizializzo con valore 0 le variabili per le somme
	somma_1  = 0,somma_2  = 0,somma_3  = 0,somma_4  = 0,somma_5  = 0;
	somma2_1 = 0,somma2_2 = 0,somma2_3 = 0,somma2_4 = 0,somma2_5 = 0;
	// Inizializzo con valore 0 le variabili per le medie
	media_1  = 0,media_2  = 0,media_3  = 0,media_4  = 0,media_5  = 0;
	media2_1 = 0,media2_2 = 0,media2_3 = 0,media2_4 = 0,media2_5 = 0;
	// Contatori
	tipo_cnt = 0,tipo_cnt2= 0,tipo_ni  = 0;
	contatore = 0,contatore2 = 0,righe = 0,righe2 = 0;
	righe_conta = 0,righe_conta2 = 0,ju = 0,ju2 = 0;
	elo = 0,elo2= 0;
	destro_non_connesso = 0, sinistro_non_connesso = 0;
	primavolta = 0;
}
// Metodo per lo schermo diapositiva

// Inserisci schema diapositiva
// ALT + D + N
void PowerPoint::Inserisci_Schema_diapositiva(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare D
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x44;
	// Manda la lettera N
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x4E;
	// Rilasco la lettera N
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x4E;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera D
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x44;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}


// Chiudi Scherma diapositiva
// ALT + D + U 
void PowerPoint::Chiudi_Schema_diapositiva(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare D
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x44;
	// Manda la lettera U
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x55;
	// Rilasco la lettera U
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x55;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera D
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x44;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Inserisci Layout
// ALT + D + I
void PowerPoint::Inserisci_Layout(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare D
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x44;
	// Manda la lettera I
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x49;
	// Rilasco la lettera I
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x49;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera D
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x44;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Elimina Diapositiva
// ALT + D + 8
void PowerPoint::Elimina_Diapositiva(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare D
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x44;
	// Manda la numero 8
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x38;
	// Rilasco la numero 8
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x38;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera D
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x44;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Rinomina
// ALT + D + R
void PowerPoint::Rinomina_Diapositiva(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare D
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x44;
	// Manda la lettera R
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x52;
	// Rilasco la lettera R
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x52;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera D
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x44;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}


// Inserisci Segnaposto ( modalita' diapositiva )
// ALT + D + J
void PowerPoint::Inserisci_Segnaposto(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare D
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x44;
	// Manda la lettera J
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x4A;
	// Rilasco la lettera J
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x4A;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera D
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x44;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Modifica tema
// ALT + D + M
void PowerPoint::Modifica_tema(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare D
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x44;
	// Manda la lettera M
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x4D;
	// Rilasco la lettera M
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x4D;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera D
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x44;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Stili sfondo
// ALT + D + B
void PowerPoint::Stili_sfondo(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare D
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x44;
	// Manda la lettera B
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x42;
	// Rilasco la lettera B
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x42;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera D
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x44;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Imposta pagina
// ALT + D + S
void PowerPoint::Imposta_Pagina_schema_diapositiva(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare D
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x44;
	// Manda la lettera S
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x53;
	// Rilasco la lettera S
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x53;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera D
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x44;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Orientamento diapositiva
// ALT + D + O 
void PowerPoint::Orientamento_schema_Diapositiva(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare D
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x44;
	// Manda la lettera O
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x4F;
	// Rilasco la lettera O
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x4F;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera D
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x44;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Titolo Diapositiva
// ALT + D + E
void PowerPoint::Titolo_Diapositiva(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare D
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x44;
	// Manda la lettera E
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x45;
	// Rilasco la lettera E
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x45;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera D
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x44;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Pie di pagina ( scherma diapositiva )
// ALT + D + P
void PowerPoint::Pie_di_pagina(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare D
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x44;
	// Manda la lettera P
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x50;
	// Rilasco la lettera P
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x50;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera D
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x44;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Nascondi grafico di sfondo (scherma diapositiva)
// ALT + D + C
void PowerPoint::Nascondi_grafico_di_sfondo(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare D
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x44;
	// Manda la lettera C
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x43;
	// Rilasco la lettera C
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x43;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera D
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x44;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Cambia Colore (scherma diapositiva)
// ALT + D + TC
void PowerPoint::Cambia_Colore(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[8];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*10);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare D
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x47;
	// Manda la lettera T
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x54;
		// Manda la lettera C
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x43;
	// Manda la lettera C
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x43;
	// Rilasco la lettera T
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = 0x54;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilasco la lettera D
	input[6].type = INPUT_KEYBOARD;
	input[6].ki.wVk = 0x54;
	input[6].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[7].type = INPUT_KEYBOARD;
	input[7].ki.wVk = VK_LMENU;
	input[7].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(8, input, sizeof(INPUT));
}

// Tipi di carattere ( schema diapositiva)
// ALT + D + TT
void PowerPoint::Tipi_Di_carattere_diapositiva(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[8];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*10);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare D
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x47;
	// Manda la lettera T
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x54;
		// Manda la lettera T
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x54;
	// Manda la lettera T
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x54;
	// Rilasco la lettera T
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = 0x54;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilasco la lettera D
	input[6].type = INPUT_KEYBOARD;
	input[6].ki.wVk = 0x54;
	input[6].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[7].type = INPUT_KEYBOARD;
	input[7].ki.wVk = VK_LMENU;
	input[7].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(8, input, sizeof(INPUT));
}

// Effetti ( scherma diapositiva )
// ALT + D + TE
void PowerPoint::Effetti_diapositiva(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[8];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*10);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare D
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x47;
	// Manda la lettera T
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x54;
		// Manda la lettera E
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x45;
	// Manda la lettera E
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x45;
	// Rilasco la lettera T
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = 0x54;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilasco la lettera D
	input[6].type = INPUT_KEYBOARD;
	input[6].ki.wVk = 0x54;
	input[6].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[7].type = INPUT_KEYBOARD;
	input[7].ki.wVk = VK_LMENU;
	input[7].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(8, input, sizeof(INPUT));
}



// Metodi per la scherma note

// Intestazione ALT + E + E 
void PowerPoint::Intestazione_Note(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare E
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x45;
	// Manda la lettera E
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x45;
	// Rilasco la lettera E
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x45;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera E
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x45;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Immagine Diapositiva ALT + E + I
void PowerPoint::Immagine_Diapositiva(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare E
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x45;
	// Manda la lettera I
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x49;
	// Rilasco la lettera I
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x49;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera E
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x45;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Pie' di pagina ( scherma note) ALT + E + X
void PowerPoint::Pie_di_pagina_Note(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare E
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x45;
	// Manda la lettera X
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x58;
	// Rilasco la lettera X
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x58;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera E
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x45;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Data (Scherma note) ALT + E + D
void PowerPoint::Data_Note(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare E
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x45;
	// Manda la lettera D
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x44;
	// Rilasco la lettera D
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x44;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera E
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x45;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Corpo note ALT + E + P (scherma note)
void PowerPoint::Corpo_note(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare E
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x45;
	// Manda la lettera P
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x50;
	// Rilasco la lettera P
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x50;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera E
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x45;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Numero di pagina ALT + E + N
void PowerPoint::Numero_di_pagina(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare E
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x45;
	// Manda la lettera N
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x4E;
	// Rilasco la lettera N
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x4E;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera E
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x45;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Orientamento pagina note ALT + E + R
void PowerPoint::Orientamento_pagina_note(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare E
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x45;
	// Manda la lettera R
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x52;
	// Rilasco la lettera R
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x52;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera E
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x45;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Orientamento diapositiva ALT + E + O
void PowerPoint::Orientamento_diapositiva_note(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare E
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x45;
	// Manda la lettera O
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x4F;
	// Rilasco la lettera O
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x4F;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera E
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x45;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Temi scherma note ALT + E + Z3 +  M
void PowerPoint::Temi_scherma_note(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[10];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*10);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare E
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x45;
	// Invia il msg Z
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x5A;
	// Invia il msg 3
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x33;
	// Manda la lettera M
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x4D;
	// Rilasco la lettera M
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = 0x4D;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio il numero 3
	input[6].type = INPUT_KEYBOARD;
	input[6].ki.wVk = 0x33;
	input[6].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera Z
	input[7].type = INPUT_KEYBOARD;
	input[7].ki.wVk = 0x5A;
	input[7].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera E
	input[8].type = INPUT_KEYBOARD;
	input[8].ki.wVk = 0x45;
	input[8].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[9].type = INPUT_KEYBOARD;
	input[9].ki.wVk = VK_LMENU;
	input[9].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(10, input, sizeof(INPUT));
}

// Chiudi visualizzazione scherma note (ALT + E + U)
void PowerPoint::Chiudi_Visualizzazione_Scherma_Note(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare E
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x45;
	// Manda la lettera U
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x55;
	// Rilasco la lettera U
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x55;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera E
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x45;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Imposta Pagina Scherma note ( ALT + E + S )
void PowerPoint::Imposta_Pagina_Scherma_Note(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare E
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x45;
	// Manda la lettera S
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x53;
	// Rilasco la lettera S
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x53;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera E
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x45;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Colore Scherma Note ( ALT + E + TC )
void PowerPoint::Colore_Scherma_Note(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[8];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*8);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare E
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x45;
	// Manda la lettera T
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x54;
		// Manda la lettera C
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x43;
	// Manda la lettera C
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x43;
	// Rilasco la lettera T
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = 0x54;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilasco la lettera E
	input[6].type = INPUT_KEYBOARD;
	input[6].ki.wVk = 0x45;
	input[6].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[7].type = INPUT_KEYBOARD;
	input[7].ki.wVk = VK_LMENU;
	input[7].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(8, input, sizeof(INPUT));
}

// Tipi di carattere Scherma Note ( ALT + E + Z3 + TT )
void PowerPoint::Carattere_Scherma_Note(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[12];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*12);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare E
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x45;
	// Invia il msg Z
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x5A;
	// Invia il msg 3
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x33;
	// Manda la lettera T
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x54;
	// Manda la lettera T
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = 0x54;
	// Rilasco la lettera T
	input[6].type = INPUT_KEYBOARD;
	input[6].ki.wVk = 0x54;
	input[6].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera T
	input[7].type = INPUT_KEYBOARD;
	input[7].ki.wVk = 0x54;
	input[7].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio il numero 3
	input[8].type = INPUT_KEYBOARD;
	input[8].ki.wVk = 0x33;
	input[8].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera Z
	input[9].type = INPUT_KEYBOARD;
	input[9].ki.wVk = 0x5A;
	input[9].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera E
	input[10].type = INPUT_KEYBOARD;
	input[10].ki.wVk = 0x45;
	input[10].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[11].type = INPUT_KEYBOARD;
	input[11].ki.wVk = VK_LMENU;
	input[11].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(12, input, sizeof(INPUT));
}

// Effetti Scherma Stampanti ALT + A + TE
void PowerPoint::Effetti_Scherma_note(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[8];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*8);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare E
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x45;
	// Manda la lettera T
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x54;
	// Manda la lettera E
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x45;
	// Manda la lettera E
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x45;
	// Rilasco la lettera T
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = 0x54;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilasco la lettera E
	input[6].type = INPUT_KEYBOARD;
	input[6].ki.wVk = 0x45;
	input[6].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[7].type = INPUT_KEYBOARD;
	input[7].ki.wVk = VK_LMENU;
	input[7].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(8, input, sizeof(INPUT));
}

// Sfondo colore scherma note ALT + E + Z4 + B
void PowerPoint::Sfondo_Colore_Scherma_Note(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[10];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*10);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare E
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x45;
	// Manda il numero Z
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x5A;
	// Manda il numero 4
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x34;
	// Manda la lettera B
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x42;
	// Rilasco la lettera B
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = 0x42;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilasco il numero 4
	input[6].type = INPUT_KEYBOARD;
	input[6].ki.wVk = 0x34;
	input[6].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera Z
	input[7].type = INPUT_KEYBOARD;
	input[7].ki.wVk = 0x5A;
	input[7].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera E
	input[8].type = INPUT_KEYBOARD;
	input[8].ki.wVk = 0x45;
	input[8].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[9].type = INPUT_KEYBOARD;
	input[9].ki.wVk = VK_LMENU;
	input[9].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(10, input, sizeof(INPUT));
}


// Chiudi visualizzazionee Schema Stampanti ALT + A + U
void PowerPoint::Chiudi_visualizzazione_Schema_Stampanti(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare A
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x41;
	// Manda la lettera U
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x55;
	// Rilasco la lettera U
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x55;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera A
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x41;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Imposta pagina Schema stampanti ALT + A + S
void PowerPoint::Imposta_pagina_Schema_stampanti(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare A
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x41;
	// Manda la lettera S
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x53;
	// Rilasco la lettera S
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x53;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera A
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x41;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Orientamento Stampanti ALT + A + R
void PowerPoint::Orientamento_Schema_stampanti(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare A
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x41;
	// Manda la lettera R
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x52;
	// Rilasco la lettera R
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x52;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera A
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x41;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Orientamento Diapositiva ALT + A + O
void PowerPoint::Orientamento_Diapositiva_Schema_stampanti(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare A
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x41;
	// Manda la lettera O
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x4F;
	// Rilasco la lettera O
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x4F;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera A
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x41;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Diapositive per pagina ALT + A + V
void PowerPoint::Diapositive_per_pagina_Schema_stampanti(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare A
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x41;
	// Manda la lettera V
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x56;
	// Rilasco la lettera V
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x56;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera A
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x41;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Temi schema stampanti ALT + A + M
void PowerPoint::Temi_schema_stampanti(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare A
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x41;
	// Manda la lettera M
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x4D;
	// Rilasco la lettera M
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x4D;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera A
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x41;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Stili sfondo Schema_stampanti ALT + A + B
void PowerPoint::Stili_Sfondo_Schema_stampanti(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare A
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x41;
	// Manda la lettera B
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x42;
	// Rilasco la lettera B
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x42;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera A
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x41;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Data Schema Stampanti ALT + A + D
void PowerPoint::Data_Schema_Stampanti(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare A
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x41;
	// Manda la lettera D
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x44;
	// Rilasco la lettera D
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x44;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera A
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x41;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Intestazionne schema Stampanti ALT + A + E
void PowerPoint::Intestazionne_Schema_Stampanti(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare A
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x41;
	// Manda la lettera E
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x45;
	// Rilasco la lettera E
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x45;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera A
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x41;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Pie' di pagina schema stampanti ALT + A + X
void PowerPoint::Pie_di_pagina_schema_Stampanti(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare A
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x41;
	// Manda la lettera X
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x58;
	// Rilasco la lettera X
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x58;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera A
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x41;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Numero di pagina scherma stampanti ALT + A + N
void PowerPoint::Numero_di_pagina_scherma_stampanti(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare A
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x41;
	// Manda la lettera N
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x4E;
	// Rilasco la lettera N
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x4E;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera A
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x41;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Colori Schema Stampanti  ALT + A + TC
void PowerPoint::Colori_Schema_Stampanti(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[8];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*8);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare A
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x41;
	// Manda la lettera T
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x54;
		// Manda la lettera C
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x43;
	// Manda la lettera C
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x43;
	// Rilasco la lettera T
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = 0x54;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilasco la lettera A
	input[6].type = INPUT_KEYBOARD;
	input[6].ki.wVk = 0x41;
	input[6].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[7].type = INPUT_KEYBOARD;
	input[7].ki.wVk = VK_LMENU;
	input[7].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(8, input, sizeof(INPUT));
}

// Tipi di carattere schema stampanti ALT + A + TT
void PowerPoint::Tipi_Di_carattere_stampanti(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[8];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*8);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare A
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x41;
	// Manda la lettera T
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x54;
		// Manda la lettera T
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x54;
	// Manda la lettera T
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x54;
	// Rilasco la lettera T
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = 0x54;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilasco la lettera A
	input[6].type = INPUT_KEYBOARD;
	input[6].ki.wVk = 0x41;
	input[6].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[7].type = INPUT_KEYBOARD;
	input[7].ki.wVk = VK_LMENU;
	input[7].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(8, input, sizeof(INPUT));
}

// Effetti Schema stampanti ALT + A + TE
void PowerPoint::Effetti_Scherma_Stampanti(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[8];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*8);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare A
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x41;
	// Manda la lettera T
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x54;
		// Manda la lettera E
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x45;
	// Manda la lettera E
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x45;
	// Rilasco la lettera T
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = 0x54;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilasco la lettera A
	input[6].type = INPUT_KEYBOARD;
	input[6].ki.wVk = 0x41;
	input[6].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[7].type = INPUT_KEYBOARD;
	input[7].ki.wVk = VK_LMENU;
	input[7].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(8, input, sizeof(INPUT));
}

// Metodi per i comandi di tipo grassetto,corsivo ecc..


// Passa ad un altra finestra
// Permette di mettere in evidenza una finestra presente nell'applicazione
// in uso
void PowerPoint::PassaAdAltraFinestra(HWND Finestra)
{   // ALT + INVIO
	if( Finestra == NULL )
		cout << "not found";
	SetForegroundWindow(Finestra);
	INPUT input[4];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*4);
	// Primo msg da inviare CTRL
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Invia G
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = VK_RETURN;
	// Rilasco G
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = VK_RETURN;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia CONTROL
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = VK_LMENU;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4,input,sizeof(INPUT) );
}


// Invia comando Grassetto
void PowerPoint::Invia_Grassetto(HWND Finestra)
{   // CTRL + G
	if( Finestra == NULL )
		cout << "not found";
	SetForegroundWindow(Finestra);
	INPUT input[4];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*4);
	// Primo msg da inviare CTRL
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LCONTROL;
	// Invia G
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x47;
	// Rilasco G
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x47;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia CONTROL
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = VK_LCONTROL;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4,input,sizeof(INPUT) );
}

// Invia Corsivo
void PowerPoint::Invia_Corsivo(HWND Finestra)
{// CTRL + I
	if( Finestra == NULL )
		cout << "not found";
	SetForegroundWindow(Finestra);
	INPUT input[4];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*4);
	// Primo msg da inviare CTRL
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LCONTROL;
	// Invia I
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x49;
	// Rilasco I
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x49;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia CONTROL
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = VK_LCONTROL;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4, input, sizeof(INPUT));
}
// Invia SottoLineato
void PowerPoint::Invia_SottoLineato(HWND Finestra)
{// CTRL + S
	if( Finestra == NULL )
		cout << "not found";
	SetForegroundWindow(Finestra);
	INPUT input[4];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*4);
	// Primo msg da inviare CTRL
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LCONTROL;
	// Invia S
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x53;
	// Rilasco S
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x53;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia CONTROL
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = VK_LCONTROL;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4, input, sizeof(INPUT) );
}

// Controllo ortografico:     ALT + R + F
void PowerPoint::Invia_ControlloOrtografico(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare R
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x52;
	// Manda la lettera F
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x46;
	// Rilasco la lettera F
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x46;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera R
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x52;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Ricerche :                 ALT + R + 9
void PowerPoint::Invia_Ricerche(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare R
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x52;
	// Manda il numero 9
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x39;
	// Rilasco il numero 9
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x39;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera R
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x52;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}
// ZOOM     :                 ALT + N + 6
void PowerPoint::Invia_ZOOM(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare N
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x4E;
	// Manda il numero 6
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x36;
	// Rilasco il numero 6
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x36;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera N
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x4E;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Visualizzazione Normale:   ALT + N + L
void PowerPoint::Visualizzazione_Normale(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare N
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x4E;
	// Manda il numero L
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x4C;
	// Rilasco il numero L
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x4C;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera L
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x4E;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Sequenza diapositive   :   ALT + N + I
void PowerPoint::Sequenza_diapositive(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare N
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x4E;
	// Manda il numero I
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x49;
	// Rilasco il numero I
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x49;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera N
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x4E;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Pagina note            :   ALT + N + T
void PowerPoint::Pagina_note(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare N
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x4E;
	// Manda il numero T
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x54;
	// Rilasco il numero T
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x54;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera N
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x4E;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Visualizzazione di lettura:ALT + N + S
void PowerPoint::Visualizzazione_di_lettura(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare N
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x4E;
	// Manda il numero S
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x53;
	// Rilasco il numero S
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x53;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera N
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x4E;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Scherma diapositiva       :ALT + N + M
void PowerPoint::Scherma_diapositiva(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare N
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x4E;
	// Manda il numero M
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x4D;
	// Rilasco il numero M
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x4D;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera N
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x4E;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Scherma stampati          :ALT + N + H
void PowerPoint::Scherma_stampati(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare N
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x4E;
	// Manda il numero H
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x48;
	// Rilasco il numero H
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x48;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera N
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x4E;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Note                      :ALT + N + E
void PowerPoint::Note(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare N
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x4E;
	// Manda il numero E
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x45;
	// Rilasco il numero E
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x45;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera N
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x4E;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Righello                  :ALT + N + 3
void PowerPoint::Righello(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare N
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x4E;
	// Manda il numero 3
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x33;
	// Rilasco il numero 3
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x33;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera N
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x4E;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Griglia                   :ALT + N + G
void PowerPoint::Griglia(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare N
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x4E;
	// Manda il numero G
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x47;
	// Rilasco il numero G
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x47;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera N
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x4E;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Guide                     :ALT + N + X
void PowerPoint::Guide(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare N
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x4E;
	// Manda il numero X
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x58;
	// Rilasco il numero X
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x58;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera N
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x4E;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Tipo di transizioni delle slides: ALT + B + T
void PowerPoint::TipoTransizioniSlides(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare B
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x42;
	// Manda il numero T
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x54;
	// Rilasco il numero T
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x54;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera B
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x42;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Stili sfondo:            ALT + G1 + B
void PowerPoint::Stili_Sfondo(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[8];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*8);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare G
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x47;
	// Manda il numero 1
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x31;
	// Manda la lettera B
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x42;
	// Rilasco la lettera B
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x42;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera 1
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = 0x31;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera G
	input[6].type = INPUT_KEYBOARD;
	input[6].ki.wVk = 0x47;
	input[6].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[7].type = INPUT_KEYBOARD;
	input[7].ki.wVk = VK_LMENU;
	input[7].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(8, input, sizeof(INPUT));
}

// Tipi di carattere:       ALT + G1 + TT
void PowerPoint::TipiDiCarattere(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[10];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*10);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare G
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x47;
	// Manda il numero 1
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x31;
		// Manda la lettera T ( prima volta)
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x54;
	// Manda la lettera T ( seconda volta)
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x54;
	// Rilasco la lettera T ( prima volta )
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = 0x54;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilasco la lettera T ( seconda volta )
	input[6].type = INPUT_KEYBOARD;
	input[6].ki.wVk = 0x54;
	input[6].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera 1
	input[7].type = INPUT_KEYBOARD;
	input[7].ki.wVk = 0x31;
	input[7].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera G
	input[8].type = INPUT_KEYBOARD;
	input[8].ki.wVk = 0x47;
	input[8].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[9].type = INPUT_KEYBOARD;
	input[9].ki.wVk = VK_LMENU;
	input[9].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(10, input, sizeof(INPUT));
}

// Colori           :       ALT + G1 + TC
void PowerPoint::ScegliColori(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[10];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*10);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare G
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x47;
	// Manda il numero 1
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x31;
	// Manda la lettera T ( prima volta)
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x54;
	// Manda la lettera C
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x43;
	// Rilasco la lettera C
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = 0x43;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilasco la lettera T
	input[6].type = INPUT_KEYBOARD;
	input[6].ki.wVk = 0x54;
	input[6].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera 1
	input[7].type = INPUT_KEYBOARD;
	input[7].ki.wVk = 0x31;
	input[7].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera G
	input[8].type = INPUT_KEYBOARD;
	input[8].ki.wVk = 0x47;
	input[8].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[9].type = INPUT_KEYBOARD;
	input[9].ki.wVk = VK_LMENU;
	input[9].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(10, input, sizeof(INPUT));
}

// Effetti          :       ALT + G1 + TE
void PowerPoint::ScegliEffetti(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[10];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*10);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare G
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x47;
	// Manda il numero 1
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x31;
	// Manda la lettera T
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x54;
	// Manda la lettera E
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x45;
	// Rilasco la lettera E
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = 0x45;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilasco la lettera T
	input[6].type = INPUT_KEYBOARD;
	input[6].ki.wVk = 0x54;
	input[6].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera 1
	input[7].type = INPUT_KEYBOARD;
	input[7].ki.wVk = 0x31;
	input[7].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera G
	input[8].type = INPUT_KEYBOARD;
	input[8].ki.wVk = 0x47;
	input[8].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[9].type = INPUT_KEYBOARD;
	input[9].ki.wVk = VK_LMENU;
	input[9].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(10, input, sizeof(INPUT));
}

// Scelta tema      :       ALT + G1 + M
void PowerPoint::Scelta_Tema(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[8];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*8);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare G
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x47;
	// Manda il numero 1
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x31;
	// Manda la lettera M
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x4D;
	// Rilasco la lettera M
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x4D;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera 1
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = 0x31;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera G
	input[6].type = INPUT_KEYBOARD;
	input[6].ki.wVk = 0x47;
	input[6].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[7].type = INPUT_KEYBOARD;
	input[7].ki.wVk = VK_LMENU;
	input[7].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(8, input, sizeof(INPUT));
}

// Orientamento diapositiva:ALT + G1 + O
void PowerPoint::Orientamento_Diapositiva(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[8];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*8);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare G
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x47;
	// Manda il numero 1
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x31;
	// Manda la lettera O
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x4F;
	// Rilasco la lettera O
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x4F;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera 1
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = 0x31;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera G
	input[6].type = INPUT_KEYBOARD;
	input[6].ki.wVk = 0x47;
	input[6].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[7].type = INPUT_KEYBOARD;
	input[7].ki.wVk = VK_LMENU;
	input[7].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(8, input, sizeof(INPUT));
}

// Imposta Pagina          :ALT + G1 + S
void PowerPoint::Imposta_Pagina(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[8];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*8);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare G
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x47;
	// Manda il numero 1
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x31;
	// Manda la lettera S
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x53;
	// Rilasco la lettera S
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x53;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera 1
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = 0x31;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera G
	input[6].type = INPUT_KEYBOARD;
	input[6].ki.wVk = 0x47;
	input[6].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[7].type = INPUT_KEYBOARD;
	input[7].ki.wVk = VK_LMENU;
	input[7].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(8, input, sizeof(INPUT));
}



void PowerPoint::FrecciaSu(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[3];
	memset(input, 0, sizeof(INPUT)*3);
	// 
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x26;
	// Rilasco arraw
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x26;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(3, input, sizeof(INPUT));
}

void PowerPoint::FrecciaGiu( HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[3];
	memset(input, 0, sizeof(INPUT)*3);
	// 
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x28;
	// Rilasco 
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x28;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(3, input, sizeof(INPUT));
}

void PowerPoint::FrecciaDestra( HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[3];
	memset(input, 0, sizeof(INPUT)*3);
	// Arraw right
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x27;
	// Rilasco 
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x27;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(3, input, sizeof(INPUT));
}

void PowerPoint::FrecciaSinistra( HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[3];
	memset(input, 0, sizeof(INPUT)*3);;
	// Arraw left
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x25;
	// Rilasco 
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x25;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(3, input, sizeof(INPUT));
}


// Nuova Diapositiva           ALT + H + I
void PowerPoint::Nuova_Diapositiva(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare H
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x48;
	// Manda il numero I
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x49;
	// Rilasco il numero I
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x49;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera H
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x48;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}


// Elenchi puntati             ALT + H + U
void PowerPoint::Elenchi_Puntati(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[7];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*7);
	//
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x09;
	// Primo msg da inviare ALT_LEFT
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = VK_LMENU;
	// Secondo msg da inviare H
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x48;
	// Manda il numero U
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x55;
	// Rilasco il numero U
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x55;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera H
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = 0x48;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[6].type = INPUT_KEYBOARD;
	input[6].ki.wVk = VK_LMENU;
	input[6].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(7, input, sizeof(INPUT));
}

// Elenchi numerati            ALT + H + N
void PowerPoint::Elenchi_Numerati(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[7];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*7);
	//
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x09;
	// Primo msg da inviare ALT_LEFT
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = VK_LMENU;
	// Secondo msg da inviare H
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x48;
	// Manda il numero N
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x4E;
	// Rilasco il numero N
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x4E;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera H
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = 0x48;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[6].type = INPUT_KEYBOARD;
	input[6].ki.wVk = VK_LMENU;
	input[6].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(7, input, sizeof(INPUT));
}


// Cambia tipo di carattere    ALT + H + FF
void PowerPoint::CambiaTipiDiCarattere(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[8];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*8);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare H
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x48;
	// Manda il numero F
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x46;
	// Manda la lettera F
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x46;
	// Rilasco la lettera F
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x46;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilasco la lettera F
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = 0x46;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera H
	input[6].type = INPUT_KEYBOARD;
	input[6].ki.wVk = 0x48;
	input[6].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[7].type = INPUT_KEYBOARD;
	input[7].ki.wVk = VK_LMENU;
	input[7].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(8, input, sizeof(INPUT));
}

// Incrementa di 1 dimn caratt ALT + H + FG
void PowerPoint::Incrementa_dimcaratt(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[8];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*8);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare H
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x48;
	// Manda il numero F
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x46;
	// Manda la lettera G
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x47;
	// Rilasco la lettera G
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x47;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilasco la lettera F
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = 0x46;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera H
	input[6].type = INPUT_KEYBOARD;
	input[6].ki.wVk = 0x48;
	input[6].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[7].type = INPUT_KEYBOARD;
	input[7].ki.wVk = VK_LMENU;
	input[7].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(8, input, sizeof(INPUT));
}

// Decrementa di 1 la dim cara ALT + H + FK
void PowerPoint::Decrementa_dimcaratt(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[8];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*8);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare H
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x48;
	// Manda il numero F
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x46;
	// Manda la lettera K
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x4B;
	// Rilasco la lettera K
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x4B;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilasco la lettera F
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = 0x46;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera H
	input[6].type = INPUT_KEYBOARD;
	input[6].ki.wVk = 0x48;
	input[6].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[7].type = INPUT_KEYBOARD;
	input[7].ki.wVk = VK_LMENU;
	input[7].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(8, input, sizeof(INPUT));
}

// Cambia dimensione carattere ALT + H + FS
void PowerPoint::Cambia_Dimensione_Carattere(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[9];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*9);
	// 
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x09;
	// Primo msg da inviare ALT_LEFT
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = VK_LMENU;
	// Secondo msg da inviare H
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x48;
	// Manda il numero F
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x46;
	// Manda la lettera S
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x53;
	// Rilasco la lettera S
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = 0x53;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilasco la lettera F
	input[6].type = INPUT_KEYBOARD;
	input[6].ki.wVk = 0x46;
	input[6].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera H
	input[7].type = INPUT_KEYBOARD;
	input[7].ki.wVk = 0x48;
	input[7].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[8].type = INPUT_KEYBOARD;
	input[8].ki.wVk = VK_LMENU;
	input[8].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(9, input, sizeof(INPUT));
}

// Inserisci Immagine          ALT + C + G
void PowerPoint::Inserisci_Immagine(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare C
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x43;
	// Manda il numero G
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x47;
	// Rilasco il numero G
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x47;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera C
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x43;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Crea tabella                ALT + C + 4
void PowerPoint::Crea_Tabella(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare C
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x43;
	// Manda il numero 4
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x34;
	// Rilasco il numero 4
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x34;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera C
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x43;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Inserisci Grafico           ALT + C + C
void PowerPoint::Inserisci_Grafico(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare C
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x43;
	// Manda il carattere C
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x43;
	// Rilasco il carattere C
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x43;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera C
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x43;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}


// Intestazione a piè di Pag   ALT + C + Q
void PowerPoint::Intestazione_pie_Pagina(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare C
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x43;
	// Manda il carattere Q
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x51;
	// Rilasco il carattere Q
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x51;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera C
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x43;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

// Inserisci forme             ALT + C + SR
void PowerPoint::Inserisci_Forme(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[8];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*8);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare C
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x43;
	// Manda il numero S
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x53;
	// Manda la lettera R
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x52;
	// Rilasco la lettera R
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x52;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilasco la lettera S
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = 0x53;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera H
	input[6].type = INPUT_KEYBOARD;
	input[6].ki.wVk = 0x48;
	input[6].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[7].type = INPUT_KEYBOARD;
	input[7].ki.wVk = VK_LMENU;
	input[7].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(8, input, sizeof(INPUT));
}

// Trova nel testo             ALT + H + FD
void PowerPoint::Trova_Nel_Testo(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[8];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*8);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare H
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x48;
	// Manda il numero F
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x46;
	// Manda la lettera D
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x44;
	// Rilasco la lettera D
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x44;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilasco la lettera F
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = 0x46;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera H
	input[6].type = INPUT_KEYBOARD;
	input[6].ki.wVk = 0x48;
	input[6].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[7].type = INPUT_KEYBOARD;
	input[7].ki.wVk = VK_LMENU;
	input[7].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(8, input, sizeof(INPUT));
}

// Sostituisci                 ALT + H + R
void PowerPoint::Sostituisci(HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare ALT_LEFT
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare H
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x48;
	// Manda il carattere R
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x52;
	// Rilasco il carattere R
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x52;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio la lettera H
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x48;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}



/// Definizione altri metodi






// Creo uno spazio
void PowerPoint::Invia_Spazio( HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia un shortcut keyboard di tipo VK_PRIOR
	// Questo messaggio equivale a Page Up
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_SPACE;
	// Rilasco il comando
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = VK_SPACE;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

// comando "invio"
void PowerPoint::Invia_Return( HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia un shortcut keyboard di tipo VK_PRIOR
	// Questo messaggio equivale a Page Up
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_RETURN;
	// Rilasco il comando
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = VK_RETURN;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

// Cancella carattere
void PowerPoint::Invia_Cancella( HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia un shortcut keyboard di tipo VK_PRIOR
	// Questo messaggio equivale a Page Up
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_BACK;
	// Rilasco il comando
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = VK_BACK;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}
// invia virgola
void PowerPoint::Invia_Virgola( HWND Finestra)
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia un shortcut keyboard di tipo VK_PRIOR
	// Questo messaggio equivale a Page Up
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0xBC;
	// Rilasco il comando
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0xBC;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}


void PowerPoint::Invia_Punto( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia un shortcut keyboard di tipo VK_PRIOR
	// Questo messaggio equivale a Page Up
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0xBE;
	// Rilasco il comando
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0xBE;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

void PowerPoint::Invia_PuntoEscalamativo( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia un shortcut keyboard di tipo VK_PRIOR
	// Questo messaggio equivale a Page Up
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_OEM_1;
	// Rilasco il comando
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk =  VK_OEM_1;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

void PowerPoint::Invia_PuntoInterrogativo( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);

	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia un shortcut keyboard di tipo VK_PRIOR
	// Questo messaggio equivale a Page Up
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk =  VK_OEM_4;
	// Rilasco il comando
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk =  VK_OEM_4;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

///////////////////////////////////////////////////////
////////////////////////////
// INIZIO METODI PER DEFINIRE L'ALFABETO
////////////////////////////
///////////////////////////////////////////////////////

// Realizza il carattere è
void PowerPoint::e_accentata( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[4];
	memset(input, 0, sizeof(INPUT)*4);
	// invia il comando per è
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0xBA;
	// Rilasco il comando è
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0xBA;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4, input, sizeof(INPUT));
}


// Realizza il carattere ù
void PowerPoint::u_accentata( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[4];
	memset(input, 0, sizeof(INPUT)*4);
	// invia il comando per ù
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0xBF;
	// Rilasco il comando ù
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0xBF;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4, input, sizeof(INPUT));
}

// Realizza il carattere ò
void PowerPoint::o_accentata( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[4];
	memset(input, 0, sizeof(INPUT)*4);
	// invia il comando per ò
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0xC0;
	// Rilasco il comando ò
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0xC0;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4, input, sizeof(INPUT));
}

void PowerPoint::Invia_BLOCNUM( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// invia il comando per è
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_NUMLOCK;
	// Rilasco il comando ò
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = VK_NUMLOCK;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}
/*
void PowerPoint::a_accentata( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[10];
	memset(input, 0, sizeof(INPUT)*10);
	// Alt sinitro
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// invia il comando per 0
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = VK_NUMPAD1;
	// Invia il comando per 2
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = VK_NUMPAD3;
	// Invia il comando per 2
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = VK_NUMPAD3;
	// Rilasco il comando 4
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = VK_NUMPAD3;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia il comando per 2
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_NUMPAD3;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia il comando per 2
	input[6].type = INPUT_KEYBOARD;
	input[6].ki.wVk = VK_NUMPAD1;
	input[6].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilasci alt sinitro
	input[7].type = INPUT_KEYBOARD;
	input[7].ki.wVk = VK_LMENU;
	input[7].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(10, input, sizeof(INPUT));

}

// a con l'accento
/*
void PowerPoint::a_accentata( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[10];
	memset(input, 0, sizeof(INPUT)*10);
	// Blocco num VK_NUMLOCK
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_NUMLOCK;
	// ALT SINISTRO
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = VK_LMENU;
	// Invio i numero 1 (versione tastierino numerico)
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x61;
	// Invio il numero 3
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x63;
	// Invio il numero 3
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x63;
	// Rilascio il numero 3
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = 0x63;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio il numero 3 per la seconda volta
	input[6].type = INPUT_KEYBOARD;
	input[6].ki.wVk = 0x63;
	input[6].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio il numero 1
	input[7].type = INPUT_KEYBOARD;
	input[7].ki.wVk = 0x61;
	input[7].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio ALT sinistro
	input[8].type = INPUT_KEYBOARD;
	input[8].ki.wVk = VK_LMENU;
	input[8].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio bloc num
	input[9].type = INPUT_KEYBOARD;
	input[9].ki.wVk = VK_NUMLOCK;
	// Invio il msg
	SendInput(10, input, sizeof(INPUT));
}
*/
// A
void PowerPoint::Invia_Lettera_A_Grande( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia un shortcut keyboard di tipo VK_PRIOR
	// Questo messaggio equivale a Page Up
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x41;
	// Rilasco il comando
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x41;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

void PowerPoint::Invia_Lettera_A_Piccolo( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[4];
	memset(input, 0, sizeof(INPUT)*4);
	// Mando SHIFT sinistro
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LSHIFT;
	// Invio A
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x41;
	// Rilasco il comando A
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x41;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio SHIFT sinistro
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = VK_LSHIFT;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4, input, sizeof(INPUT));
}

// Metodo per gestire la LETTERA B
void PowerPoint::Invia_Lettera_B_Grande( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia un shortcut keyboard di tipo VK_PRIOR
	// Questo messaggio equivale a Page Up
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x42;
	// Rilasco il comando
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x42;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

void PowerPoint::Invia_Lettera_B_Piccolo( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[4];
	memset(input, 0, sizeof(INPUT)*4);
	// Mando SHIFT sinistro
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LSHIFT;
	// Invio B
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x42;
	// Rilasco il comando B
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x42;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio SHIFT sinistro
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = VK_LSHIFT;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4, input, sizeof(INPUT));
}

// LETTERA C
void PowerPoint::Invia_Lettera_C_Grande( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia un shortcut keyboard di tipo VK_PRIOR
	// Questo messaggio equivale a Page Up
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x43;
	// Rilasco il comando
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x43;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

void PowerPoint::Invia_Lettera_C_Piccolo( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[4];
	memset(input, 0, sizeof(INPUT)*4);
	// Mando SHIFT sinistro
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LSHIFT;
	// Invio B
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x43;
	// Rilasco il comando B
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x43;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio SHIFT sinistro
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = VK_LSHIFT;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4, input, sizeof(INPUT));
}
// D
void PowerPoint::Invia_Lettera_D_Grande( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia un shortcut keyboard di tipo VK_PRIOR
	// Questo messaggio equivale a Page Up
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x44;
	// Rilasco il comando
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x44;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

void PowerPoint::Invia_Lettera_D_Piccolo( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[4];
	memset(input, 0, sizeof(INPUT)*4);
	// Mando SHIFT sinistro
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LSHIFT;
	// Invio D
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x44;
	// Rilasco il comando D
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x44;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio SHIFT sinistro
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = VK_LSHIFT;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4, input, sizeof(INPUT));
}

// E
void PowerPoint::Invia_Lettera_E_Grande( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia comando E
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x45;
	// Rilasco il comando E
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x45;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

void PowerPoint::Invia_Lettera_E_Piccolo( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[4];
	memset(input, 0, sizeof(INPUT)*4);
	// Mando SHIFT sinistro
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LSHIFT;
	// Invio E
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x45;
	// Rilasco il comando E
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x45;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio SHIFT sinistro
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = VK_LSHIFT;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4, input, sizeof(INPUT));
}
// F
void PowerPoint::Invia_Lettera_F_Grande( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia comando F
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x46;
	// Rilasco il comando F
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x46;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

void PowerPoint::Invia_Lettera_F_Piccolo( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[4];
	memset(input, 0, sizeof(INPUT)*4);
	// Mando SHIFT sinistro
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LSHIFT;
	// Invio F
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x46;
	// Rilasco il comando F
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x46;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio SHIFT sinistro
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = VK_LSHIFT;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4, input, sizeof(INPUT));
}
// G
void PowerPoint::Invia_Lettera_G_Grande( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia comando G
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x47;
	// Rilasco il comando G
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x47;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

void PowerPoint::Invia_Lettera_G_Piccolo( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[4];
	memset(input, 0, sizeof(INPUT)*4);
	// Mando SHIFT sinistro
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LSHIFT;
	// Invio G
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x47;
	// Rilasco il comando G
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x47;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio SHIFT sinistro
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = VK_LSHIFT;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4, input, sizeof(INPUT));
}
// H
void PowerPoint::Invia_Lettera_H_Grande( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia comando H
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x48;
	// Rilasco il comando H
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x48;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

void PowerPoint::Invia_Lettera_H_Piccolo( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[4];
	memset(input, 0, sizeof(INPUT)*4);
	// Mando SHIFT sinistro
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LSHIFT;
	// Invio H
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x48;
	// Rilasco il comando H
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x48;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio SHIFT sinistro
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = VK_LSHIFT;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4, input, sizeof(INPUT));
}
// I
void PowerPoint::Invia_Lettera_I_Grande( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia comando I
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x49;
	// Rilasco il comando I
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x49;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

void PowerPoint::Invia_Lettera_I_Piccolo( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[4];
	memset(input, 0, sizeof(INPUT)*4);
	// Mando SHIFT sinistro
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LSHIFT;
	// Invio I
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x49;
	// Rilasco il comando I
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x49;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio SHIFT sinistro
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = VK_LSHIFT;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4, input, sizeof(INPUT));
}
// J
void PowerPoint::Invia_Lettera_J_Grande( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia comando J
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x4A;
	// Rilasco il comando J
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x4A;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

void PowerPoint::Invia_Lettera_J_Piccolo( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[4];
	memset(input, 0, sizeof(INPUT)*4);
	// Mando SHIFT sinistro
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LSHIFT;
	// Invio J
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x4A;
	// Rilasco il comando J
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x4A;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio SHIFT sinistro
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = VK_LSHIFT;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4, input, sizeof(INPUT));
}
// K
void PowerPoint::Invia_Lettera_K_Grande( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia comando K
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x4B;
	// Rilasco il comando K
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x4B;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

void PowerPoint::Invia_Lettera_K_Piccolo( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[4];
	memset(input, 0, sizeof(INPUT)*4);
	// Mando SHIFT sinistro
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LSHIFT;
	// Invio K
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x4B;
	// Rilasco il comando K
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x4B;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio SHIFT sinistro
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = VK_LSHIFT;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4, input, sizeof(INPUT));
}
// L
void PowerPoint::Invia_Lettera_L_Grande( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia comando L
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x4C;
	// Rilasco il comando L
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x4C;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

void PowerPoint::Invia_Lettera_L_Piccolo( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[4];
	memset(input, 0, sizeof(INPUT)*4);
	// Mando SHIFT sinistro
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LSHIFT;
	// Invio L
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x4C;
	// Rilasco il comando L
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x4C;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio SHIFT sinistro
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = VK_LSHIFT;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4, input, sizeof(INPUT));
}
// M
void PowerPoint::Invia_Lettera_M_Grande( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia comando M
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x4D;
	// Rilasco il comando M
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x4D;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

void PowerPoint::Invia_Lettera_M_Piccolo( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[4];
	memset(input, 0, sizeof(INPUT)*4);
	// Mando SHIFT sinistro
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LSHIFT;
	// Invio M
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x4D;
	// Rilasco il comando M
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x4D;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio SHIFT sinistro
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = VK_LSHIFT;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4, input, sizeof(INPUT));
}
// N
void PowerPoint::Invia_Lettera_N_Grande( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia comando N
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x4E;
	// Rilasco il comando N
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x4E;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

void PowerPoint::Invia_Lettera_N_Piccolo( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[4];
	memset(input, 0, sizeof(INPUT)*4);
	// Mando SHIFT sinistro
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LSHIFT;
	// Invio N
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x4E;
	// Rilasco il comando N
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x4E;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio SHIFT sinistro
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = VK_LSHIFT;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4, input, sizeof(INPUT));
}
// O
void PowerPoint::Invia_Lettera_O_Grande( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia comando O
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x4F;
	// Rilasco il comando O
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x4F;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

void PowerPoint::Invia_Lettera_O_Piccolo( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[4];
	memset(input, 0, sizeof(INPUT)*4);
	// Mando SHIFT sinistro
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LSHIFT;
	// Invio O
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x4F;
	// Rilasco il comando O
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x4F;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio SHIFT sinistro
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = VK_LSHIFT;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4, input, sizeof(INPUT));
}
// P
void PowerPoint::Invia_Lettera_P_Grande( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia comando P
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x50;
	// Rilasco il comando P
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x50;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

void PowerPoint::Invia_Lettera_P_Piccolo( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[4];
	memset(input, 0, sizeof(INPUT)*4);
	// Mando SHIFT sinistro
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LSHIFT;
	// Invio P
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x50;
	// Rilasco il comando P
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x50;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio SHIFT sinistro
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = VK_LSHIFT;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4, input, sizeof(INPUT));
}
// Q
void PowerPoint::Invia_Lettera_Q_Grande( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia comando Q
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x51;
	// Rilasco il comando Q
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x51;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

void PowerPoint::Invia_Lettera_Q_Piccolo( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[4];
	memset(input, 0, sizeof(INPUT)*4);
	// Mando SHIFT sinistro
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LSHIFT;
	// Invio Q
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x51;
	// Rilasco il comando Q
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x51;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio SHIFT sinistro
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = VK_LSHIFT;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4, input, sizeof(INPUT));
}
// R
void PowerPoint::Invia_Lettera_R_Grande( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia comando R
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x52;
	// Rilasco il comando R
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x52;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

void PowerPoint::Invia_Lettera_R_Piccolo( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[4];
	memset(input, 0, sizeof(INPUT)*4);
	// Mando SHIFT sinistro
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LSHIFT;
	// Invio R
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x52;
	// Rilasco il comando R
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x52;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio SHIFT sinistro
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = VK_LSHIFT;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4, input, sizeof(INPUT));
}
// S
void PowerPoint::Invia_Lettera_S_Grande( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia comando S
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x53;
	// Rilasco il comando S
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x53;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

void PowerPoint::Invia_Lettera_S_Piccolo( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[4];
	memset(input, 0, sizeof(INPUT)*4);
	// Mando SHIFT sinistro
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LSHIFT;
	// Invio S
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x53;
	// Rilasco il comando S
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x53;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio SHIFT sinistro
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = VK_LSHIFT;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4, input, sizeof(INPUT));
}
// T
void PowerPoint::Invia_Lettera_T_Grande( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia comando T
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x54;
	// Rilasco il comando T
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x54;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

void PowerPoint::Invia_Lettera_T_Piccolo( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[4];
	memset(input, 0, sizeof(INPUT)*4);
	// Mando SHIFT sinistro
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LSHIFT;
	// Invio T
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x54;
	// Rilasco il comando T
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x54;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio SHIFT sinistro
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = VK_LSHIFT;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4, input, sizeof(INPUT));
}
// U
void PowerPoint::Invia_Lettera_U_Grande( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia comando U
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x55;
	// Rilasco il comando U
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x55;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

void PowerPoint::Invia_Lettera_U_Piccolo( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[4];
	memset(input, 0, sizeof(INPUT)*4);
	// Mando SHIFT sinistro
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LSHIFT;
	// Invio U
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x55;
	// Rilasco il comando U
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x55;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio SHIFT sinistro
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = VK_LSHIFT;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4, input, sizeof(INPUT));
}
// V
void PowerPoint::Invia_Lettera_V_Grande( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia comando V
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x56;
	// Rilasco il comando V
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x56;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

void PowerPoint::Invia_Lettera_V_Piccolo( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[4];
	memset(input, 0, sizeof(INPUT)*4);
	// Mando SHIFT sinistro
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LSHIFT;
	// Invio V
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x56;
	// Rilasco il comando V
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x56;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio SHIFT sinistro
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = VK_LSHIFT;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4, input, sizeof(INPUT));
}
// W
void PowerPoint::Invia_Lettera_W_Grande( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia comando W
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x57;
	// Rilasco il comando W
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x57;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

void PowerPoint::Invia_Lettera_W_Piccolo( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[4];
	memset(input, 0, sizeof(INPUT)*4);
	// Mando SHIFT sinistro
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LSHIFT;
	// Invio W
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x57;
	// Rilasco il comando W
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x57;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio SHIFT sinistro
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = VK_LSHIFT;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4, input, sizeof(INPUT));
}
// X
void PowerPoint::Invia_Lettera_X_Grande( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia comando X
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x58;
	// Rilasco il comando X
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x58;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

void PowerPoint::Invia_Lettera_X_Piccolo( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[4];
	memset(input, 0, sizeof(INPUT)*4);
	// Mando SHIFT sinistro
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LSHIFT;
	// Invio X
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x58;
	// Rilasco il comando X
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x58;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio SHIFT sinistro
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = VK_LSHIFT;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4, input, sizeof(INPUT));
}
// Y
void PowerPoint::Invia_Lettera_Y_Grande( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia comando Y
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x59;
	// Rilasco il comando Y
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x59;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

void PowerPoint::Invia_Lettera_Y_Piccolo( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[4];
	memset(input, 0, sizeof(INPUT)*4);
	// Mando SHIFT sinistro
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LSHIFT;
	// Invio Y
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x59;
	// Rilasco il comando Y
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x59;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio SHIFT sinistro
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = VK_LSHIFT;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4, input, sizeof(INPUT));
}
// Z
void PowerPoint::Invia_Lettera_Z_Grande( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia comando Z
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x5A;
	// Rilasco il comando Z
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x5A;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

void PowerPoint::Invia_Lettera_Z_Piccolo( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[4];
	memset(input, 0, sizeof(INPUT)*4);
	// Mando SHIFT sinistro
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LSHIFT;
	// Invio Z
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x5A;
	// Rilasco il comando Z
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x5A;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio SHIFT sinistro
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = VK_LSHIFT;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4, input, sizeof(INPUT));
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////
// Da questo punto sono presenti solo i metodi per inviare i comandi
//////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//
void PowerPoint::Invia_PageUp( HWND Finestra)
{// Page Up
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia un shortcut keyboard di tipo VK_PRIOR
	// Questo messaggio equivale a Page Up
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_NEXT;
	// Rilasco il comando
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = VK_NEXT;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

void PowerPoint::Invia_PageDown( HWND Finestra )
{// Page Down
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia un shortcut keyboard di tipo VK_PRIOR
	// Questo messaggio equivale a Page Up
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_PRIOR;
	// Rilasco il comando
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = VK_PRIOR;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

void PowerPoint::Invia_MenuOpzioni ( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare CTRL
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare F
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x46;
	// Manda O
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x4F;
	// Rilasco O
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x4F;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio CNTRL
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x46;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

void PowerPoint::Invia_F5( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia un shortcut keyboard di tipo VK_PRIOR
	// Questo messaggio equivale a Page Up
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_F5;
	// Rilasco il comando
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = VK_F5;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

void PowerPoint::Invia_ChiudiPowerPoint( HWND Finestra )
{// CTRL + Q
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[4];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*4);
	// Primo msg da inviare CTRL
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LCONTROL;
	// Invia Q
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x51;
	// Rilasco Q
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x51;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia CONTROL
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = VK_LCONTROL;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(4, input, sizeof(INPUT));
}

void PowerPoint::Invia_SalvaSuFileCorrente ( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare CTRL
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare F
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x46;
	// Manda L
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x4C;
	// Rilasco L
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x4C;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio F
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x46;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

void PowerPoint::Invia_GUIDA ( HWND Finestra )
{// ALT + F + W
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[6];
	// Vai al menu Opzioni
	memset(input, 0, sizeof(INPUT)*6);
	// Primo msg da inviare CTRL
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LMENU;
	// Secondo msg da inviare F
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x46;
	// Manda W
	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x57;
	// Rilasco W
	input[3].type = INPUT_KEYBOARD;
	input[3].ki.wVk = 0x57;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascio F
	input[4].type = INPUT_KEYBOARD;
	input[4].ki.wVk = 0x46;
	input[4].ki.dwFlags = KEYEVENTF_KEYUP;
	// Rilascia shift sinistro
	input[5].type = INPUT_KEYBOARD;
	input[5].ki.wVk = VK_LMENU;
	input[5].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(6, input, sizeof(INPUT));
}

void PowerPoint::Invia_TAB( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia un shortcut keyboard di tipo VK_PRIOR
	// Questo messaggio equivale a Page Up
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_TAB;
	// Rilasco il comando
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = VK_TAB;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}

void PowerPoint::Invia_ESC( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// Invia un shortcut keyboard di tipo VK_PRIOR
	// Questo messaggio equivale a Page Up
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x1B;
	// Rilasco il comando
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x1B;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}
// Invia F1
void PowerPoint::Invia_F1( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// invia il comando F1
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x70;
	// Rilasco il comando
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x70;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}
// Invia F2
void PowerPoint::Invia_F2( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// invia il comando F2
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x71;
	// Rilasco il comando
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x71;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}
// Invia F3
void PowerPoint::Invia_F3( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// invia il comando F3
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x72;
	// Rilasco il comando
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x72;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}
// Invia F4
void PowerPoint::Invia_F4( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// invia il comando F4
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x73;
	// Rilasco il comando
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x73;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}
// Invia F6
void PowerPoint::Invia_F6( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// invia il comando F6
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x75;
	// Rilasco il comando
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x75;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}
// invia F7
void PowerPoint::Invia_F7( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// invia il comando F7
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x76;
	// Rilasco il comando
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x76;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}
// invia F8
void PowerPoint::Invia_F8( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// invia il comando F8
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x77;
	// Rilasco il comando
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x77;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}
//invia F9
void PowerPoint::Invia_F9( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// invia il comando F9
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x78;
	// Rilasco il comando
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x78;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}
//Invia F10
void PowerPoint::Invia_F10( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// invia il comando F10
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x79;
	// Rilasco il comando
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x79;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}
// Invia F11
void PowerPoint::Invia_F11( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// invia il comando F11
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x7A;
	// Rilasco il comando
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x7A;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}
// Invia F12
void PowerPoint::Invia_F12( HWND Finestra )
{
	if( Finestra == NULL )
		cout << "not found";
	//SetForegroundWindow(Finestra);
	INPUT input[2];
	memset(input, 0, sizeof(INPUT)*2);
	// invia il comando F12
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x7B;
	// Rilasco il comando
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x7B;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	// Invio il msg
	SendInput(2, input, sizeof(INPUT));
}