#include <windows.h>
#include <stdlib.h>
#include <cctype>
#include <stdio.h>
#define FILE_MENU_ESCI 1
#define GENERA 2
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
void AddMenus(HWND);
void AddControls(HWND);
char fiscale[16];
char provvisorio[3];
int errore = 0;
void loadImages();
void programma(char cognome[], char nome[], char anno[], char mesecontrollo[], char giorno[], char sex[], char nazione[]);
void estrazione(char nome[], char* cons, char* voc);
void primecifre(int a, int b, char checkvoc[], char checkcons[]);
HWND hCognome, hNome, hAnno, hMese, hGiorno, hSesso, hNazione, hOut, hLogo;
HMENU hMenu;
HBITMAP hLogoImage;
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
	WNDCLASSW wc = {0};
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = hInst;
	wc.lpszClassName = L"myWindowClass";
	wc.lpfnWndProc = WindowProcedure;
	if (!RegisterClassW(&wc)) {
		return -1;
	}
	CreateWindowW(L"myWindowClass", L"Calcola Codice Fiscale", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500, NULL, NULL, NULL, NULL);
	MSG msg = {0};
	while( GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch ( msg) {
		case WM_COMMAND:
			switch(wp) {
				case FILE_MENU_ESCI:
					DestroyWindow(hWnd);
					break;
				case GENERA:
					errore = 0;
					char cognomespecial[100], nomespecial[100], annospecial[5], mesespecial[3], giornospecial[3], sessospecial[2], nazionespecial[100];
					GetWindowText(hCognome, cognomespecial, 100);
					GetWindowText(hNome, nomespecial, 100);
					GetWindowText(hAnno, annospecial, 5);
					GetWindowText(hMese, mesespecial, 3);
					GetWindowText(hGiorno, giornospecial, 3);
					GetWindowText(hSesso, sessospecial, 2);
					GetWindowText(hNazione, nazionespecial, 100);
					programma(cognomespecial, nomespecial, annospecial, mesespecial, giornospecial, sessospecial, nazionespecial);
					if (errore == 0) {
						SetWindowText(hOut, fiscale);
					}
					break;
			}
			break;
		case WM_CREATE:
			ShowWindow (GetConsoleWindow(), SW_HIDE);
			loadImages();
			AddMenus(hWnd);
			AddControls(hWnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProcW(hWnd, msg, wp, lp);
	}
}
void AddMenus(HWND hWnd) {
	hMenu = CreateMenu();
	HMENU hFileMenu = CreateMenu();
	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_ESCI, "Esci");
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, "File");
	SetMenu(hWnd, hMenu);
}
void AddControls(HWND hWnd) {
	CreateWindowW(L"static", L"Calcolo Codice Fiscale (Creato da MistMayMarchpeppe2):", WS_VISIBLE | WS_CHILD | SS_CENTER, 10, 10, 480, 28, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Cognome :", WS_VISIBLE | WS_CHILD | SS_CENTER, 10, 55, 180, 28, hWnd, NULL, NULL, NULL);
	hCognome = CreateWindowW(L"edit", L"Inserire Cognome", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 200, 50, 230, 28, hWnd, NULL, NULL, NULL);
    CreateWindowW(L"static", L"Nome :", WS_VISIBLE | WS_CHILD | SS_CENTER, 10, 95, 180, 28, hWnd, NULL, NULL, NULL);
	hNome = CreateWindowW(L"edit", L"Inserire Nome", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 200, 90, 230, 28, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Anno di nascita :", WS_VISIBLE | WS_CHILD | SS_CENTER, 10, 135, 180, 28, hWnd, NULL, NULL, NULL);
	hAnno = CreateWindowW(L"edit", L"Anno a 4 cifre (ex: 1994)", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 200, 130, 230, 28, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Mese di nascita :", WS_VISIBLE | WS_CHILD | SS_CENTER, 10, 175, 180, 28, hWnd, NULL, NULL, NULL);
	hMese = CreateWindowW(L"edit", L"Inserire numero da 1 a 12", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 200, 170, 230, 28, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Giorno di nascita :", WS_VISIBLE | WS_CHILD | SS_CENTER, 10, 215, 180, 28, hWnd, NULL, NULL, NULL);
	hGiorno = CreateWindowW(L"edit", L"Inserire numero da 1 a 31", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 200, 210, 230, 28, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Sesso :", WS_VISIBLE | WS_CHILD | SS_CENTER, 10, 255, 180, 28, hWnd, NULL, NULL, NULL);
	hSesso = CreateWindowW(L"edit", L"Inserire una lettera (M o F)", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 200, 250, 230, 28, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Citta'di Nascita :", WS_VISIBLE | WS_CHILD | SS_CENTER, 10, 295, 180, 28, hWnd, NULL, NULL, NULL);
	hNazione = CreateWindowW(L"edit", L"Inserire Citta' di nascita", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 200, 290, 230, 28, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"button", L"Genera", WS_VISIBLE | WS_CHILD, 306, 330, 98, 38, hWnd, (HMENU)GENERA, NULL, NULL);
	hOut = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 240, 380, 230, 28, hWnd, NULL, NULL, NULL);
	hLogo = CreateWindowW(L"static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, 10, 330, 200, 100, hWnd, NULL, NULL, NULL);
	SendMessageW(hLogo, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM) hLogoImage);
}
void programma(char cognome[], char nome[], char anno[], char mesecontrollo[], char giorno[], char sex[], char nazione[]) {
	int lenght;
	int lenght1;
	char consonanti1[100];
    char vocali1[100];
    char consonanti2[100];
    char vocali2[100];
    int mese = 0;
    char sesso;
    char controllo[100];
    char controllo2[100];
    int q = 0;
    int x = 0;
    FILE * fp;
    if ((fp = fopen("filecodicefiscale", "r")) == NULL) {
    	MessageBox(NULL, "Il file filecodicefiscale non e' stato trovato nella stessa cartella dell eseguibile. Riavviare di nuovo l eseguibile in presenza del file", "Errore", MB_OK);
		errore = 1;
	}
	estrazione(cognome, consonanti1, vocali1);
	estrazione(nome, consonanti2, vocali2);
	lenght = strlen(consonanti1);
	lenght1 = strlen(vocali1);
	primecifre(lenght, lenght1, vocali1, consonanti1);
	strcpy(fiscale, provvisorio);
	lenght = strlen(consonanti2);
	lenght1 = strlen(vocali2);
	if (lenght > 3) {
		fiscale[3] = consonanti2[0];
	    fiscale[4] = consonanti2[2];
	    fiscale[5] = consonanti2[3];
	}
	else {
		primecifre(lenght, lenght1, vocali2, consonanti2);
	    fiscale[3] = provvisorio[0];
	    fiscale[4] = provvisorio[1];
	    fiscale[5] = provvisorio[2];
	}
		anno[strcspn(anno, "\n")] = '\0';
		lenght = strlen(anno);
		for (mese = 0; mese < lenght; mese++) {
			if (isdigit(anno[mese]) == 0) {
				MessageBox(NULL, "Inserire come Anno un numero di 4 cifre", "Errore", MB_OK);
			    errore = 1;
			}
		}
		if (lenght != 4) {
			MessageBox(NULL, "Inserire come Anno un numero di 4 cifre", "Errore", MB_OK);
			errore = 1;
		}
		else {
			fiscale[6] = anno[2];
			fiscale[7] = anno[3];
		}
		mese = atoi(mesecontrollo);
		switch(mese) {
			case 1 :
				fiscale[8] = 'A';
				break;
			case 2 :
			    fiscale[8] = 'B';
			    break;
		    case 3 :
			    fiscale[8] = 'C';
			    break;
		    case 4 :
		        fiscale[8] = 'D';
			    break;
		    case 5 :
		        fiscale[8] = 'E';
			    break;
		    case 6 :
		        fiscale[8] = 'H';
			    break;
		    case 7 :
		        fiscale[8] = 'L';
			    break;
		    case 8 :
		        fiscale[8] = 'M';
			    break;
		    case 9 :
		        fiscale[8] = 'P';
			    break;
		    case 10 :
		        fiscale[8] = 'R';
			    break;
		    case 11 :
		        fiscale[8] = 'S';
			    break;
		    case 12 :
		        fiscale[8] = 'T';
			    break;
		    default :
		    	MessageBox(NULL, "Inserire un numero del Mese valido da 1 a 12", "Errore", MB_OK);
				errore = 1;
		}
		mese = atoi(giorno);
		if (mese < 1 || mese > 31) {
			MessageBox(NULL, "Inserire un giorno da 1 a 31", "Errore", MB_OK);
			errore = 1;
		}
		sesso = sex[0];
		sesso = toupper(sesso);
		switch(sesso) {
			case 'M' :
				break;
			case 'F' :
				mese = mese + 40;
				break;
			default :
				MessageBox(NULL, "Inserire M o F come Sesso", "Errore", MB_OK);
				errore = 1;
		}
	if (mese < 10) {
		sprintf(anno, "0%d", mese);
	}
	else {
		sprintf(anno, "%d", mese);
	}
	fiscale[9] = anno[0];
	fiscale[10] = anno[1];
	lenght1 = 0;
	nazione[strcspn(nazione, "\n")] = '\0';
	for (lenght1 = 0; nazione[lenght1] != '\0'; lenght1++) {
		nazione[lenght1] = toupper(nazione[lenght1]);
	}
	for (q = 0; nazione[q] != '\0'; q++) {
		if (nazione[q] == ' ') {
			nazione[q] = nazione[q + 1];
			for (x = q + 1; nazione[x] != '\0'; x++) {
				nazione[x] = nazione[x + 1];
			}
		}
	}
	nazione[strcspn(nazione, "\n")] = '\0';
	fp = fopen("filecodicefiscale", "r");
	while (fscanf(fp,"%s    %s", &controllo, &controllo2) != EOF) {
		if (strcmp(controllo2, nazione) == 0) {
			strcpy(nazione, controllo);
			lenght1 = 0;
		}
	}
	fclose(fp); 
	if (lenght1 != 0) {
		MessageBox(NULL, "Citta' o Stato non trovato.", "Errore", MB_OK);
		errore = 1;
	}
	fiscale[11] = nazione[0];
	fiscale[12] = nazione[1];
	fiscale[13] = nazione[2];
	fiscale[14] = nazione[3];
	fiscale[15] = '\0';
	fiscale[16] = '\0';
	fiscale[17] = '\0';
	lenght = 0;
	lenght1 = 0;
	for (lenght = 0; fiscale[lenght] != '\0'; lenght = lenght + 2) {
		consonanti1[lenght1] = fiscale[lenght];
		lenght1++;
	}
	consonanti1[lenght1] = '\0';
	lenght = 0;
	lenght1 = 0;
	for (lenght = 1; fiscale[lenght] != '\0'; lenght = lenght + 2) {
		consonanti2[lenght1] = fiscale[lenght];
		lenght1++;
	}
	consonanti2[lenght1] = '\0';
	lenght = 0;
	lenght1 = 0;
	for (lenght = 0; consonanti1[lenght] != '\0'; lenght++) {
		switch(consonanti1[lenght]) {
			case '0' :
				lenght1 = lenght1 + 1;
				break;
			case '1' :
				lenght1 = lenght1 + 0;
				break;
			case '2' :
				lenght1 = lenght1 + 5;
				break;
			case '3' :
				lenght1 = lenght1 + 7;
				break;
			case '4' :
			    lenght1 = lenght1 + 9;
				break;
			case '5' :
				lenght1 = lenght1 + 13;
				break;
			case '6' :
				lenght1 = lenght1 + 15;
				break;
			case '7' :
				lenght1 = lenght1 + 17;
				break;
			case '8' :
				lenght1 = lenght1 + 19;
				break;
			case '9' :
				lenght1 = lenght1 + 21;
				break;
			case 'A' :
				lenght1 = lenght1 + 1;
				break;
			case 'B' :
				lenght1 = lenght1 + 0;
				break;
			case 'C' :
				lenght1 = lenght1 + 5;
				break;
			case 'D' :
				lenght1 = lenght1 + 7;
				break;
			case 'E' :
				lenght1 = lenght1 + 9;
				break;
			case 'F' :
				lenght1 = lenght1 + 13;
				break;
			case 'G' :
				lenght1 = lenght1 + 15;
				break;
			case 'H' :
				lenght1 = lenght1 + 17;
				break;
			case 'I' :
				lenght1 = lenght1 + 19;
				break;
			case 'J' :
				lenght1 = lenght1 + 21;
				break;
			case 'K' :
				lenght1 = lenght1 + 2;
				break;
			case 'L' :
				lenght1 = lenght1 + 4;
				break;
			case 'M' :
				lenght1 = lenght1 + 18;
				break;
			case 'N' :
				lenght1 = lenght1 + 20;
				break;
			case 'O' :
				lenght1 = lenght1 + 11;
				break;
			case 'P' :
				lenght1 = lenght1 + 3;
				break;
			case 'Q' :
				lenght1 = lenght1 + 6;
				break;
			case 'R' :
				lenght1 = lenght1 + 8;
				break;
			case 'S' :
				lenght1 = lenght1 + 12;
				break;
			case 'T' :
				lenght1 = lenght1 + 14;
				break;
			case 'U' :
				lenght1 = lenght1 + 16;
				break;
			case 'V' :
				lenght1 = lenght1 + 10;
				break;
			case 'W' :
				lenght1 = lenght1 + 22;
				break;
			case 'X' :
				lenght1 = lenght1 + 25;
				break;
			case 'Y' :
				lenght1 = lenght1 + 24;
				break;
			case 'Z' :
				lenght1 = lenght1 + 23;
				break;
			default :
				MessageBox(NULL, "Si prega di controllare i dati.", "Errore", MB_OK);
		        errore = 1;
		}
	}
	lenght = 0;
	for (lenght = 0; consonanti2[lenght] != '\0'; lenght++) {
		switch(consonanti2[lenght]) {
			case '0' :
				lenght1 = lenght1 + 0;
				break;
			case '1' :
				lenght1 = lenght1 + 1;
				break;
			case '2' :
				lenght1 = lenght1 + 2;
				break;
			case '3' :
				lenght1 = lenght1 + 3;
				break;
			case '4' :
			    lenght1 = lenght1 + 4;
				break;
			case '5' :
				lenght1 = lenght1 + 5;
				break;
			case '6' :
				lenght1 = lenght1 + 6;
				break;
			case '7' :
				lenght1 = lenght1 + 7;
				break;
			case '8' :
				lenght1 = lenght1 + 8;
				break;
			case '9' :
				lenght1 = lenght1 + 9;
				break;
			case 'A' :
				lenght1 = lenght1 + 0;
				break;
			case 'B' :
				lenght1 = lenght1 + 1;
				break;
			case 'C' :
				lenght1 = lenght1 + 2;
				break;
			case 'D' :
				lenght1 = lenght1 + 3;
				break;
			case 'E' :
				lenght1 = lenght1 + 4;
				break;
			case 'F' :
				lenght1 = lenght1 + 5;
				break;
			case 'G' :
				lenght1 = lenght1 + 6;
				break;
			case 'H' :
				lenght1 = lenght1 + 7;
				break;
			case 'I' :
				lenght1 = lenght1 + 8;
				break;
			case 'J' :
				lenght1 = lenght1 + 9;
				break;
			case 'K' :
				lenght1 = lenght1 + 10;
				break;
			case 'L' :
				lenght1 = lenght1 + 11;
				break;
			case 'M' :
				lenght1 = lenght1 + 12;
				break;
			case 'N' :
				lenght1 = lenght1 + 13;
				break;
			case 'O' :
				lenght1 = lenght1 + 14;
				break;
			case 'P' :
				lenght1 = lenght1 + 15;
				break;
			case 'Q' :
				lenght1 = lenght1 + 16;
				break;
			case 'R' :
				lenght1 = lenght1 + 17;
				break;
			case 'S' :
				lenght1 = lenght1 + 18;
				break;
			case 'T' :
				lenght1 = lenght1 + 19;
				break;
			case 'U' :
				lenght1 = lenght1 + 20;
				break;
			case 'V' :
				lenght1 = lenght1 + 21;
				break;
			case 'W' :
				lenght1 = lenght1 + 22;
				break;
			case 'X' :
				lenght1 = lenght1 + 23;
				break;
			case 'Y' :
				lenght1 = lenght1 + 24;
				break;
			case 'Z' :
				lenght1 = lenght1 + 25;
				break;
			default :
				MessageBox(NULL, "Si prega di controllare i dati.", "Errore", MB_OK);
		        errore = 1;
		}
	}
	fiscale[15] = 'A';
	fiscale[16] = '\0';
	switch(lenght1 % 26) {
		case 0 :
			sesso = 'A';
			break;
		case 1 :
			sesso = 'B';
			break;
		case 2 :
			sesso = 'C';
			break;
		case 3 :
			sesso = 'D';
			break;
		case 4 :
			sesso = 'E';
			break;
		case 5 :
			sesso = 'F';
			break;
		case 6 :
			sesso = 'G';
			break;
		case 7 :
			sesso = 'H';
			break;
		case 8 :
			sesso = 'I';
			break;
		case 9 :
			sesso = 'J';
			break;
		case 10 :
			sesso = 'K';
			break;
		case 11 :
			sesso = 'L';
			break;
		case 12 :
			sesso = 'M';
			break;
		case 13 :
			sesso = 'N';
			break;
		case 14 :
			sesso = 'O';
			break;
		case 15 :
			sesso = 'P';
			break;
		case 16 :
			sesso = 'Q';
			break;
		case 17 :
			sesso = 'R';
			break;
		case 18 :
			sesso = 'S';
			break;
		case 19 :
			sesso = 'T';
			break;
		case 20 :
			sesso = 'U';
			break;
		case 21 :
			sesso = 'V';
			break;
		case 22 :
			sesso = 'W';
			break;
		case 23 :
			sesso = 'X';
			break;
		case 24 :
			sesso = 'Y';
			break;
		case 25 :
			sesso = 'Z';
			break;
		default :
			MessageBox(NULL, "Si prega di controllare i dati.", "Errore", MB_OK);
		    errore = 1;
	}
	fiscale[15] = sesso;
}
void estrazione(char nome[], char* cons, char* voc) {
	char consonanti[100];
	char vocali[100];
	int i = 0;
	int j = 0;
	int k = 0;
	int count = 0;
	nome[strcspn(nome, "\n")] = '\0';
	for (i = 0; nome[i] != '\0'; i++) {
		nome[i] = toupper(nome[i]);
	}
	i = 0;
	for (i = 0; nome[i] != '\0'; i++) {
		if (nome[i] == ' ') {
			nome[i] = nome[i + 1];
			for (j = i + 1; nome[j] != '\0'; j++) {
				nome[j] = nome[j + 1];
			}
		}
	}
	nome[i + 1] = '\0';
	i = 0;
	j = 0;
	for (i = 0; nome[i] != '\0'; i++) {
		if (nome[i] == 'A' || nome[i] == 'E' || nome[i] == 'I' || nome[i] == 'O' || nome[i] == 'U') {
			vocali[j] = nome[i];
			j++;
		}
		else {
			consonanti[k] = nome[i];
			k++;
		}
	}
	vocali[j] = '\0';
	consonanti[k] = '\0';
	strcpy(cons, consonanti);
	strcpy(voc, vocali);
}
void primecifre(int a, int b, char checkvoc[], char checkcons[]) {
	if(a < 3) {
		if(a < 2) {
			if (a < 1) {
				if (b < 3) {
					if (b < 2) {
						if (b < 1) {
							provvisorio[0] = 'X';
							provvisorio[1] = 'X';
							provvisorio[2] = 'X';
						}
						else {
							provvisorio[0] = checkvoc[0];
							provvisorio[1] = 'X';
							provvisorio[2] = 'X';
						}
					}
					else {
						provvisorio[0] = checkvoc[0];
						provvisorio[1] = checkvoc[1];
						provvisorio[2] = 'X';
					}
				}
				else {
					provvisorio[0] = checkvoc[0];
					provvisorio[1] = checkvoc[1];
					provvisorio[2] = checkvoc[2];
				}
			}
			else {
				provvisorio[0] = checkcons[0];
				provvisorio[1] = checkvoc[0];
				provvisorio[2] = checkvoc[1];
			}
		}
		else {
			provvisorio[0] = checkcons[0];
			provvisorio[1] = checkcons[1];
			provvisorio[2] = checkvoc[0];
		}
	}
	else {
		provvisorio[0] = checkcons[0];
		provvisorio[1] = checkcons[1];
		provvisorio[2] = checkcons[2];
	}
}
void loadImages() {
	hLogoImage = (HBITMAP)LoadImageW(NULL, L"logocodicefiscale.bmp", IMAGE_BITMAP, 200, 100, LR_LOADFROMFILE);
}
