//
// Header File: GestioneFile.h
#include "stdafx.h"
#define norma 0.58f

using namespace std;

class GestioneFile
{
private:
	float valore;
	// Parametri per le somme
	float somma_1,somma_2,somma_3;
	float somma_4,somma_5;
	// Parametri per le medie
	float media_1,media_2,media_3;
	float media_4,media_5;
	// Parametri per la normalizzazione
	float norm_1,norm_2,norm_3;
	float norm_4,norm_5;
	// Matrice
	float fMatrice[5][5];
	// Paramatri per il conteggio
	int contaA,contaB,contaC,contaD,contaE,contaF,contaG,contaH,
		contaI,contaL,contaM,contaN,contaO,contaP,contaQ,contaR,
		contaS,contaT,contaU,contaV,contaZ,contaJ,contaK,contaW,
		contaX,contaY,conta3,conta4,conta5;
	// Variabili per i contatori
	int contatore_A,contatore_B,contatore_C,contatore_F,
		contatore_L,contatore_P,contatore_U,contatore_V,
		contatore_W,contatore_Y;
	// 
	int totline;
	// Serve per i duplicati
	int cnt_duplicati,cnt_fine;
	// Variabili per il glove
	DataGlove *dg1;
	DataGlove *dg2;
	// Tipo guanto utilizzato(Sinistro/Destro)
	char tipo_guanto[2];
	// Il tipo gesto che viene chiesto in modalita' scansione gesti mutipli
	char tipo_gesto_multiplo[2];
public:
	void Calcola_Media(void);
	void Normalizzazione(void);
	int ShowValues(Project *proj,int gloveid1,int gloveid2);
	bool FileExists(string strFilename);
	void Conta_Scansioni(int no_output);
	void Salva_File_Ordinato(void);
	void Controllo(FILE *FileTraining,FILE *FileTest,char let,int num,int numerico);
	void Cerca_Doppioni(char nomefile[25]);
	void Elimina_Doppioni(char stringa[]);
	void Cerca_ValoreMinMax(char nomefile[25]);
	void Crea_File_Normalizzato(char nomefile[25]);
	void Stampa_Matrice();
	void MenuScelta();
	void Scarica_Info_DataGlove(Project *proj,int gloveid1,int gloveid2);
};
