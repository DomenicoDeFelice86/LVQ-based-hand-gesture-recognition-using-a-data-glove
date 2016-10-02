// Header file PowerPoint.h
#include "stdafx.h"
#define norma 0.58f
#define RIGHE 100
#define COL   6

class PowerPoint
{
private:
	// Variabili per la somma
	float  somma_1, somma_2, somma_3, somma_4, somma_5;
	float somma2_1,somma2_2,somma2_3,somma2_4,somma2_5;
	// Variabili per la media
	float  media_1, media_2, media_3, media_4, media_5;
	float media2_1,media2_2,media2_3,media2_4,media2_5;
	// Variabili per la normalizzazione
	float  norm_1, norm_2, norm_3, norm_4, norm_5;
	float norm2_1,norm2_2,norm2_3,norm2_4,norm2_5;
	float valore;
	int totline,totline2;
	int contatore,contatore2;
	int righe,righe2;
	int destro_non_connesso;
	int sinistro_non_connesso;

	float matrice_Glove[5][5];
	float matrice_Glove_2[5][5];

	int ju,ju2;
	int tipo_cnt,tipo_cnt2;
	int tipo_ni;
	float matrice_Centroidi[RIGHE][COL],matrice_Centroidi2[RIGHE][COL];
	
	float distanze[RIGHE],distanze2[RIGHE];

	int cntA,cntB,cntC,cntE,cntF,cntG,cntH,cntL,cntU,cntW,cntY,cnt4;
	int massimo_cnt;
	char lettera;

	char tipo_guanto[2];
	DataGlove *dg1;
	DataGlove *dg2;
	//In questo vettore salvo i valori letti dal guanto
	float dati_input[5],dati_input2[5];
	//
	char tipo_elm_gesto[35],tipo_elm_gesto2[35];
	// Contatore
	int  elo,elo2;
	int  righe_conta,righe_conta2;
	float vettore_corr[5],vettore_corr2[5];
	float vettore_pow[5],vettore_pow2[5];
	float val_min_global[5],val_min_global2[5];
	float vettore_min[5];
	float vettore_corr_pow[5],vettore_corr_pow2[5];
	float val1,  val2,  val3, val4,  val5;
	float val2_1,val2_2,val2_3,val2_4,val2_5;
	// Buffer dati
	char buf_dati[256],buf_dati2[256];
	char numf[256],numf2[256];
	int primavolta;
	char gesto,gesto2;
	int sinistro,destro;
public:
	// Costruttore
	PowerPoint();
	~PowerPoint();
	int  Avvia_power(Project *proj,int gloveid1,int gloveid2);
	char Trova_Gesto();
	void somma_distanze();
	void Inserisci_Elem_Matr(float elemento,char tipguanto[2]);
	void Inserisci_Tipo_Matr(float elemento,char tipguanto[2]);
	void Calcola_Media();
	void Normalizzazione();
	void Stampa_Matrice();
	void Invia_PageUp( HWND Finestra );
	void Cmd_PowerPoint(char t_gesto);
	void Invia_PageDown( HWND Finestra );
	void Invia_MenuOpzioni ( HWND Finestra );
	void Invia_ChiudiPowerPoint( HWND Finestra );
	void Invia_SalvaSuFileCorrente ( HWND Finestra );
	void Invia_GUIDA ( HWND Finestra );
	void Invia_TAB( HWND Finestra );
	void Invia_ESC( HWND Finestra );

	void Invia_F1( HWND Finestra );
	void Invia_F2( HWND Finestra );
	void Invia_F3( HWND Finestra );
	void Invia_F4( HWND Finestra );
	void Invia_F5( HWND Finestra );
	void Invia_F6( HWND Finestra );
	void Invia_F7( HWND Finestra );
	void Invia_F8( HWND Finestra );
	void Invia_F9( HWND Finestra );
	void Invia_F10( HWND Finestra );
	void Invia_F11( HWND Finestra );
	void Invia_F12( HWND Finestra );
	void ScaricaDatieSalvaSuMatrice(Project *proj,int gloveid1,int gloveid2);
	void Calcola_Distanza_Centroidi(void);

