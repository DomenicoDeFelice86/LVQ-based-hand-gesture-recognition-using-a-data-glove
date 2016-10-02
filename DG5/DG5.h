// File header DG5.h
// Stabilisco alcuni define che verranno utilizzati nell'applicazione
#define INVIO 13
#define CTRLX 24
#define ESC 27
#define SPAZIO 32

// Permette di pulire lo schermo
void Pulisci_Schermo(void);
void Pulisci_Seconda_Cornice();
// Crea una cornice con i parametri forniti in input
void Crea_Cornice(int X, int Y, int W, int H);
// Crea la lista opzioni da scegliere
void Crea_Lista_Opzioni(char *Frase, int X, int Y, int C);
// Permette di spostarsi in uno specifico punto
void Spostati_Nella_Posizione (int x, int y);

void LeggiStringa (char *Stringa, int LenMax);
void Maschera (char *Titolo);
void MascheraColori (void);
void MascheraMano (void);
void MascheraFeature (void);
int ScegliMenu (void);
void TextColor (unsigned short Colore);
void fai_qualcosa();
void Scarto(void);
void Addestramento();
void Rileva_Gesto(void);
void Demo_PowerPoint(void);
