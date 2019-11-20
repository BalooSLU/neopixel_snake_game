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

//Funktion Bereinigt den StandardInput
void flashStandardInput(void) {
	char ch;
	while ((ch = getchar() != '\n') && (ch != EOF));
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