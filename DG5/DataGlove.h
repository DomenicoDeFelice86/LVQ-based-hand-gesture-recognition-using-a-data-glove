#pragma once

// La struttura Dataglove garantisce l'accesso a tutte le informazioni di cui si ha bisogno
// 
// Per accedere ai dati è possibile utilizzare un apposita classe che permette di
// leggere le informazioni fornite dal dataglove.
// f[i] indica lo stato del dito in percentuale ( % da 0.0 a 100.0 )
// Se si sta utilizzando un data glove DESTRO allora 
// f[0] rappresenta il POLLICE
// 
// f[4] rappresenta il MIGNOLO, viceversa se il glove è di tipo SINISTRA allora 
// f[0] rappresenta il mignolo e 
// f[4] il POLLICE.

// ROLL e PITCH riportano l'inclinazione della mano in GRADI, per esempio da -90 a +90,
// mentre  ax,ay,az, rappresentano le accelerazioni istantanee della mano, i cui valori sono
// utili per sviluppare soffisticati algoritmi per il riconoscimento del gesto.

// Gli altri valori rappresentano lo stato interno del glove e non dovrebbero essere manipolati.
#include "stdafx.h"
#include <afx.h>

class DataGlove
{
public:
	DataGlove();
	~DataGlove(void);

	CString label;
	int id;
	int comport;
	int type;
	
	// Valori minimo a massimo in raw 0..1024
	int   fmin[5],fmax[5];
	// finger 0.0 ... 100.0
	float f[5];
	// Angolo di rotazione -90.. +90
	float roll, pitch;
	// Accelerazione in g -2...+2;
	float ax, ay, az;

	bool NewDataAvailable ;
	bool Sampling ;
	bool PortOpened ;
	bool SelfCalibration;
	int counter_buffer;
	// Dimensione buffer dei finger
	int buffersize;
	int buff[2000];
	// Dimensione buffer delle accelerazioni
	int buffaccsize ; 
	int buffacc[2000];

	int ResetValue(void);
	int ElaborateValues(void);
	int AddPackage(unsigned char * buffer);
};
