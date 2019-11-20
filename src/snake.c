/******************************************************
Praktikum Programmieren 2
Aufgabe 4
03.01.2017
Konstantin Meyer

Aufgabe ist es Snake zu programmieren. Die folgenden Vorgaben müssen erfüllt werden:
- Der Nutzer muss die möglichkeit haben das Programm mit seinem Namen als Parameter von der Konsole zu starten
- Es werden (mehr oder weniger) zufällige Positionen für das Futter ausgesucht
- Es soll möglich sein das SPielfeld auf der einen Seite zu verlassen und auf der anderen wieder einzutreten
- Die Schlange soll mit den Pfeiltasten gesteuert werden
- Steuert der Spieler die Schlange gegen ein Schlangenglied so verliert er
- Jedes Futterteil bringt einen Punkt
- Es soll eine Bestenliste geben
- Es soll ein Hauptmenu geben aus dem auf die Bestenliste zugegriffen werden kann und ein neues Spiel gestartet werden kann.
- Das Spiel soll über eine umfangreiche Hilfe verfügen
******************************************************/

//Umlenken der Standartausgabe bei aufruf mit Namen "help" sinnvoll möglich #18 aufruf aus der Konsole z.B. "snake.exe help > hilfe.txt"

#include "snake.h" //Verwendug der eigens für das Projekt geschriebenen Header Datei #19


//Funktion gibt die Bestenliste auf der Konsole aus
void printHighscore(highscoreEntry highscore[10]) {
	int i;
	system("cls");
	printf("---Die aktuelle Bestenliste---\n");
	for (i = 0; i < 10; i++) {
		printf("%5d\t%-20s\n", highscore[i].points, highscore[i].name);
	}
	system("pause");
}

//Funktion sortiert die übergebene Bestenliste
void sort_highscore(highscoreEntry list[11]) {
	int i, stop;
	highscoreEntry help;
	do {
		stop = 1;
		for (i = 0; i < 10; i++) {
			if (list[i].points < list[i + 1].points) {
				help.points = list[i].points;
				strcpy(help.name, list[i].name);

				list[i].points = list[i + 1].points;
				strcpy(list[i].name, list[i + 1].name);

				list[i + 1].points = help.points;
				strcpy(list[i + 1].name, help.name);

				stop = 0;
			}
		}
	} while (!stop);
}

//Funktion speichert die Bestenliste in einer Datei ab
int highscoreSave(highscoreEntry highscore[10]) {
	FILE *fp;
	fp = fopen(highscoreFile, "w");
	if (fp == NULL) return -1;
	fwrite(highscore, sizeof(highscoreEntry), 10, fp);	//In eine Datei schreiben #11
	fclose(fp);
	return 0;
}

//Funktion lädt die Bestenliste aus einer Datei
int highscoreLoad(highscoreEntry highscore[10]) {
	FILE *fp;
	int i;

	fp = fopen(highscoreFile, "a+");
	if (fp == NULL) {
		return 1;
	}
	else {
		rewind(fp);	//Wahlfreier Zugriff #12
		if (fread(highscore, sizeof(highscoreEntry), 10, fp) == 0) { //Lesen aus einer Datei #10
			for (i = 0; i < 10; i++) {
				strcpy(highscore[i].name, "---");
				highscore[i].points = 0;
			}
			fwrite(highscore, sizeof(highscoreEntry), 10, fp);
		}
		fclose(fp);
	}
	return 0;
}

//Funktion gibt das Spielfeld auf der Konsole aus
//TODO Ändern so dass das Feld auf dem NeoPixel Ausgegeben wird.
void printField(int field[WIDTH][HEIGHT], int points) {
	int y, x, i;
	char c;
	system("cls");
	printf("%c", 201);
	for (i = 0; i < WIDTH; i++) printf("%c", 205);
	printf("%c\n", 187);

	for (y = 0; y < HEIGHT; y++) {
		printf("%c", 186);
		for (x = 0; x < WIDTH; x++) {
			switch (field[x][y]) {
			case 0:
				c = ' ';
				break;
			case 1:
				c = 'o';
				break;
			case 2:
				c = 'x';
				break;
			}
			printf("%c", c);
		}
		printf("%c\n", 186);
	}

	printf("%c", 200);
	for (i = 0; i < WIDTH; i++) printf("%c", 205);
	printf("%c", 188);
	printf("\n\nPunkte: %d", points);
}

