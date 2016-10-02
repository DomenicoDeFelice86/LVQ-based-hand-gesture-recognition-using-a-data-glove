// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <tchar.h>
#include <stdio.h>
#include <conio.h>
#include <sys/stat.h> 
#include "targetver.h"
#include "DataGlove.h"
#include "Project.h"
#include "PowerPoint.h"
#include "GestioneFile.h"
#include "Riconosci_Gesti.h"
#include "DG5.h"
#include "Calcola_Scarto.h"
#include <malloc.h>

using namespace std;

bool SelfCalibration = true;

// TODO: reference additional headers your program requires here
#ifndef VC_EXTRALEAN
// Escludere dalle intestazioni Windows gli elementi utilizzati di rado.
#define VC_EXTRALEAN		
#endif

// Modificare le seguenti definizioni se è necessario creare una piattaforma prima di quelle specificate di seguito.
// Fare riferimento a MSDN per le ultime informazioni sui valori corrispondenti per le differenti piattaforme.
#ifndef WINVER				// È consentito l'utilizzo di funzionalità specifiche per Windows 95 e Windows NT 4 o versioni successive.
#define WINVER 0x0501		// Cambiare il valore immettendo quello corretto per Windows 98, Windows 2000 o versione successiva.
#endif

#ifndef _WIN32_WINNT		// È consentito l'utilizzo di funzionalità specifiche per Windows NT 4 o versioni successive.
#define _WIN32_WINNT 0x0501		// Cambiare il valore immettendo quello corretto per Windows 98, Windows 2000 o versione successiva.
#endif						

#ifndef _WIN32_WINDOWS		// È consentito l'utilizzo di funzionalità specifiche per Windows 98 o versioni successive.
#define _WIN32_WINDOWS 0x0501 // Cambiare il valore immettendo quello corretto per Windows Millennium Edition o versione successiva.
#endif

#ifndef _WIN32_IE			// È consentito l'utilizzo di funzionalità specifiche per IE 4.0 o versioni successive.
#define _WIN32_IE 0x0500	// Cambiare il valore immettendo quello corretto per Internet Explorer 5.0 o versione successiva.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// alcuni costruttori CString saranno espliciti.

// disattiva l'operazione delle classi MFC che consiste nel nascondere alcuni comuni messaggi di avviso che vengono spesso ignorati.
#define _AFX_ALL_WARNINGS

//#include <afxwin.h>         // componenti MFC di base e standard
//#include <afxext.h>         // estensioni MFC
//#include <afxdisp.h>        // classi di automazione MFC

//#include <afxdtctl.h>		// Supporto MFC per controlli comuni di Internet Explorer 4
#ifndef _AFX_NO_AFXCMN_SUPPORT
//#include <afxcmn.h>			// Supporto MFC per controlli comuni di Windows
#endif // _AFX_NO_AFXCMN_SUPPORT