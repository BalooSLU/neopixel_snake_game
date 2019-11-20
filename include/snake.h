#pragma once
#define _CRT_SECURE_NO_WARNINGS

//Verwendung von Makros #16
//Symbolische Konstanten #17

#define uuml "\201"
#define Uuml "\232"
#define auml "\204"
#define Auml "\216"
#define ouml "\224"
#define Ouml "\231"
#define szlig "\341"

#define WIDTH 30		
#define HEIGHT 10
#define ZERO(v) v = 0

#include <stdlib.h>
#include <stdio.h>
//#include <Windows.h>
#include <math.h>
#include <String.h>
#include <ctype.h>

typedef enum {	//Verwendung einer Aufzählung #8
	up,
	down, left, right
} direction;

typedef struct snake_part { //Verwendung von Strukturen #7 + Typennamenneudefinition #9
	short x;
	short y;
	struct snake_part *next;
} snakePart;

typedef struct {
	int points;
	char name[20];
}highscoreEntry;


const int menu_items = 3;
const char highscoreFile[] = {"bestenliste"};
const char gameText[] = "Du hast das Spiel gestartet.\n"
						"Sehr gut!\n" 
						"Erinnerst du dich noch daran die Schlange mit den Pfeiltasten zu steuern?\n"
						"Entscheide vorsichtig wohin du gehst und sammle immer genug Futter!\n"
						"Du darfst dich nicht selber rammen!\n"
						"Gleich geht's los! Halte dich bereit!\n";
const char gameOverText_highscore[] =	"Auch das beste Spiel ist irgendwann vorbei.\n"
										"Schade!\n"
										"Mach dir nichts daraus du kannst dich verbessern.\n"
										"Starte einfach eine weitere Runde!";
const char gameOverText[] = "Das war ein guter Versuch!\n"
							"Es hat nicht gereicht um auf die Bestenliste zu kommen.\n"
							"Versuche es immer weiter!\n"
							"Irgendwann klappt es da bin ich mir sicher!\n";

//Funktion Bereinigt den StandardInput
void flashStandardInput(void) {
	char ch;
	while ((ch = getchar() != '\n') && (ch != EOF));
}

//Funktion zum ausgeben eines Strings Zeichen für Zeichen um den Eindruck von Tippen zu erwecken
void printType(const char *text) {
	int i, n, delay=100;
	n = strlen(text);
	GetAsyncKeyState(VK_ESCAPE);
	for (i = 0; i < n; i++) {		
		printf("%c", text[i]);
		Sleep(delay);
		if (GetAsyncKeyState(VK_ESCAPE)) delay = 0;
		if ((text[i] == '.') || (text[i] == '!') || (text[i] == '?')) Sleep(2*delay);
	}
}

//FUnktion gibt den übergebenen String in GROSSBUCHSTABEN zurück. Der ursprüngliche Wert wird nicht verändert.
char * toCaps(char * string) {
	int i, n = strlen(string);  //Verwendung strlen() #3
	char * result;

	result = malloc(sizeof(char)*n);

	for (i = 0; i < n; i++) {
		result[i] = toupper(string[i]);
	}
	result[n] = '\0';
	return result;
}