//Funktion erzeugt ein neues Futerteil auf dem Spielfeld
void food(int field[WIDTH][HEIGHT]) {
	int x_rand, y_rand, result = 1;
	do {
		x_rand = rand() % (WIDTH);	//Diese Zufallsfunktion erzeugt keine gleichmäßig verteilten Zufallszahlen
		y_rand = rand() % (HEIGHT);	//für eine nicht sicherheitskritische Anwendung wie hier ist sie jedoch hinreichend.

		if (field[x_rand][y_rand] == 0) {
			field[x_rand][y_rand] = 2;
			result = 0;
		}
	} while (result);
}

//Schrittfunktion bewegt die Schlange einen Schritt weiter und reagiert auf entsprechende Besonderheiten (GameOver, Futter) gibt 0 bei niederlage zurück
int step(direction move, snakePart **first, snakePart **last, int field[WIDTH][HEIGHT], int *points) {
	snakePart *newPart, *temp;
	int x, y,x_new,y_new, state;

	switch (move) {
	case up:
		x = 0;
		y = -1;
		break;
	case down:
		x = 0;
		y = 1;
		break;
	case left:
		x = -1;
		y = 0;
		break;
	case right:
		x = 1;
		y = 0;
		break;
	}
	x_new = ((*first)->x + x + WIDTH) % WIDTH;
	y_new = ((*first)->y + y + HEIGHT) % HEIGHT;
	
	state = field[x_new][y_new];

	if (state != 1) {
		newPart = malloc(sizeof(snakePart)); //Speicher allokieren #13 Verkettete Liste #15
		if (newPart == NULL) return 0;
		newPart->x = x_new;
		newPart->y = y_new;
		field[x_new][y_new] = 1;

		(*first)->next =  newPart;
		(*first) = newPart;

		if (state == 0) {
			temp = (*last)->next;
			field[(*last)->x][(*last)->y] = 0;
			free(*last);
			*last = temp;
		}
		else {
			food(field);
			*points += 1;
		}
		return 1;
	}
	else return 0;
}

//Funktion fragt die Richtungseingabe ab
void getDirection(direction *move) {
    /* TODO
	if (GetAsyncKeyState(VK_UP)) *move = up;
	else if (GetAsyncKeyState(VK_DOWN)) *move = down;
	else if	(GetAsyncKeyState(VK_LEFT)) *move = left;
	else if (GetAsyncKeyState(VK_RIGHT)) *move = right;
    */
}

//Funktion zur initialisierung des Spiels
void gameInit(int field[WIDTH][HEIGHT],snakePart **first, snakePart **last) { //Verwendung Pointer auf Pointer #4
	int x,y;
    /* TODO 
	GetAsyncKeyState(VK_UP);
	GetAsyncKeyState(VK_DOWN);
	GetAsyncKeyState(VK_LEFT);
	GetAsyncKeyState(VK_RIGHT);
    */

	for (x = 0; x < WIDTH; x++) {
		for (y = 0; y < HEIGHT; y++) {
			ZERO(field[x][y]);
		}
	}

	*first = malloc(sizeof(snakePart));
	(*first)->x = 0;
	(*first)->y = 1;

	*last = malloc(sizeof(snakePart));
	(*last)->x = 0;
	(*last)->y = 0;

	(*last)->next = *first;
	field[0][0] = 1;
	field[0][1] = 1;
	field[0][2] = 2;
	
}

//Funktion für den Fall der Niederlage
int gameOver(int points, char name[20], highscoreEntry highscore[10]) {
	int i;
	highscoreEntry *p_score;

	p_score = malloc(sizeof(highscoreEntry)*10);
	if (p_score == NULL) return -1;
	if (points > highscore[9].points) {
		
		Sleep(1000);
		realloc(p_score, sizeof(highscoreEntry)*11);
		for (i = 0; i < 10; i++) {
			strcpy(p_score[i].name, highscore[i].name);
			p_score[i].points = highscore[i].points;
		}
		p_score[10].points = points;
		strcpy(p_score[10].name, name);		//Verwendung strcpy() #3

		sort_highscore(p_score);
		realloc(p_score, sizeof(highscoreEntry) * 10);	//Speicher Reallokieren #14
		highscoreSave(p_score);
		printHighscore(p_score);
		highscoreLoad(highscore);
	}
	else {
		//Kein neuer Highscore
	}
	return 0;
}

//Funktion zum starten des Spiels
void gameStart(char name[20], highscoreEntry highscore[10]) {
	snakePart *first = NULL, *last;
	int field[WIDTH][HEIGHT], points = 0, resume;	//Verwendung mehrdimensionales Array #2
	direction move=right;

	gameTeaser();
	gameInit(field,&first,&last);

		do {
			getDirection(&move);
			resume = step(move, &first, &last, field, &points);
			printField(field,points);
			Sleep(400);
		} while (resume);
		gameOver(points,name, highscore);
}