	int Riconosci_Gesti_Multipli(Project *proj,int gloveid1,int gloveid2);
	int Presentazione_A_Pieno_Schermo(Project *proj,int gloveid1,int gloveid2);
	void Trova_Massimo_gesti_Multipli();
	void Cmd_PowerPoint_Multiplo( );
	int Cmd_PowerPoint_Multiplo_Sinistro( );
	void Resetta_contatori();
	void Inizializzazione_Matrice(void);
	void Conta_Elementi();
	void Inizializzatore();
	void DomandaTipoGuanto(void);
	void DomandaNomeFilePowerPoint(void);
	// Metodi per lo schema diapositiva
	void Inserisci_Schema_diapositiva(HWND Finestra);
	void Chiudi_Schema_diapositiva(HWND Finestra);
	void Inserisci_Layout(HWND Finestra);
	void Elimina_Diapositiva(HWND Finestra);
	void Rinomina_Diapositiva(HWND Finestra);
	void Inserisci_Segnaposto(HWND Finestra);
	void Modifica_tema(HWND Finestra);
	void Stili_sfondo(HWND Finestra);
	void Imposta_Pagina_schema_diapositiva(HWND Finestra);
	void Orientamento_schema_Diapositiva(HWND Finestra);
	void Titolo_Diapositiva(HWND Finestra);
	void Pie_di_pagina(HWND Finestra);
	void Nascondi_grafico_di_sfondo(HWND Finestra);
	void Cambia_Colore(HWND Finestra);
	void Tipi_Di_carattere_diapositiva(HWND Finestra);
	void Effetti_diapositiva(HWND Finestra);
	// Metodi per la scherma note
	void Intestazione_Note(HWND Finestra);
	void Immagine_Diapositiva(HWND Finestra);
	void Pie_di_pagina_Note(HWND Finestra);
	void Data_Note(HWND Finestra);
	void Corpo_note(HWND Finestra);
	void Numero_di_pagina(HWND Finestra);
	void Orientamento_pagina_note(HWND Finestra);
	void Orientamento_diapositiva_note(HWND Finestra);
	void Temi_scherma_note(HWND Finestra);
	void Chiudi_Visualizzazione_Scherma_Note(HWND Finestra);
	void Imposta_Pagina_Scherma_Note(HWND Finestra);
	void Colore_Scherma_Note(HWND Finestra);
	void Carattere_Scherma_Note(HWND Finestra);
	void Effetti_Scherma_note(HWND Finestra);
	void Sfondo_Colore_Scherma_Note(HWND Finestra);
	// Scherma Stampanti
	void Chiudi_visualizzazione_Schema_Stampanti(HWND Finestra);
	void Imposta_pagina_Schema_stampanti(HWND Finestra);
	void Orientamento_Schema_stampanti(HWND Finestra);
	void Orientamento_Diapositiva_Schema_stampanti(HWND Finestra);
	void Diapositive_per_pagina_Schema_stampanti(HWND Finestra);
	void Temi_schema_stampanti(HWND Finestra);
	void Colori_Schema_stampanti(HWND Finestra);
	void Stili_Sfondo_Schema_stampanti(HWND Finestra);
	void Data_Schema_Stampanti(HWND Finestra);
	void Intestazionne_Schema_Stampanti(HWND Finestra);
	void Pie_di_pagina_schema_Stampanti(HWND Finestra);
	void Numero_di_pagina_scherma_stampanti(HWND Finestra);
	void Colori_Schema_Stampanti(HWND Finestra);
	void Effetti_Scherma_Stampanti(HWND Finestra);
	void Tipi_Di_carattere_stampanti(HWND Finestra);
	//
	void PassaAdAltraFinestra(HWND Finestra);
	void Invia_Grassetto   (HWND Finestra);
	void Invia_Corsivo     (HWND Finestra);
	void Invia_SottoLineato(HWND Finestra);
	void Invia_ControlloOrtografico(HWND Finestra);
	void Invia_Ricerche(HWND Finestra);
	void Invia_ZOOM(HWND Finestra);
	void Visualizzazione_Normale(HWND Finestra);
	void Sequenza_diapositive(HWND Finestra);
	void Pagina_note(HWND Finestra);
	void Visualizzazione_di_lettura(HWND Finestra);
	void Scherma_diapositiva(HWND Finestra);
	void Scherma_stampati(HWND Finestra);
	void Note(HWND Finestra);
	void Righello(HWND Finestra);
	void Griglia(HWND Finestra);
	void Guide(HWND Finestra);
	void TipoTransizioniSlides(HWND Finestra);
	void Stili_Sfondo(HWND Finestra);
	void TipiDiCarattere(HWND Finestra);
	void ScegliColori(HWND Finestra);
	void ScegliEffetti(HWND Finestra);
	void Scelta_Tema(HWND Finestra);
	void Orientamento_Diapositiva(HWND Finestra);
	void Imposta_Pagina(HWND Finestra);
	void FrecciaSu( HWND Finestra);
	void FrecciaGiu( HWND Finestra);
	void FrecciaDestra( HWND Finestra);
	void FrecciaSinistra( HWND Finestra);

