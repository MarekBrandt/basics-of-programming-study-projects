#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

#define SCREEN_WIDTH 640 //px
#define SCREEN_HEIGHT 480 //px
#define INFOBAR_HEIGHT 40 //px
#define UNISPEED 8
#define MAX_FPS 60
#define UNIXPOS 50 //odleglosc jednoroza od lewej krawdzi px
#define OBSTACLES 10 // liczba przeszkod
#define JUMP_TIME 0.5 // w sekundach
#define JUMPS 2
#define UNIACC 1000 //wspolczynnik przyspieszenia jednorozca, im mniejszy tym szybciej gra przyspiesza (zalecane okolice 1000)
#define DASH_ACC 60 //wartosc przyspieszenia dasha
#define DASH_TIME 0.08 //czas trwania dasha sekundy

enum sterowanie
{
	arrows,
	freerun,
};


// narysowanie napisu txt na powierzchni screen, zaczynajπc od punktu (x, y)
// charset to bitmapa 128x128 zawierajπca znaki
// draw a text txt on surface screen, starting from the point (x, y)
// charset is a 128x128 bitmap containing character images
void DrawString(SDL_Surface* screen, int x, int y, const char* text,
	SDL_Surface* charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	};
};


// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to lewy gorny rog obrazka sprite na ekranie
// draw a surface sprite on a surface screen in point (x, y)
// (x, y) is the left corner of sprite on screen
void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
};


// rysowanie pojedynczego pixela
// draw a single pixel
void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32*)p = color;
};


// rysowanie linii o d≥ugoúci l w pionie (gdy dx = 0, dy = 1) 
// bπdü poziomie (gdy dx = 1, dy = 0)
// draw a vertical (when dx = 0, dy = 1) or horizontal (when dx = 1, dy = 0) line
void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for (int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	};
};


// rysowanie prostokπta o d≥ugoúci bokÛw l i k
// draw a rectangle of size l by k
void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k,
	Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for (i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};

void checkBorders(double& xpos, double& ypos, SDL_Surface* uni, SDL_Surface* background, bool& naPodlozu, int& jumpCounter)
{
	if (xpos - uni->w / 2 < 0) //lewa krawedz
	{
		xpos += UNISPEED;
	}

	if (ypos - uni->h / 2 < INFOBAR_HEIGHT) //gorna krawedz, tj dolna krawedz infobara
	{
		ypos += UNISPEED;
	}

	if (ypos + uni->h / 2 > SCREEN_HEIGHT) //dolna krawedz ekranu
	{
		ypos -= UNISPEED;
		naPodlozu = 1;
		jumpCounter = 0;
	}

	if (xpos > (0.9005 * background->w)) //osiaganie konca mapy, przeniesienia na poczatek
	{
		xpos = uni->w;
	}
}

void countFPS(double& fpsTimer, double delta, double& fps, int& frames)
{
	fpsTimer += delta;
	if (fpsTimer > 0.5) {
		fps = frames * 2;
		frames = 0;
		fpsTimer -= 0.5;
	};
}

void setFPS(double t2, double t3)
{
	if (1000 / MAX_FPS > t3 - t2)
	{
		SDL_Delay(1000 / MAX_FPS - t3 + t2);
	}
}

void velTo0(SDL_Event event, double& velx, double& vely)
{
	switch (event.key.keysym.sym)
	{
	case SDLK_UP:
		vely = 0;
		break;
	case SDLK_DOWN:
		vely = 0;
		break;
	case SDLK_RIGHT:
		velx = 0;
		break;
	case SDLK_LEFT:
		velx = 0;
		break;
	default:
		break;
	}
}

void setNewGame(double& worldTime, double& xpos, double& ypos)
{
	xpos = 0;
	ypos = 0;
	worldTime = 0;
}

