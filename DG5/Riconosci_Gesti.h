// Classe "Riconosci_Gesti"
// -- Riconosci_gest.h --
//
// Questa classe definisce i metodi e le variabili per riconoscere un gesto.
// 
#include "stdafx.h"
#include "cv.h"
#include "opencv/cxcore.h"
#include "opencv2/highgui/highgui.hpp"
// Definisce il parametro per la normalizzazione
#define norma 0.58f
// Definisce le righe per la matrice che nel nostro caso corrisponde al numero 
// di centroidi, fisso il numero massimo di centroidi possibili a 100
#define MAXRIGHE 10000
// Definisce le colonne per la matrice
#define COL   6

class Riconosci_Gesti
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
	int righ,primo_avvio;
	int contatore;
	int totline;
	// Matrice dati generale dove vado a salvare i dati dei centroidi
	float mat[MAXRIGHE][COL];
	// Questo vettore contiene il tipo di gesto associato ad ogni rigo
	// Viene utilizzato per stabilire quale gesto e' associato ad un
	// numero. Quindi, ad esempio, numero 10 gesto C.
	char  tipo_elm[MAXRIGHE];
	// Questo vettore viene utilizzato per salvare il valore numerico della
	// distanza euclidea tra il centroide e i valori forniti dal glove
	float distanze[MAXRIGHE];
	// Questo vettore viene utilizzato per salvare i dati appena letti dal
	// glove
	float prova[5];
	// Contatori
	int CNT1,CNT2,CNT3;
	int tipo_cnt;
	int tipo_ni;
	// Buffer dati
	char buf_dati[256];
	char numf[256];
	// il tipo di gesto associato al valore minimo
	char tipo_scelta;
	// Variabile per il file
	FILE *FileGlove;
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
public:
	// Costruttore
	Riconosci_Gesti();
	~Riconosci_Gesti();
	// Definizione Metodi
	int Riconoscimento(Project *proj,int gloveid1,int gloveid2);
	void Conta_Elementi(char nomefile[25]);
	void Inserisci_Elem_Matr(float elemento);
	void Inserisci_Tipo_Matr(float elemento);
	void somma_distanze();
	void Trova_Gesto();
	void Calcola_Media();
	void Normalizzazione();
	void Inizializzazione_Matrice();
	void Inizializzatore();
	void Stampa_Matrice();
	void ChiediTipoDiGuato();
};