	void Nuova_Diapositiva(HWND Finestra);
	void Elenchi_Puntati(HWND Finestra);
	void Elenchi_Numerati(HWND Finestra);
	void CambiaTipiDiCarattere(HWND Finestra);
	void Incrementa_dimcaratt(HWND Finestra);
	void Decrementa_dimcaratt(HWND Finestra);
	void Cambia_Dimensione_Carattere(HWND Finestra);
	void Inserisci_Immagine(HWND Finestra);
	void Crea_Tabella(HWND Finestra);
	void Inserisci_Grafico(HWND Finestra);
	void Intestazione_pie_Pagina(HWND Finestra);
	void Inserisci_Forme(HWND Finestra);
	void Trova_Nel_Testo(HWND Finestra);
	void Sostituisci(HWND Finestra);
	//
	void Invia_Spazio( HWND Finestra );
	void Invia_Return( HWND Finestra );
	void Invia_Cancella( HWND Finestra );
	void Invia_Virgola( HWND Finestra );
	void Invia_Punto( HWND Finestra );
	void Invia_PuntoEscalamativo( HWND Finestra );
	void Invia_PuntoInterrogativo( HWND Finestra );
	// Carattere accentati
	void Invia_BLOCNUM( HWND Finestra );
	void a_accentata( HWND Finestra );
	void o_accentata( HWND Finestra );
	void e_accentata( HWND Finestra );
	void u_accentata( HWND Finestra );
	// A
	void Invia_Lettera_A_Grande ( HWND Finestra );
	void Invia_Lettera_A_Piccolo( HWND Finestra );
	// B
	void Invia_Lettera_B_Grande ( HWND Finestra );
	void Invia_Lettera_B_Piccolo( HWND Finestra );
	// C
	void Invia_Lettera_C_Grande ( HWND Finestra );
	void Invia_Lettera_C_Piccolo( HWND Finestra );
	// D
	void Invia_Lettera_D_Grande ( HWND Finestra );
	void Invia_Lettera_D_Piccolo( HWND Finestra );
	// E
	void Invia_Lettera_E_Grande ( HWND Finestra );
	void Invia_Lettera_E_Piccolo( HWND Finestra );
	// F
	void Invia_Lettera_F_Grande ( HWND Finestra );
	void Invia_Lettera_F_Piccolo( HWND Finestra );
	// G
	void Invia_Lettera_G_Grande ( HWND Finestra );
	void Invia_Lettera_G_Piccolo( HWND Finestra );
	// H
	void Invia_Lettera_H_Grande ( HWND Finestra );
	void Invia_Lettera_H_Piccolo( HWND Finestra );
	// I
	void Invia_Lettera_I_Grande ( HWND Finestra );
	void Invia_Lettera_I_Piccolo( HWND Finestra );
	// J
	void Invia_Lettera_J_Grande ( HWND Finestra );
	void Invia_Lettera_J_Piccolo( HWND Finestra );
	// K
	void Invia_Lettera_K_Grande ( HWND Finestra );
	void Invia_Lettera_K_Piccolo( HWND Finestra );
	// L
	void Invia_Lettera_L_Grande ( HWND Finestra );
	void Invia_Lettera_L_Piccolo( HWND Finestra );
	// M
	void Invia_Lettera_M_Grande ( HWND Finestra );
	void Invia_Lettera_M_Piccolo( HWND Finestra );
	// N
	void Invia_Lettera_N_Grande ( HWND Finestra );
	void Invia_Lettera_N_Piccolo( HWND Finestra );
	// O
	void Invia_Lettera_O_Grande ( HWND Finestra );
	void Invia_Lettera_O_Piccolo( HWND Finestra );
	// P
	void Invia_Lettera_P_Grande ( HWND Finestra );
	void Invia_Lettera_P_Piccolo( HWND Finestra );
	// Q
	void Invia_Lettera_Q_Grande ( HWND Finestra );
	void Invia_Lettera_Q_Piccolo( HWND Finestra );
	// R
	void Invia_Lettera_R_Grande ( HWND Finestra );
	void Invia_Lettera_R_Piccolo( HWND Finestra );
	// S
	void Invia_Lettera_S_Grande ( HWND Finestra );
	void Invia_Lettera_S_Piccolo( HWND Finestra );
	// T
	void Invia_Lettera_T_Grande ( HWND Finestra );
	void Invia_Lettera_T_Piccolo( HWND Finestra );
	// U
	void Invia_Lettera_U_Grande ( HWND Finestra );
	void Invia_Lettera_U_Piccolo( HWND Finestra );
	// V
	void Invia_Lettera_V_Grande ( HWND Finestra );
	void Invia_Lettera_V_Piccolo( HWND Finestra );
	// W
	void Invia_Lettera_W_Grande ( HWND Finestra );
	void Invia_Lettera_W_Piccolo( HWND Finestra );
	// X
	void Invia_Lettera_X_Grande ( HWND Finestra );
	void Invia_Lettera_X_Piccolo( HWND Finestra );
	// Y
	void Invia_Lettera_Y_Grande ( HWND Finestra );
	void Invia_Lettera_Y_Piccolo( HWND Finestra );
	// Z
	void Invia_Lettera_Z_Grande ( HWND Finestra );
	void Invia_Lettera_Z_Piccolo( HWND Finestra );
};


