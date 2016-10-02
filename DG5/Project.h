#pragma once
// La classe Project permette di ottenere l'accesso e l'immagazzinamento dei differenti
// data glove.
// Le funzioni disponibili nella classe possono essere divise in 3 sezioni:
// -- Global Project manipulation
// -- Data Glove Related functions
// -- Sampling Functions
#include "stdafx.h"

class Project
{
public:
	Project(void);
	~Project(void);
	
	int AddDataGlove(const char *label, int comport, int type);
	bool RemoveGlove(int gloveid);
	DataGlove *GetDataGlove(int id);
	int SetGloveBufferSize(int size_finger, int size_acc, int gloveid);
	int SaveProject(void);
	int LoadProject(void);
	int GetNumDataGlove(void);
	void CloseProject(void);
	
	int StartSampling(int gloveid);
	int StopSampling(int gloveid);
};