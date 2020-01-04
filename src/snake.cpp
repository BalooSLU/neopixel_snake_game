/******************************************************

Konstantin Meyer


******************************************************/
#include <NeoPixelBus.h>
#include <ESPAsyncWebServer.h>


#define WIDTH 20		
#define HEIGHT 12
#define ZERO(v) v = 0
#define LED_COUNT 240
#define LED_PIN 23

typedef enum {	//Verwendung einer Aufzählung #8
	UP,
	DOWN, LEFT, RIGHT
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
volatile direction move = DOWN;
int field[WIDTH][HEIGHT];

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(LED_COUNT, LED_PIN);

void Sleep(int time) {
	int i = millis() + time;
	while(millis()< i);
}

void neoPixel_setup()
{
	strip.Begin();
	strip.Show();
}

//Funktion gibt das Spielfeld auf der Konsole aus
//TODO Ändern so dass das Feld auf dem NeoPixel Ausgegeben wird.
void printField(int points)
{
	int y, x;
	RgbColor red(128,0,0);
	RgbColor black(0);
	RgbColor blue(0,0,128);
	RgbColor c(0);

	for (y = 0; y < HEIGHT; y++)
	{
		for (x = 0; x < WIDTH; x++)
		{
			switch (field[x][y])
			{
			case 0:
				c = black;
				break;
			case 1:
				c = blue;
				break;
			case 2:
				c = red;
				break;
			}
			if (y % 2 == 0) {
				strip.SetPixelColor((y*WIDTH)+x, c);
			} else {
				strip.SetPixelColor(((y+1)*WIDTH)-x-1, c);
			}
			
			
		}
		
	}
	strip.Show();
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
		y = 1;
		break;
	case DOWN:
		x = 0;
		y = -1;
		break;
	case LEFT:
		x = -1;
		y = 0;
		break;
	default:
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
	Serial.println("Left");
	move = LEFT;
	request->send_P(200, "text/plain", "");
}

//interrupthandler für right
void right(AsyncWebServerRequest *request)
{
	Serial.println("Right");
	move = RIGHT;
	request->send_P(200, "text/plain", "");
}

//interrupthandler für up
void up(AsyncWebServerRequest *request)
{
	Serial.println("Up");
	move = UP;
	request->send_P(200, "text/plain", "");
}

//interrupthandler für down
void down(AsyncWebServerRequest *request)
{
	Serial.println("Down");
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

	*first = (snakePart *)malloc(sizeof(snakePart));
	(*first)->x = 1;
	(*first)->y = 2;

	*last = (snakePart *)malloc(sizeof(snakePart));
	(*last)->x = 1;
	(*last)->y = 1;

	(*last)->next = *first;
	field[1][1] = 1;
	field[1][2] = 1;
	field[1][3] = 2;
}

//Funktion zum starten des Spiels
void gameStart(void)
{
	snakePart *first = NULL, *last;
	int points = 0, resume;
	move = DOWN;
	Serial.println("GameStart Waiting for RIGHT");
	while(move!=RIGHT);

	gameInit(&first, &last);

	Serial.println("GameINIT succesfull");
	do
	{
		resume = step(&first, &last, &points);
		printField(points);
		Sleep(400);
	} while (resume);
}