int loadBMP(SDL_Surface*& charset, SDL_Surface*& uni,
	SDL_Surface*& background, SDL_Surface* screen,
	SDL_Texture* scrtex, SDL_Window* window, SDL_Renderer* renderer)
{
	charset = SDL_LoadBMP("./cs8x8.bmp");
	if (charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};
	SDL_SetColorKey(charset, true, 0x000000);

	uni = SDL_LoadBMP("./eti2.bmp");
	if (uni == NULL) {
		printf("SDL_LoadBMP(eti2.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(charset);
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	background = SDL_LoadBMP("./background2.bmp");
	if (background == NULL) {
		printf("SDL_LoadBMP(background2.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(charset);
		SDL_FreeSurface(uni);
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};
	return 0;
}

void createRocks(SDL_Rect rock[OBSTACLES], double mapWidth, double screenHeight, int rockWidth, int rockHeight)
{
	double distance = mapWidth / (OBSTACLES + 3);

	for (int i = 0; i < OBSTACLES; i++)
	{
		rock[i] = { (int)((i + 2) * distance), int(screenHeight - rockHeight), rockWidth, rockHeight };
	}
}

void drawObstacles(SDL_Surface* background, SDL_Rect rock[OBSTACLES], int kolor)
{
	for (int i = 0; i < OBSTACLES; i++)
	{
		SDL_FillRect(background, &rock[i], kolor);
	}
}

bool checkObstacles(SDL_Rect uniHitBox, SDL_Rect rock[], bool dash)
{
	for (int i = 0; i < OBSTACLES; i++)
	{
		if (SDL_HasIntersection(&uniHitBox, &rock[i]) == SDL_TRUE)
		{
			/*if (dash == true) //przenikanie przeszkod podczas dasha
			{
				return false;
			}
			else*/
			{
				printf("intersection bylo na rocku %d\n", i);
				return true;
			}
		}
	}
	return false;
}

void showInfoBar(SDL_Surface* screen, SDL_Surface* charset, int borderColor,
	int bgColor, double worldTime, double fps, char text[], sterowanie poruszanie, double multiplier)
{
	// tekst informacyjny / info text
	DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, INFOBAR_HEIGHT, borderColor, bgColor);
	//            "template for the second project, elapsed time = %.1lf s  %.0lf frames / s"

	if (poruszanie == arrows)
	{
		sprintf(text, "Unicorn Robot Atack, czas trwania = %.1lf s  %.0lf klatek / s", worldTime, fps);
	}
	else
	{
		sprintf(text, "Unicorn Robot Atack, czas trwania = %.1lf s  %.0lf klatek / s, predkosc = %f", worldTime, fps, UNISPEED * multiplier);
	}
	DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);
	//	      "Esc - exit, \030 - faster, \031 - slower"
	if (poruszanie == arrows)
	{
		sprintf(text, "Esc - wyjscie, n - nowa gra, \030\031 - gora/dol, \033\032 - prawo/lewo, d - free run");
	}
	else
	{
		sprintf(text, "Esc - wyjscie, n - nowa gra, z - skok, x - dash, d - arrows");
	}
	DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 26, text, charset);
}

void increaseMulitplier(double& multiplier, double worldTime)
{
	multiplier = worldTime / UNIACC + 1;
}

void ruchJednorozca(sterowanie poruszanie, double& xpos, double& velx, double& ypos, double& vely, double& jumpTime, double delta, double& dashTime, double multiplier, bool& dash, bool naPodlozu, int jumpsCounter, bool& canJump)
{
	if (poruszanie == arrows)
	{
		xpos += velx;
		ypos += vely;
	}
	else if (poruszanie == freerun)
	{
		jumpTime += delta;
		dashTime += delta;

		xpos += (UNISPEED * multiplier + velx);

		if (dash == true)
		{
			if (dashTime > DASH_TIME)
			{
				velx = 0;
				vely = 0;
				dash = false;
			}
		}
		else if (naPodlozu == true || jumpsCounter < JUMPS) //na podlozu albo w powietrzu i moze skakac jeszcze 
		{
			//printf("%d", jumpsCounter);
			canJump = true;

			if (jumpTime >= JUMP_TIME)
			{
				vely = 0;
			}
		}
		else
		{
			canJump = false;
			vely = 0;
		}

		ypos += (UNISPEED + vely); //domyslnie grawitacja o wartosci unispeed, vely domyslnie 0
	}
}


// main
#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char** argv) {
	int t1, t2, t3, quit, frames, rc, jumpsCounter;
	double delta, worldTime, fpsTimer, fps, distance, xpos, ypos, velx, vely, multiplier, jumpTime, dashTime;
	bool canJump, naPodlozu, skokTrwa, dash;
	sterowanie poruszanie;
	SDL_Event event;
	SDL_Surface* screen, * charset;
	SDL_Surface* uni, * background;
	SDL_Texture* scrtex;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Rect rock[OBSTACLES];
	SDL_Rect uniHitBox;

	// okno konsoli nie jest widoczne, jeøeli chcemy zobaczyÊ
	// komunikaty wypisywane printf-em trzeba w opcjach:
	// project -> szablon2 properties -> Linker -> System -> Subsystem
	// zmieniÊ na "Console"
	// console window is not visible, to see the printf output
	// the option:
	// project -> szablon2 properties -> Linker -> System -> Subsystem
	// must be changed to "Console"
	printf("wyjscie printfa trafia do tego okienka\n");
	printf("printf output goes here\n");

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}

	// tryb pe≥noekranowy / fullscreen mode
