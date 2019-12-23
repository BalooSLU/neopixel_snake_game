/******************************************************

Konstantin Meyer


******************************************************/

#include "snake.h"
#include <Adafruit_NeoPixel.h>
#include <ESPAsyncWebServer.h> 

#define LED_COUNT 12
#define PIN0 2

direction move = DOWN;
int field[WIDTH][HEIGHT];

Adafruit_Neopixel strip_0(LED_COUNT, PIN0, NEO_RGB + NEO_KHZ800);

void neoPixel_setup()
{

}

//Funktion gibt das Spielfeld auf der Konsole aus
//TODO Ändern so dass das Feld auf dem NeoPixel Ausgegeben wird.
void printField(int points)
{
	int y, x, i;
	char c;

	for (y = 0; y < HEIGHT; y++)
	{
		for (x = 0; x < WIDTH; x++)
		{
			switch (field[x][y])
			{
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
	for (i = 0; i < WIDTH; i++)
		printf("%c", 205);
	printf("%c", 188);
	printf("\n\nPunkte: %d", points);
}

//Funktion erzeugt ein neues Futerteil auf dem Spielfeld
void food()
{
	int x_rand, y_rand, result = 1;
	do
	{
		x_rand = rand() % (WIDTH);  //Diese Zufallsfunktion erzeugt keine gleichmäßig verteilten Zufallszahlen
		y_rand = rand() % (HEIGHT); //für eine nicht sicherheitskritische Anwendung wie hier ist sie jedoch hinreichend.

		if (field[x_rand][y_rand] == 0)
		{
			field[x_rand][y_rand] = 2;
			result = 0;
		}
	} while (result);
}

//Schrittfunktion bewegt die Schlange einen Schritt weiter und reagiert auf entsprechende Besonderheiten (GameOver, Futter) gibt 0 bei niederlage zurück
int step(snakePart **first, snakePart **last, int *points)
{
	snakePart *newPart, *temp;
	int x, y, x_new, y_new, state;

	switch (move)
	{
	case UP:
		x = 0;
		y = -1;
		break;
	case DOWN:
		x = 0;
		y = 1;
		break;
	case LEFT:
		x = -1;
		y = 0;
		break;
	case RIGHT:
		x = 1;
		y = 0;
		break;
	}
	x_new = ((*first)->x + x + WIDTH) % WIDTH;
	y_new = ((*first)->y + y + HEIGHT) % HEIGHT;

	state = field[x_new][y_new];

	if (state != 1)
	{
		newPart = (snakePart *)malloc(sizeof(snakePart)); //Speicher allokieren für neues Schlangenteil
		if (newPart == NULL)
			return 0;
		newPart->x = x_new;
		newPart->y = y_new;
		field[x_new][y_new] = 1;

		(*first)->next = newPart;
		(*first) = newPart;

		if (state == 0)
		{
			temp = (*last)->next;
			field[(*last)->x][(*last)->y] = 0;
			free(*last);
			*last = temp;
		}
		else
		{
			food();
			*points += 1;
		}
		return 1;
	}
	else
		return 0;
}

//interrupthandler für left
void left(AsyncWebServerRequest *request)
{
	move = LEFT;
	request->send_P(200, "text/plain", "");
}

//interrupthandler für right
void right(AsyncWebServerRequest *request)
{
	move = RIGHT;
	request->send_P(200, "text/plain", "");
}

//interrupthandler für up
void up(AsyncWebServerRequest *request)
{
	move = UP;
	request->send_P(200, "text/plain", "");
}

//interrupthandler für down
void down(AsyncWebServerRequest *request)
{
	move = DOWN;
	request->send_P(200, "text/plain", "");
}

//Funktion zur initialisierung des Spiels
void gameInit(snakePart **first, snakePart **last)
{
	int x, y;

	for (x = 0; x < WIDTH; x++)
	{
		for (y = 0; y < HEIGHT; y++)
		{
			ZERO(field[x][y]);
		}
	}

	*first = malloc(sizeof(snakePart));
	(*first)->x = 1;
	(*first)->y = 2;

	*last = malloc(sizeof(snakePart));
	(*last)->x = 1;
	(*last)->y = 1;

	(*last)->next = *first;
	field[1][1] = 1;
	field[1][2] = 1;
	field[1][3] = 2;
}

//Funktion für den Fall der Niederlage
int gameOver(int points)
{

	return 0;
}

//Funktion zum starten des Spiels
void gameStart(char name[20], highscoreEntry highscore[10])
{
	snakePart *first = NULL, *last;
	int points = 0, resume;
	move = RIGHT;

	gameTeaser();
	gameInit(&first, &last);

	do
	{
		resume = step(&first, &last, &points);
		printField(points);
		Sleep(400);
	} while (resume);
	gameOver(points);
}