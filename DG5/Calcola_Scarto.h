#include "stdafx.h"


class Calcola_Scarto
{
private:
	// Variabili per la somma
	float somma_1,somma_2,somma_3;
	float somma_4,somma_5;
	// Variabili per la media
	float media_1,media_2,media_3;
	float media_4,media_5;
	// Variabili per la normalizzazione
	float norm_1,norm_2,norm_3;
	float norm_4,norm_5;
	// Valore che dobbiamo sottrare durante la normalizzazione
	float valore;
	// Tipo di guanto utilizzato (Sinistro/Destro)
	char tipo_guanto[2];
	// Variabile per il data glove destro
	DataGlove *dg1;
	// Variabile per il data glove sinistro
	DataGlove *dg2;
	// Variabili per rilevare il tempo impiegato a rilevare un gesto
	clock_t start, finish;
	// Matrice dati per i parametri scaricati dal glove
	float fMatrice[5][5];
	//
	int righ,primo_avvio;
	//
	int contatore;
	//
	int totline,totline2;
	// Matrice dati generale dove vado a salvare i dati dei centroidi
	float matrice_Centroidi[35][COL];
	float matrice_Test[MAXRIGHE][COL];
	float matrice_Centroidi_pow[MAXRIGHE][COL];
	float matrice_Text_pow[MAXRIGHE][COL];
	float matrice_DistanzeEuclidee[MAXRIGHE][COL];

	// matrice contenente le info riguardo la distanza euclidea calcolata e il gesto
	// associato ad essa
	float matrice_Finale[MAXRIGHE][2];

	// Questo vettore contiene il tipo di gesto associato ad ogni rigo
	// Viene utilizzato per stabilire quale gesto e' associato ad un
	// numero. Quindi, ad esempio, numero 10 gesto C.
	char  tipo_elemento[35];
	char tipo_elm2[35];
	// Questo vettore viene utilizzato per salvare il valore numerico della
	// distanza euclidea tra il centroide e i valori forniti dal glove
	float distanze[35];
	// Questo vettore viene utilizzato per salvare i dati appena letti dal
	// glove
	float prova[5];
	// Contatori
	int CNT1,CNT2,CNT3,CNT4,CNT5,CNT6;
	int tipo_cnt;
	int tipo_ni;
	// Buffer dati
	char buf_dati[256];
	char numf[256];
	// il tipo di gesto associato al valore minimo
	char tipo_scelta[2];
	// Variabile per il file
	FILE *FileGlove;
	//
	int primavolta;
	// Variabili per ogni dito della mano
	// Vengono inizializzate con un valore minimo
	float val1,val2,val3,val4,val5;
	// Vettore da elavare al quadrato
	float vettore_pow[5];
	// Vettore che contiene il valore minimo globale
	float val_min_global[5];
	float vettore_min[5];
	// Vettore corrente 
	float vettore_corr[5];
	// Vettore correte elevato al quadrato
	float vettore_corr_pow[5];
	
	int cont_righe;
	int cont_colonne;

public:
	// Costruttore
	Calcola_Scarto();
	~Calcola_Scarto();
	// Definizione Metodi
	int Riconoscimento(Project *proj,int gloveid1,int gloveid2);
	void Conta_Elementi(char nomefile[25],int tipologia);

	void Inserisci_Elem_Matr(float mat[MAXRIGHE][COL],float elemento);
	void Inserisci_Tipo_Matr(float mat[MAXRIGHE][COL],float elemento);

	void somma_distanze(float mat[MAXRIGHE][COL]);
	char Trova_Gesto();
	void Calcola_Media();
	void Normalizzazione();
	void Inizializzazione_Vettore(float vet[MAXRIGHE]);

	void Inizializzazione_Matrice(float mat[MAXRIGHE][COL]);
	void Inizializzatore();
	void Stampa_Matrice(float mat[MAXRIGHE][COL]);
	void ChiediTipoDiGuato();

	void pow_Centroidi();
	void pow_Test();

	void Inserisci_Valore_Matrice(float valore);

	void Scarto();

	void Genera_Matrice_Test();
	void Genera_Matrice_Centroidi();
	void Inserisci_Elm_Finale(float distanza);
};