//	rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP,
//	                                 &window, &renderer);
	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
		&window, &renderer);
	if (rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "Unicorn Robot Atack");


	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);


	// wy≥πczenie widocznoúci kursora myszy
	SDL_ShowCursor(SDL_DISABLE);

	// wczytanie charsetu, sprite'a i tla
	loadBMP(charset, uni, background, screen, scrtex, window, renderer);

	//tworze hitbox jednorozca
	uniHitBox.w = uni->w;
	uniHitBox.h = uni->h;


	//tworze przeszkody
	createRocks(rock, background->w, screen->h, 50, 70);

	char text[128];
	int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	int niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);
	int brazowy = SDL_MapRGB(screen->format, 139, 69, 19);

	t1 = SDL_GetTicks();

	frames = 0;
	fpsTimer = 0;
	fps = 0;
	quit = 0;
	worldTime = 0;
	xpos = SCREEN_WIDTH / 2;
	ypos = (SCREEN_HEIGHT - INFOBAR_HEIGHT) / 2;
	velx = 0;
	vely = 0;
	poruszanie = arrows;
	multiplier = 1;
	jumpTime = 0;
	dashTime = 0;
	jumpsCounter = 0;
	canJump = 0;
	naPodlozu = 0;
	skokTrwa = 0;
	dash = 0;

	while (!quit) {
		t2 = SDL_GetTicks();

		// w tym momencie t2-t1 to czas w milisekundach,
		// jaki uplyna≥ od ostatniego narysowania ekranu
		// delta to ten sam czas w sekundach
		// here t2-t1 is the time in milliseconds since
		// the last screen was drawn
		// delta is the same time in seconds
		delta = (t2 - t1) * 0.001;
		t1 = t2;

		worldTime += delta;

		increaseMulitplier(multiplier, worldTime);
		//liniowo zwieksza mnoznik predkosci

		ruchJednorozca(poruszanie, xpos, velx, ypos, vely, jumpTime, delta, dashTime, multiplier, dash, naPodlozu, jumpsCounter, canJump);
		//odpowiada za poruszanie jednorozca w zaleznoci od kliknietych przyciskow

		SDL_FillRect(screen, NULL, czarny);
		//czysci ekran, wypelnia na czarno

		checkBorders(xpos, ypos, uni, background, naPodlozu, jumpsCounter);
		//sprawdza czy jednorozec nie wychodzi poza okno, zapobiega takiemu dzialania
		//zapetla mape

		if (checkObstacles(uniHitBox, rock, dash) == true) //sprawdza czy sprite nie natrafil na przeszkode
		{
			setNewGame(worldTime, xpos, ypos);
		}

		DrawSurface(screen, background, background->w / 2 - xpos, SCREEN_HEIGHT / 2); //rysuje tlo

		//narysuj kamienie
		drawObstacles(background, rock, brazowy);

		//zeby zobaczyc hitbox
		//SDL_FillRect(background, &uniHitBox, czerwony);
		uniHitBox.x = xpos;
		uniHitBox.y = ypos - uni->h / 2;

		DrawSurface(screen, uni, UNIXPOS, ypos); //rysuje jednorozca

		countFPS(fpsTimer, delta, fps, frames); //oblicza klatki na sekunde

		showInfoBar(screen, charset, czerwony, niebieski, worldTime, fps, text, poruszanie, multiplier);//wyswietla pasek z info

		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch); //wyrzuca bufor ekranu
		//		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);

		// obs≥uga zdarzeÒ (o ile jakieú zasz≥y) / handling of events (if there were any)
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
			{
				quit = 1;
			}
			if (poruszanie == arrows) //dla poruszania na strzalkach
			{
				if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						quit = 1;
						break;
					case SDLK_UP:
						vely = -UNISPEED;
						break;
					case SDLK_DOWN:

						vely = UNISPEED;
						break;
					case SDLK_RIGHT:
						velx = UNISPEED;
						break;
					case SDLK_LEFT:
						velx = -UNISPEED;
						break;
					case SDLK_n:
						setNewGame(worldTime, xpos, ypos);
						break;
					case SDLK_d:
						poruszanie = freerun;
						setNewGame(worldTime, xpos, ypos);
						break;
					default:
						break;
					}
				}
				else if (event.type == SDL_KEYUP && event.key.repeat == 0)
				{
					//ustawia predkosc jednorozca na 0
					velTo0(event, velx, vely);
				}
			}
			else if (poruszanie == freerun) //przy poruszaniu, gdy jednorozec biegnie sam
			{
				if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						quit = 1;
						break;
					case SDLK_z:

						if (canJump == true)
						{
							naPodlozu = 0;
							jumpTime = 0;
							vely = -2 * UNISPEED;
						}
						else
						{
							//printf("nie moge skakac\n skokow oddanych %d\n", jumpsCounter);
						}
						break;
					case SDLK_x:
						dash = true;
						dashTime = 0;
						velx = DASH_ACC;
						vely = -UNISPEED; //zrownowazy grawitacje
						if (jumpsCounter > 0)
						{
							jumpsCounter--;
						}
						break;
					case SDLK_n:
						setNewGame(worldTime, xpos, ypos);
						break;
					case SDLK_d:
						poruszanie = arrows;
						setNewGame(worldTime, xpos, ypos);
						break;
					default:
						break;
					}
				}
				else if (event.type == SDL_KEYUP && event.key.repeat == 0)
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_z:
						if (canJump == true)
						{
							jumpsCounter++;
						}
						vely = 0;
						break;
					case SDLK_x:
						break;
					default:
						break;
					}
				}
			}
		};
		t3 = SDL_GetTicks();

		setFPS(t2, t3); //ustawia fps na zadana wartosc w stalej max fps

		frames++;
	};

	// zwolnienie powierzchni / freeing all surfaces
	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_FreeSurface(background);
	SDL_FreeSurface(uni);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}


