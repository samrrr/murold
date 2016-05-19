#include <windows.h>		// Header File For Windows
#include <math.h>			// Header File For Math Library Routines


#include <string.h>  // ???????????? ???? ??? Windows
#include <time.h>    // ???????????? ???? ??? ??????????? ?????????? ?????/??????(?????)
#include <gl\gl.h>    // ???????????? ???? ??? ?????????? OpenGL32
#include <gl\glu.h>   // ???????????? ???? ??? ?????????? GLu32
//#include <gl\glaux.h> // ???????????? ???? ??? ?????????? GLaux
#include <olectl.h>          // ???????????? ???? ??? ??????????
#include <stdarg.h>   // Заголовочный файл для функций для работы с переменным
//#include <fstream.h>
#include <mmsystem.h>
#include <math.h>

#include <conio.h>
#include <stdio.h>
#include <malloc.h>
#include <string>
#include <vector>
#include <deque>

#include "out.h"
#include "mur.h"
#include "samrrr_bibl.h"

#include "NeHeGL.h"														// Header File For NeHeGL

#pragma comment( lib, "opengl32.lib" )									// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )										// Search For GLu32.lib While Linking

static BOOL g_isProgramLooping;											// Window Creation Loop, For FullScreen/Windowed Toggle																		// Between Fullscreen / Windowed Mode
static BOOL g_createFullScreen;
#ifndef CDS_FULLSCREEN													// CDS_FULLSCREEN Is Not Defined By Some
#define CDS_FULLSCREEN 4												// Compilers. By Defining It This Way,
#endif																	// We Can Avoid Errors
int exitgame
, razokx =/*/640/*/GetSystemMetrics(SM_CXSCREEN)/**/
, razoky =/*/480/*/GetSystemMetrics(SM_CYSCREEN)/**/;
float fps = 0;

GL_Window			window;

GLuint  base;      // База списка отображения для фонта

HDC HDc;

GLuint tex_mur;
int M_speed;



using namespace std;
//#define MAX_AST 30
#define WM_TOGGLEFULLSCREEN (WM_USER+1)								

typedef struct													// Create A Structure
{
	GLubyte	*imageData;											// Image Data (Up To 32 Bits)
	GLuint	bpp;												// Image Color Depth In Bits Per Pixel.
	GLuint	width;												// Image Width
	GLuint	height;												// Image Height
	GLuint	texID;												// Texture ID Used To Select A Texture
} TextureImage;													// Structure Name
TextureImage textureTGA[60];

int mx, my, mxz, myz, mch, mchz, mchl, mchr, mchlz, mchrz;

GL_Window*	g_window;													// Window Structure
Keys*		g_keys;														// Keyboard



struct                    // Создание структуры для информации о таймере
{
	__int64       frequency;                 // Частота таймера
	float         resolution;                // Точность таймера
	unsigned long mm_timer_start;            // Стартовое значение мультимедийного таймера
	unsigned long mm_timer_elapsed;          // Прошедшее время мультимедийного таймера
	bool          performance_timer;         // Использовать высокоточный таймер?
	__int64       performance_timer_start;   // Стартовое значение высокоточного таймера
	__int64       performance_timer_elapsed; // Прошедшее время высокоточного таймера
} mytimer;

typedef struct
{
	float c1, c2, c3, x, y, z;
} ggcam;
ggcam cam;

/*
typedef struct
{
	float movup, movleft, movroat, x, y, z, xx, yy, zz, xm, ym, zm, mass, c1, c2, c3, c3z, c3z1;
	int t, needremax, minx, maxx, miny, maxy, minz, maxz;
	ggblock block[MAX_SIZE_AST][MAX_SIZE_AST][MAX_SIZE_AST];
} ggast;
ggast ast[MAX_AST];
*/

TextureImage basetex[100];



typedef struct{
	union
	{
		struct{ float x, y, z; };
		float f[3];
	};
}Vec;

long ti;


GLuint	texture[10];

float fpsla[10];




void circl(float x, float y, float z, float c);
void movveforw(float cam_c, float cam_c1, float cam_c2, float rass, float *cam_x, float *cam_y, float *cam_z);
void movve(float *cam_c, float *cam_c1, float *cam_c2, float x, float y);
void noise_init();


typedef struct{
	int food;
	int block;
	float z[4], zzz[4];//food; base near; 
}struct_point;

typedef struct{
	int t;
	int food;
	float life;
	char mission[100];
	int x, y;
	int x1[15000], y1[15000];
	int ray_p;
	int zx, zy;
	float mark_level_h;
	float mark_level_f;
	int dead_time;
}struct_mur;

struct_point pole[300][300];
struct_mur mur[1000];
char pole_ch[300][300];


void M_upd();
void M_init();






/*

void TimerInit(void)
{
memset(&mytimer, 0, sizeof(mytimer));

if (!QueryPerformanceFrequency((LARGE_INTEGER *) &mytimer.frequency))
{
// Нет высокоточного таймера
mytimer.performance_timer  = FALSE;
mytimer.mm_timer_start  = timeGetTime();
mytimer.resolution  = 1.0f/1000.0f;
mytimer.frequency    = 1000;
mytimer.mm_timer_elapsed  = mytimer.mm_timer_start;
}
else
{
// Высокоточный таймер доступен, используем его вместо мультимедийного таймера
// Взять текущее время и сохранить его в performance_timer_start
QueryPerformanceCounter((LARGE_INTEGER *) &mytimer.performance_timer_start);
mytimer.performance_timer    = TRUE;        // Установить флаг наличия таймера в TRUE
// Вычислить точность таймера, используя частоту
mytimer.resolution    = (float) (((double)1.0f)/((double)mytimer.frequency));
// Присвоить прошедшему времени текущее время
mytimer.performance_timer_elapsed  = mytimer.performance_timer_start;
}
}

float TimerGetTim()           // Взять время в миллисекундах
{
__int64 ztime;                // time содержит 64 бита
__int64 time;                // time содержит 64 бита

if (mytimer.performance_timer) // Есть высокоточный таймер?
{
// Захват текущего значения высокоточного таймера
QueryPerformanceCounter((LARGE_INTEGER *) &time);
// Вернем текущее время минус начальное время, умноженное на точность и 1000 (для миллисекунд)
ztime=mytimer.performance_timer_start;
mytimer.performance_timer_start=time;
return ( (float) ( time - ztime) * mytimer.resolution)*1000.0f;
}
else
{
// Вернем текущее время минус начальное время, умноженное на точность и 1000 (для миллисекунд)
ztime=mytimer.mm_timer_start;
mytimer.mm_timer_start=timeGetTime();
return( (float) ( timeGetTime() - mytimer.mm_timer_start) * mytimer.resolution)*1000.0f;
}
}

*/

GLvoid BuildFont(float heigf)								// Build Our Bitmap Font
{
	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping

	base = glGenLists(96);								// Storage For 96 Characters

	font = CreateFont(heigf,							// Height Of Font
		0,								// Width Of Font
		0,								// Angle Of Escapement
		0,								// Orientation Angle
		FW_BOLD,						// Font Weight
		FALSE,							// Italic
		FALSE,							// Underline
		FALSE,							// Strikeout
		ANSI_CHARSET,					// Character Set Identifier
		OUT_TT_PRECIS,					// Output Precision
		CLIP_DEFAULT_PRECIS,			// Clipping Precision
		ANTIALIASED_QUALITY,			// Output Quality
		FF_DONTCARE | DEFAULT_PITCH,		// Family And Pitch
		L"Courier New");					// Font Name

	oldfont = (HFONT)SelectObject(HDc, font);           // Selects The Font We Want
	wglUseFontBitmaps(HDc, 32, 96, base);				// Builds 96 Characters Starting At Character 32
	SelectObject(HDc, oldfont);							// Selects The Font We Want
	DeleteObject(font);									// Delete The Font
}

GLvoid KillFont(GLvoid)									// Delete The Font List
{
	glDeleteLists(base, 96);							// Delete All 96 Characters
}

GLvoid glPrint(const char *fmt, ...)					// Custom GL "Print" Routine
{
	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

	va_start(ap, fmt);									// Parses The String For Variables
	vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(base - 32);								// Sets The Base Character to 32
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits
}

void TerminateApplication(GL_Window* window)							// Terminate The Application
{
	PostMessage(window->hWnd, WM_QUIT, 0, 0);							// Send A WM_QUIT Message
	g_isProgramLooping = FALSE;											// Stop Looping Of The Program
}

BOOL ChangeScreenResolution(int width, int height, int bitsPerPixel)	// Change The Screen Resolution
{
	DEVMODE dmScreenSettings;											// Device Mode
	ZeroMemory(&dmScreenSettings, sizeof (DEVMODE));					// Make Sure Memory Is Cleared
	dmScreenSettings.dmSize = sizeof (DEVMODE);				// Size Of The Devmode Structure
	dmScreenSettings.dmPelsWidth = width;						// Select Screen Width
	dmScreenSettings.dmPelsHeight = height;						// Select Screen Height
	dmScreenSettings.dmBitsPerPel = bitsPerPixel;					// Select Bits Per Pixel
	dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
	if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
	{
		return FALSE;													// Display Change Failed, Return False
	}
	return TRUE;														// Display Change Was Successful, Return True
}

void ReshapeGL(int width, int height)									// Reshape The Window When It's Moved Or Resized
{
	glViewport(0, 0, (GLsizei)(width), (GLsizei)(height));				// Reset The Current Viewport
	glMatrixMode(GL_PROJECTION);										// Select The Projection Matrix
	glLoadIdentity();													// Reset The Projection Matrix
	gluPerspective(90.0f, (GLfloat)(width) / (GLfloat)(height),			// Calculate The Aspect Ratio Of The Window
		0.1f, 10000.0f);
	glMatrixMode(GL_MODELVIEW);										// Select The Modelview Matrix
	glLoadIdentity();													// Reset The Modelview Matrix
	KillFont();
	BuildFont(-height / 40.0f);
	razokx = width;
	razoky = height;
}

int dvavx(int i)
{
	int r, o = 2;
	for (r = 1; r<i; r++)
		o *= 2;
	return o;
}

float min1(float i, float r)
{
	if (i>r)return r;
	return i;
}

float abs1(float i)
{
	if (i<0)return -i;
	return i;
}

float max1(float i, float r)
{
	if (i>r)return i;
	return r;
}


BOOL CreateWindowGL(GL_Window* window)									// This Code Creates Our OpenGL Window
{
	DWORD windowStyle = WS_OVERLAPPEDWINDOW;							// Define Our Window Style
	DWORD windowExtendedStyle = WS_EX_APPWINDOW;						// Define The Window's Extended Style

	PIXELFORMATDESCRIPTOR pfd =											// pfd Tells Windows How We Want Things To Be
	{
		sizeof (PIXELFORMATDESCRIPTOR),									// Size Of This Pixel Format Descriptor
		1,																// Version Number
		PFD_DRAW_TO_WINDOW |											// Format Must Support Window
		PFD_SUPPORT_OPENGL |											// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,												// Must Support Double Buffering
		PFD_TYPE_RGBA,													// Request An RGBA Format
		window->init.bitsPerPixel,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,												// Color Bits Ignored
		0,																// No Alpha Buffer
		0,																// Shift Bit Ignored
		0,																// No Accumulation Buffer
		0, 0, 0, 0,														// Accumulation Bits Ignored
		16,																// 16Bit Z-Buffer (Depth Buffer)  
		0,																// No Stencil Buffer
		0,																// No Auxiliary Buffer
		PFD_MAIN_PLANE,													// Main Drawing Layer
		0,																// Reserved
		0, 0, 0															// Layer Masks Ignored
	};

	RECT windowRect = { 0, 0, window->init.width, window->init.height };	// Define Our Window Coordinates

	GLuint PixelFormat;													// Will Hold The Selected Pixel Format

	if (window->init.isFullScreen == TRUE)								// Fullscreen Requested, Try Changing Video Modes
	{
		if (ChangeScreenResolution(window->init.width, window->init.height, window->init.bitsPerPixel) == FALSE)
		{
			// Fullscreen Mode Failed.  Run In Windowed Mode Instead
			MessageBox(HWND_DESKTOP, L"Mode Switch Failed.\nRunning In Windowed Mode.", L"Error", MB_OK | MB_ICONEXCLAMATION);
			window->init.isFullScreen = FALSE;							// Set isFullscreen To False (Windowed Mode)
		}
		else															// Otherwise (If Fullscreen Mode Was Successful)
		{
			//ShowCursor (FALSE);											// Turn Off The Cursor
			windowStyle = WS_POPUP;										// Set The WindowStyle To WS_POPUP (Popup Window)
			windowExtendedStyle |= WS_EX_TOPMOST;						// Set The Extended Window Style To WS_EX_TOPMOST
		}																// (Top Window Covering Everything Else)
	}
	else																// If Fullscreen Was Not Selected
	{
		// Adjust Window, Account For Window Borders
		AdjustWindowRectEx(&windowRect, windowStyle, 0, windowExtendedStyle);
	}

	// Create The OpenGL Window
	window->hWnd = CreateWindowEx(windowExtendedStyle,					// Extended Style
		(LPCWSTR)window->init.application->className,	// Class Name
		(LPCWSTR)window->init.title,					// Window Title
		windowStyle,							// Window Style
		0, 0,								// Window X,Y Position
		windowRect.right - windowRect.left,	// Window Width
		windowRect.bottom - windowRect.top,	// Window Height
		HWND_DESKTOP,						// Desktop Is Window's Parent
		0,									// No Menu
		window->init.application->hInstance, // Pass The Window Instance
		window);

	if (window->hWnd == 0)												// Was Window Creation A Success?
	{
		return FALSE;													// If Not Return False
	}

	window->hDC = GetDC(window->hWnd);									// Grab A Device Context For This Window
	if (window->hDC == 0)												// Did We Get A Device Context?
	{
		// Failed
		DestroyWindow(window->hWnd);									// Destroy The Window
		window->hWnd = 0;												// Zero The Window Handle
		return FALSE;													// Return False
	}

	PixelFormat = ChoosePixelFormat(window->hDC, &pfd);				// Find A Compatible Pixel Format
	if (PixelFormat == 0)												// Did We Find A Compatible Format?
	{
		// Failed
		ReleaseDC(window->hWnd, window->hDC);							// Release Our Device Context
		window->hDC = 0;												// Zero The Device Context
		DestroyWindow(window->hWnd);									// Destroy The Window
		window->hWnd = 0;												// Zero The Window Handle
		return FALSE;													// Return False
	}

	if (SetPixelFormat(window->hDC, PixelFormat, &pfd) == FALSE)		// Try To Set The Pixel Format
	{
		// Failed
		ReleaseDC(window->hWnd, window->hDC);							// Release Our Device Context
		window->hDC = 0;												// Zero The Device Context
		DestroyWindow(window->hWnd);									// Destroy The Window
		window->hWnd = 0;												// Zero The Window Handle
		return FALSE;													// Return False
	}

	window->hRC = wglCreateContext(window->hDC);						// Try To Get A Rendering Context
	if (window->hRC == 0)												// Did We Get A Rendering Context?
	{
		// Failed
		ReleaseDC(window->hWnd, window->hDC);							// Release Our Device Context
		window->hDC = 0;												// Zero The Device Context
		DestroyWindow(window->hWnd);									// Destroy The Window
		window->hWnd = 0;												// Zero The Window Handle
		return FALSE;													// Return False
	}

	// Make The Rendering Context Our Current Rendering Context
	if (wglMakeCurrent(window->hDC, window->hRC) == FALSE)
	{
		// Failed
		wglDeleteContext(window->hRC);									// Delete The Rendering Context
		window->hRC = 0;												// Zero The Rendering Context
		ReleaseDC(window->hWnd, window->hDC);							// Release Our Device Context
		window->hDC = 0;												// Zero The Device Context
		DestroyWindow(window->hWnd);									// Destroy The Window
		window->hWnd = 0;												// Zero The Window Handle
		return FALSE;													// Return False
	}

	ShowWindow(window->hWnd, SW_NORMAL);								// Make The Window Visible
	window->isVisible = TRUE;											// Set isVisible To True

	ReshapeGL(window->init.width, window->init.height);				// Reshape Our GL Window

	ZeroMemory(window->keys, sizeof (Keys));							// Clear All Keys

	window->lastTickCount = GetTickCount();							// Get Tick Count




	HDc = window->hDC;


	return TRUE;														// Window Creating Was A Success

}

BOOL DestroyWindowGL(GL_Window* window)								// Destroy The OpenGL Window & Release Resources
{
	if (window->hWnd != 0)												// Does The Window Have A Handle?
	{
		if (window->hDC != 0)											// Does The Window Have A Device Context?
		{
			wglMakeCurrent(window->hDC, 0);							// Set The Current Active Rendering Context To Zero
			if (window->hRC != 0)										// Does The Window Have A Rendering Context?
			{
				wglDeleteContext(window->hRC);							// Release The Rendering Context
				window->hRC = 0;										// Zero The Rendering Context
			}
			ReleaseDC(window->hWnd, window->hDC);						// Release The Device Context
			window->hDC = 0;											// Zero The Device Context
		}
		DestroyWindow(window->hWnd);									// Destroy The Window
		window->hWnd = 0;												// Zero The Window Handle
	}

	if (window->init.isFullScreen)										// Is Window In Fullscreen Mode
	{
		ChangeDisplaySettings(NULL, 0);									// Switch Back To Desktop Resolution
		ShowCursor(TRUE);												// Show The Cursor
	}
	return TRUE;														// Return True
}

bool LoadTGA(TextureImage *texture, char *filename, int fff)				// Loads A TGA File Into Memory
{
	GLubyte		TGAheader[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		// Uncompressed TGA Header
	GLubyte		TGAcompare[12];									// Used To Compare TGA Header
	GLubyte		header[6];										// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;									// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;										// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;											// Temporary Variable
	GLuint		type = GL_RGBA;									// Set The Default GL Mode To RBGA (32 BPP)

	FILE *file = fopen(filename, "rb");							// Open The TGA File

	if (file == NULL ||											// Does File Even Exist?
		fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||	// Are There 12 Bytes To Read?
		memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||	// Does The Header Match What We Want?
		fread(header, 1, sizeof(header), file) != sizeof(header))				// If So Read Next 6 Header Bytes
	{
		if (file == NULL)										// Did The File Even Exist? *Added Jim Strong*
			return FALSE;										// Return False
		else													// Otherwise
		{
			fclose(file);										// If Anything Failed, Close The File
			return FALSE;										// Return False
		}
	}

	texture->width = header[1] * 256 + header[0];				// Determine The TGA Width	(highbyte*256+lowbyte)
	texture->height = header[3] * 256 + header[2];				// Determine The TGA Height	(highbyte*256+lowbyte)

	if (texture->width <= 0 ||									// Is The Width Less Than Or Equal To Zero
		texture->height <= 0 ||									// Is The Height Less Than Or Equal To Zero
		(header[4] != 24 && header[4] != 32))						// Is The TGA 24 or 32 Bit?
	{
		fclose(file);											// If Anything Failed, Close The File
		return FALSE;											// Return False
	}

	texture->bpp = header[4];								// Grab The TGA's Bits Per Pixel (24 or 32)
	bytesPerPixel = texture->bpp / 8;							// Divide By 8 To Get The Bytes Per Pixel
	imageSize = (texture->width)*(texture->height)*bytesPerPixel;	// Calculate The Memory Required For The TGA Data
	//imageSize	=128*128*128;	
	texture->imageData = (GLubyte *)malloc(imageSize);			// Reserve Memory To Hold The TGA Data

	if (texture->imageData == NULL ||								// Does The Storage Memory Exist?
		fread(texture->imageData, 1, imageSize, file) != imageSize)	// Does The Image Size Match The Memory Reserved?
	{
		if (texture->imageData != NULL)							// Was Image Data Loaded
			free(texture->imageData);							// If So, Release The Image Data

		fclose(file);											// Close The File
		return FALSE;											// Return False
	}

	for (GLuint i = 0; i<int(imageSize); i += bytesPerPixel)			// Loop Through The Image Data
	{															// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
		temp = texture->imageData[i];								// Temporarily Store The Value At Image Data 'i'
		texture->imageData[i] = texture->imageData[i + 2];		// Set The 1st Byte To The Value Of The 3rd Byte
		texture->imageData[i + 2] = temp;						// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}

	fclose(file);												// Close The File

	// Build A Texture From The Data
	glGenTextures(1, &texture[0].texID);						// Generate OpenGL texture IDs

	glBindTexture(GL_TEXTURE_2D, texture[0].texID);				// Bind Our Texture
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, fff);	// Linear Filtered
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, fff);	// Linear Filtered

	if (texture[0].bpp == 24)										// Was The TGA 24 Bits
	{
		type = GL_RGB;											// If So Set The 'type' To GL_RGB
	}

	glTexImage2D(GL_TEXTURE_2D, 0, type, texture[0].width, texture[0].height, 0, type, GL_UNSIGNED_BYTE, texture[0].imageData);

	return true;												// Texture Building Went Ok, Return True
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	DWORD				tickCount;										// Used For The Tick Counter

	// Get The Window Context
	GL_Window* window = (GL_Window*)(GetWindowLong(hWnd, GWL_USERDATA));

	switch (uMsg)														// Evaluate Window Message
	{
	case WM_PAINT:													// Window Needs Updating
	{
																		tickCount = GetTickCount();								// Get The Tick Count
																		Update(tickCount - window->lastTickCount);					// Update The Counter
																		window->lastTickCount = tickCount;							// Set Last Count To Current Count
																		Draw();													// Draw Our Scene
																		SwapBuffers(window->hDC);									// Swap Buffers (Double Buffering)
	}
		return 0;														// Return
	case WM_MOUSEMOVE:
		mxz = mx;
		myz = my;
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		break;
	case WM_LBUTTONDOWN:
		mchl = 1;
		break;
	case WM_LBUTTONUP:
		mchl = 0;
		break;
	case WM_RBUTTONDOWN:
		mchr = 1;
		break;
	case WM_RBUTTONUP:
		mchr = 0;
		break;

	case WM_SYSCOMMAND:												// Intercept System Commands
	{
																		switch (wParam)												// Check System Calls
																		{
																		case SC_SCREENSAVE:										// Screensaver Trying To Start?
																		case SC_MONITORPOWER:									// Monitor Trying To Enter Powersave?
																			return 0;												// Prevent From Happening
																		}
																		break;														// Exit
	}
		return 0;														// Return

	case WM_CREATE:													// Window Creation
	{
																		CREATESTRUCT* creation = (CREATESTRUCT*)(lParam);			// Store Window Structure Pointer
																		window = (GL_Window*)(creation->lpCreateParams);
																		SetWindowLong(hWnd, GWL_USERDATA, (LONG)(window));



	}
		return 0;														// Return
	case WM_CLOSE:													// Closing The Window
		TerminateApplication(window);								// Terminate The Application
		return 0;														// Return

	case WM_SIZE:													// Size Action Has Taken Place
		switch (wParam)												// Evaluate Size Action
		{
		case SIZE_MINIMIZED:									// Was Window Minimized?
			window->isVisible = FALSE;							// Set isVisible To False
			return 0;												// Return

		case SIZE_MAXIMIZED:									// Was Window Maximized?
			window->isVisible = TRUE;							// Set isVisible To True
			ReshapeGL(LOWORD(lParam), HIWORD(lParam));		// Reshape Window - LoWord=Width, HiWord=Height
			return 0;												// Return

		case SIZE_RESTORED:										// Was Window Restored?
			window->isVisible = TRUE;							// Set isVisible To True
			ReshapeGL(LOWORD(lParam), HIWORD(lParam));		// Reshape Window - LoWord=Width, HiWord=Height
			return 0;												// Return
		}
		break;															// Break


	case WM_KEYDOWN:												// Update Keyboard Buffers For Keys Pressed
		if ((wParam >= 0) && (wParam <= 255))						// Is Key (wParam) In A Valid Range?
		{
			/*
			if (wParam == 'Q')
			{
				if (submenu == 0)
				{
					submenu = 1;
				}
				else{
					submenu = 0;
				}

			}
			*/
			window->keys->keyDown[wParam] = TRUE;					// Set The Selected Key (wParam) To True
			return 0;												// Return
		}
		break;															// Break

	case WM_KEYUP:													// Update Keyboard Buffers For Keys Released
		if ((wParam >= 0) && (wParam <= 255))						// Is Key (wParam) In A Valid Range?
		{
			window->keys->keyDown[wParam] = FALSE;					// Set The Selected Key (wParam) To False
			return 0;												// Return
		}
		break;															// Break

	case WM_TOGGLEFULLSCREEN:										// Toggle FullScreen Mode On/Off
		g_createFullScreen = (g_createFullScreen == TRUE) ? FALSE : TRUE;
		PostMessage(hWnd, WM_QUIT, 0, 0);
		break;															// Break
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);					// Pass Unhandled Messages To DefWindowProc
}

BOOL RegisterWindowClass(Application* application)						// Register A Window Class For This Application.
{																		// TRUE If Successful
	// Register A Window Class
	WNDCLASSEX windowClass;												// Window Class
	ZeroMemory(&windowClass, sizeof (WNDCLASSEX));						// Make Sure Memory Is Cleared
	windowClass.cbSize = sizeof (WNDCLASSEX);					// Size Of The windowClass Structure
	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraws The Window For Any Movement / Resizing
	windowClass.lpfnWndProc = (WNDPROC)(WindowProc);				// WindowProc Handles Messages
	windowClass.hInstance = application->hInstance;				// Set The Instance
	windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW + 1;				// Class Background Brush Color
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	windowClass.lpszClassName = (LPCWSTR)application->className;				// Sets The Applications Classname
	if (RegisterClassEx(&windowClass) == 0)							// Did Registering The Class Fail?
	{
		// NOTE: Failure, Should Never Happen
		MessageBox(HWND_DESKTOP, L"RegisterClassEx Failed!", L"Error", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;													// Return False (Failure)
	}
	return TRUE;														// Return True (Success)
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int i, r, o, j;
	noise_init();

	Application			application;									// Window Structure
	Keys				keys;											// Key Structure
	BOOL				isMessagePumpActive;							// Message Pump Active?
	MSG					msg;											// Window Message Structure

	// Fill Out Application Data
	application.className = "OpenGL";									// Application Class Name
	application.hInstance = hInstance;									// Application Instance

	// Fill Out Window
	ZeroMemory(&window, sizeof (GL_Window));							// Make Sure Memory Is Zeroed
	window.keys = &keys;								// Window Key Structure
	window.init.application = &application;							// Window Application

	// Window Title
	window.init.title = "Eufloria 2";		// Tutorial Title


	window.init.width = razokx;									// Window Width
	window.init.height = razoky;									// Window Height
	window.init.bitsPerPixel = 32;									// Bits Per Pixel
	window.init.isFullScreen = TRUE;									// Fullscreen? (Set To TRUE)

	ZeroMemory(&keys, sizeof (Keys));									// Zero keys Structure

	// Ask The User If They Want To Start In FullScreen Mode? (Remove These 4 Lines If You Want To Force Fullscreen)
	if (MessageBox(HWND_DESKTOP, L"Would You Like To Run In Fullscreen Mode?", L"Start FullScreen?", MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		window.init.isFullScreen = FALSE;								// If Not, Run In Windowed Mode
	}

	// Register A Class For Our Window To Use
	if (RegisterWindowClass(&application) == FALSE)					// Did Registering A Class Fail?
	{
		// Failure
		MessageBox(HWND_DESKTOP, L"Error Registering Window Class!", L"Error", MB_OK | MB_ICONEXCLAMATION);
		return -1;														// Terminate Application
	}

	g_isProgramLooping = TRUE;											// Program Looping Is Set To TRUE
	g_createFullScreen = window.init.isFullScreen;						// g_createFullScreen Is Set To User Default
	while (g_isProgramLooping)											// Loop Until WM_QUIT Is Received
	{
		// Create A Window
		window.init.isFullScreen = g_createFullScreen;					// Set Init Param Of Window Creation To Fullscreen?
		if (CreateWindowGL(&window) == TRUE)							// Was Window Creation Successful?
		{
			// At This Point We Should Have A Window That Is Setup To Render OpenGL
			if (Initialize(&window, &keys) == FALSE)				
			{
				// Failure
				TerminateApplication(&window);							// Close Window, This Will Handle The Shutdown
			}
			else														// Otherwise (Start The Message Pump)
			{	
				isMessagePumpActive = TRUE;								// Set isMessagePumpActive To TRUE
				while (isMessagePumpActive == TRUE)						// While The Message Pump Is Active
				{
					// Success Creating Window.  Check For Window Messages
					if (PeekMessage(&msg, window.hWnd, 0, 0, PM_REMOVE) != 0)
					{
						// Check For WM_QUIT Message
						if (msg.message != WM_QUIT)						// Is The Message A WM_QUIT Message?
						{
							DispatchMessage(&msg);						// If Not, Dispatch The Message
						}
						else											// Otherwise (If Message Is WM_QUIT)
						{
							isMessagePumpActive = FALSE;				// Terminate The Message Pump
						}
					}
					else												// If There Are No Messages
					{
						if (window.isVisible == FALSE)					// If Window Is Not Visible
						{
							WaitMessage();								// Application Is Minimized Wait For A Message
						}
					}
				}														// Loop While isMessagePumpActive == TRUE
			}															// If ( (...

			// Application Is Finished
			Deinitialize();											// User Defined DeInitialization

			DestroyWindowGL(&window);									// Destroy The Active Window
		}
		else															// If Window Creation Failed
		{
			// Error Creating Window
			MessageBox(HWND_DESKTOP, L"Error Creating OpenGL Window", L"Error", MB_OK | MB_ICONEXCLAMATION);
			g_isProgramLooping = FALSE;									// Terminate The Loop
		}
	}																	// While (isProgramLooping)

	UnregisterClass((LPCWSTR)application.className, application.hInstance);		// UnRegister Window Class
	return 0;
}																		// End Of WinMain()

void ToggleFullscreen(GL_Window* window)								// Toggle Fullscreen/Windowed
{
	PostMessage(window->hWnd, WM_TOGGLEFULLSCREEN, 0, 0);				// Send A WM_TOGGLEFULLSCREEN Message
}

void putknopk(float x, float y, float x1, float y1, char* textinkn)
{
	int i, r, o, j, q, w, a, b, c, d;


	glDisable(GL_TEXTURE_2D);
	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	glVertex3f(x - 1, 1 - (y), -1);
	glVertex3f(x - 1, 1 - (y1), -1);
	glVertex3f(x1 - 1, 1 - (y1), -1);
	glVertex3f(x1 - 1, 1 - (y), -1);
	glEnd();

	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	glVertex3f(x + 0.01 - 1, 1 - (y + 0.01), -1);
	glVertex3f(x + 0.01 - 1, 1 - (y1 - 0.01), -1);
	glVertex3f(x1 - 0.01 - 1, 1 - (y1 - 0.01), -1);
	glVertex3f(x1 - 0.01 - 1, 1 - (y + 0.01), -1);
	glEnd();

	glColor3f(0, 0, 0);
	glRasterPos3f(x + 0.015 - 1, 1 - (y1 + y) / 2 - 0.014, -1);
	glPrint(textinkn);

}

int mxv(float x, float y, float x1, float y1)

{
	if ((float)(razoky / 2.0 - my) / (float)razoky * 2>min1(y, y1))
	if ((float)(razoky / 2.0 - my) / (float)razoky * 2<max1(y, y1))
	if ((float)(mx - razokx / 2.0) / (float)razoky * 2>min1(x, x1))
	if ((float)(mx - razokx / 2.0) / (float)razoky * 2<max1(x, x1))
		return 1;
	return 0;
}

int BuildTexture(char *szPathName, GLuint &texid)						// Load Image And Convert To A Texture
{
	HDC			hdcTemp;												// The DC To Hold Our Bitmap
	HBITMAP		hbmpTemp;												// Holds The Bitmap Temporarily
	IPicture	*pPicture;												// IPicture Interface
	OLECHAR		wszPath[MAX_PATH + 1];									// Full Path To Picture (WCHAR)
	char		szPath[MAX_PATH + 1];	// Full Path To Picture
	TCHAR		szPathDD[MAX_PATH + 1];
	long		lWidth;													// Width In Logical Units
	long		lHeight;												// Height In Logical Units
	long		lWidthPixels;											// Width In Pixels
	long		lHeightPixels;											// Height In Pixels
	GLint		glMaxTexDim;											// Holds Maximum Texture Size

	if (strstr(szPathName, "http://"))									// If PathName Contains http:// Then...
	{
		strcpy(szPath, szPathName);										// Append The PathName To szPath
	}
	else																// Otherwise... We Are Loading From A File
	{
		//GetCurrentDirectory(MAX_PATH, szPathDD);							// Get Our Working Directory
		CharToOem(szPathDD, szPath);
		strcat(szPath, "\\");											// Append "\" After The Working Directory
		strcat(szPath, szPathName);										// Append The PathName
	}

	MultiByteToWideChar(CP_ACP, 0, szPath, -1, wszPath, MAX_PATH);		// Convert From ASCII To Unicode
	HRESULT hr = OleLoadPicturePath(wszPath, 0, 0, 0, IID_IPicture, (void**)&pPicture);

	if (FAILED(hr))														// If Loading Failed
		return FALSE;													// Return False

	hdcTemp = CreateCompatibleDC(GetDC(0));								// Create The Windows Compatible Device Context
	if (!hdcTemp)														// Did Creation Fail?
	{
		pPicture->Release();											// Decrements IPicture Reference Count
		return FALSE;													// Return False (Failure)
	}

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);					// Get Maximum Texture Size Supported

	pPicture->get_Width(&lWidth);										// Get IPicture Width (Convert To Pixels)
	lWidthPixels = MulDiv(lWidth, GetDeviceCaps(hdcTemp, LOGPIXELSX), 2540);
	pPicture->get_Height(&lHeight);										// Get IPicture Height (Convert To Pixels)
	lHeightPixels = MulDiv(lHeight, GetDeviceCaps(hdcTemp, LOGPIXELSY), 2540);

	// Resize Image To Closest Power Of Two
	if (lWidthPixels <= glMaxTexDim) // Is Image Width Less Than Or Equal To Cards Limit
		lWidthPixels = 1 << (int)floor((log((double)lWidthPixels) / log(2.0f)) + 0.5f);
	else  // Otherwise  Set Width To "Max Power Of Two" That The Card Can Handle
		lWidthPixels = glMaxTexDim;

	if (lHeightPixels <= glMaxTexDim) // Is Image Height Greater Than Cards Limit
		lHeightPixels = 1 << (int)floor((log((double)lHeightPixels) / log(2.0f)) + 0.5f);
	else  // Otherwise  Set Height To "Max Power Of Two" That The Card Can Handle
		lHeightPixels = glMaxTexDim;

	//	Create A Temporary Bitmap
	BITMAPINFO	bi = { 0 };												// The Type Of Bitmap We Request
	DWORD		*pBits = 0;												// Pointer To The Bitmap Bits

	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);				// Set Structure Size
	bi.bmiHeader.biBitCount = 32;									// 32 Bit
	bi.bmiHeader.biWidth = lWidthPixels;							// Power Of Two Width
	bi.bmiHeader.biHeight = lHeightPixels;						// Make Image Top Up (Positive Y-Axis)
	bi.bmiHeader.biCompression = BI_RGB;								// RGB Encoding
	bi.bmiHeader.biPlanes = 1;									// 1 Bitplane

	//	Creating A Bitmap This Way Allows Us To Specify Color Depth And Gives Us Imediate Access To The Bits
	hbmpTemp = CreateDIBSection(hdcTemp, &bi, DIB_RGB_COLORS, (void**)&pBits, 0, 0);

	if (!hbmpTemp)														// Did Creation Fail?
	{
		DeleteDC(hdcTemp);												// Delete The Device Context
		pPicture->Release();											// Decrements IPicture Reference Count
		return FALSE;													// Return False (Failure)
	}

	SelectObject(hdcTemp, hbmpTemp);									// Select Handle To Our Temp DC And Our Temp Bitmap Object

	// Render The IPicture On To The Bitmap
	pPicture->Render(hdcTemp, 0, 0, lWidthPixels, lHeightPixels, 0, lHeight, lWidth, -lHeight, 0);

	// Convert From BGR To RGB Format And Add An Alpha Value Of 255
	for (long i = 0; i < lWidthPixels * lHeightPixels; i++)				// Loop Through All Of The Pixels
	{
		BYTE* pPixel = (BYTE*)(&pBits[i]);							// Grab The Current Pixel
		BYTE  temp = pPixel[0];									// Store 1st Color In Temp Variable (Blue)
		pPixel[0] = pPixel[2];									// Move Red Value To Correct Position (1st)
		pPixel[2] = temp;											// Move Temp Value To Correct Blue Position (3rd)
		pPixel[3] = 255;											// Set The Alpha Value To 255
	}

	glGenTextures(1, &texid);											// Create The Texture

	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, texid);								// Bind To The Texture ID
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);		// (Modify This For The Type Of Filtering You Want)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);     // (Modify This For The Type Of Filtering You Want)

	// (Modify This If You Want Mipmaps)
	glTexImage2D(GL_TEXTURE_2D, 0, 3, lWidthPixels, lHeightPixels, 0, GL_RGBA, GL_UNSIGNED_BYTE, pBits);

	DeleteObject(hbmpTemp);												// Delete The Object
	DeleteDC(hdcTemp);													// Delete The Device Context

	pPicture->Release();												// Decrements IPicture Reference Count
	//}else{return FALSE;};
	return TRUE;														// Return True (All Good)
}

BOOL Initialize(GL_Window* window, Keys* keys)							// Any GL Init Code & User Initialiazation Goes Here
{
	char s[100];
	char destination[65];

	int i, r, o, j, q, w;
	g_window = window;												// Window Values
	g_keys = keys;													// Key Values

	M_init();

	//TimerInit();

	glGenTextures(1, &tex_mur);

	LoadTGA(&textureTGA[0], "Data/bmp/0.tga", GL_LINEAR);
	LoadTGA(&textureTGA[6], "Data/bmp/6.tga", GL_LINEAR);

	/*
	for (i = 1; i <= 5; i++)
	{
		strcpy(destination, "Data/bmp/block");
		itoa(i, s, 10);
		strcat(destination, s);
		strcat(destination, "icon.tga");
		LoadTGA(&blockinfo[i].textureTGA, destination, GL_LINEAR);

		strcpy(destination, "Data/bmp/block");
		itoa(i, s, 10);
		strcat(destination, s);
		strcat(destination, "tu0.tga");
		LoadTGA(&blocktex[i][0], destination, GL_LINEAR);

		strcpy(destination, "Data/bmp/block");
		itoa(i, s, 10);
		strcat(destination, s);
		strcat(destination, "tu1.tga");
		LoadTGA(&blocktex[i][1], destination, GL_LINEAR);

		strcpy(destination, "Data/bmp/block");
		itoa(i, s, 10);
		strcat(destination, s);
		strcat(destination, "tu2.tga");
		LoadTGA(&blocktex[i][2], destination, GL_LINEAR);

		strcpy(destination, "Data/bmp/block");
		itoa(i, s, 10);
		strcat(destination, s);
		strcat(destination, "tu3.tga");
		LoadTGA(&blocktex[i][3], destination, GL_LINEAR);

		strcpy(destination, "Data/bmp/block");
		itoa(i, s, 10);
		strcat(destination, s);
		strcat(destination, "tu4.tga");
		LoadTGA(&blocktex[i][4], destination, GL_LINEAR);

		strcpy(destination, "Data/bmp/block");
		itoa(i, s, 10);
		strcat(destination, s);
		strcat(destination, "tu5.tga");
		LoadTGA(&blocktex[i][5], destination, GL_LINEAR);

	}
	*/
	/*
	LoadTGA(&blockinfo[1].textureTGA, "Data/bmp/block1icon.tga", GL_LINEAR);
	LoadTGA(&blocktex[1][0], "Data/bmp/block1tu0.tga", GL_LINEAR);
	LoadTGA(&blocktex[1][1], "Data/bmp/block1tu1.tga", GL_LINEAR);
	LoadTGA(&blocktex[1][2], "Data/bmp/block1tu2.tga", GL_LINEAR);
	LoadTGA(&blocktex[1][3], "Data/bmp/block1tu3.tga", GL_LINEAR);
	LoadTGA(&blocktex[1][4], "Data/bmp/block1tu4.tga", GL_LINEAR);
	LoadTGA(&blocktex[1][5], "Data/bmp/block1tu5.tga", GL_LINEAR);

	LoadTGA(&blockinfo[2].textureTGA, "Data/bmp/block2icon.tga", GL_LINEAR);
	LoadTGA(&blocktex[2][0], "Data/bmp/block2tu0.tga", GL_LINEAR);
	LoadTGA(&blocktex[2][1], "Data/bmp/block2tu1.tga", GL_LINEAR);
	LoadTGA(&blocktex[2][2], "Data/bmp/block2tu2.tga", GL_LINEAR);
	LoadTGA(&blocktex[2][3], "Data/bmp/block2tu3.tga", GL_LINEAR);
	LoadTGA(&blocktex[2][4], "Data/bmp/block2tu4.tga", GL_LINEAR);
	LoadTGA(&blocktex[2][5], "Data/bmp/block2tu5.tga", GL_LINEAR);

	LoadTGA(&blockinfo[3].textureTGA, "Data/bmp/block3icon.tga", GL_LINEAR);
	LoadTGA(&blocktex[3][0], "Data/bmp/block3tu0.tga", GL_LINEAR);
	LoadTGA(&blocktex[3][1], "Data/bmp/block3tu1.tga", GL_LINEAR);
	LoadTGA(&blocktex[3][2], "Data/bmp/block3tu2.tga", GL_LINEAR);
	LoadTGA(&blocktex[3][3], "Data/bmp/block3tu3.tga", GL_LINEAR);
	LoadTGA(&blocktex[3][4], "Data/bmp/block3tu4.tga", GL_LINEAR);
	LoadTGA(&blocktex[3][5], "Data/bmp/block3tu5.tga", GL_LINEAR);

	LoadTGA(&blockinfo[4].textureTGA, "Data/bmp/block4icon.tga", GL_LINEAR);
	*/
	LoadTGA(&basetex[0], "Data/bmp/base0.tga", GL_LINEAR);

	glEnable(GL_TEXTURE_2D);											// Enable Texture Mapping
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);								// Black Background
	glClearDepth(1.0f);												// Depth Buffer Setup
	glDepthFunc(GL_LEQUAL);											// The Type Of Depth Testing
	glEnable(GL_DEPTH_TEST);											// Enable Depth Testing
	glShadeModel(GL_SMOOTH);											// Select Smooth Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);					// Set Perspective Calculations To Most Accurate


	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glAlphaFunc(GL_GREATER, 0.99f);
	glDisable(GL_ALPHA_TEST);






	BuildFont(-razoky / 40.0f);


	return TRUE;
}

void Deinitialize(void)
{
}

bool keys[256];
bool old_keys[256];

void Update(DWORD milliseconds)
{
	int i, r, o, j, q, w, a, b, c, d;

	for (i = 0; i < 256; i++)
	{
		old_keys[i] = keys[i];
		keys[i] = g_keys->keyDown[i];
	}
	if (exitgame == 1)
	{
		TerminateApplication(g_window);
	}
	/*
	if (g_keys->keyDown[VK_ESCAPE])
	{
		if (gamemen == 1)
		{
			baseship = ship[myshipid];
		}
		gamemen = 3;
		//exitgame=1;
	}
	*/
	if (g_keys->keyDown[VK_F1])
	{
		if (!g_createFullScreen)
		{
			window.init.width = GetSystemMetrics(SM_CXSCREEN);
			window.init.height = GetSystemMetrics(SM_CYSCREEN);
		}
		else{
			window.init.width = 150;
			window.init.height = 150;
		}

		ToggleFullscreen(g_window);
	}

	if (g_keys->keyDown['S'])
		movveforw(cam.c2, cam.c1, cam.c3, -0.2, &cam.x, &cam.y, &cam.z);
	if (g_keys->keyDown['W'])
		movveforw(cam.c2, cam.c1, cam.c3, 0.2, &cam.x, &cam.y, &cam.z);

	for (i = 0; i < M_speed;i++)
		M_upd();

	if (keys['Q'] && !old_keys['Q'])
	{
		o = 0;
		while (o == 0)
		{
			i = rand() % 261 + 20;
			r = rand() % 261 + 20;
			if ((i - 150)*(i - 150) + (r - 150)*(r - 150) > 100)
				o = 1;
		}
		pole[i][r].food += 500;
	}
	if (keys['Z'] && !old_keys['Z'])
	{
		M_speed++;
	}
	if (keys['X'] && !old_keys['X'])
	{
		M_speed--;
	}

}

void circl(float x, float y, float z, float c)
{
	int i, r, o, j, q, w, a, b, d;
	float xx, yy, x1, y1, x2, y2, x3, y3, u, v;
	glDisable(GL_TEXTURE_2D);


	glBegin(GL_POLYGON);

	o = 10;
	for (i = 0; i<o; i++)
	{
		xx = ddx(i * 360 / o, c);
		yy = ddy(i * 360 / o, c);
		glVertex3f(x + xx, y, z + yy);
		//		glVertex3f(x+rand()%10,y+rand()%10,z+rand()%10);
	}

	glEnd();


}

float movec(float c, float cnu, float gr)
{

	int i, r, o, j, q, w;
	float u, v, xx, yy;
	xx = c;

	q = 0;
	if (c >= 360) c -= 360;
	if (c  <   0) c += 360;
	if (cnu >= 360) cnu -= 360;
	if (cnu<   0) cnu += 360;

	if (min1(min1(min1(abs1(cnu - c - 360), abs1(c - cnu - 360)),
		min1(abs1(cnu - c), abs1(c - cnu))),
		min1(abs1(cnu - c + 360), abs1(c - cnu + 360))


		)<abs1(gr))
	{
		q = 1; xx = cnu;
	}

	if (q == 0)
	{
		if (abs(cnu - c)<180)
		{
			if (c - cnu>0){ xx = c - gr; }
			else{ xx = c + gr; }
		}
		else{
			if (c  >180){ xx = c + gr; }
			else{ xx = c - gr; }
		}
	}








	if (xx >= 360) xx -= 360;
	if (xx  <   0) xx += 360;
	return xx;

}

void getvec3d(float cam_c, float cam_c1, float cam_c2, float *x, float *y, float *z)
{
	float u, v;

	Vec raycam;
	Vec raycamleft;
	Vec raycamup;

	raycam.x = 1;
	raycam.y = 0;
	raycam.z = 0;

	raycamleft.x = 0;
	raycamleft.y = 1;
	raycamleft.z = 0;

	raycamup.x = 0;
	raycamup.y = 0;
	raycamup.z = -1;

	roate(&raycam.y, &raycam.z, 0, 0, cam_c2);
	roate(&raycamleft.y, &raycamleft.z, 0, 0, cam_c2);
	roate(&raycamup.y, &raycamup.z, 0, 0, cam_c2);

	roate(&raycam.x, &raycam.z, 0, 0, cam_c1);
	roate(&raycamleft.x, &raycamleft.z, 0, 0, cam_c1);
	roate(&raycamup.x, &raycamup.z, 0, 0, cam_c1);

	roate(&raycam.x, &raycam.y, 0, 0, cam_c);
	roate(&raycamleft.x, &raycamleft.y, 0, 0, cam_c);
	roate(&raycamup.x, &raycamup.y, 0, 0, cam_c);

	z[0] = -raycam.x;
	x[0] = raycam.y;
	y[0] = -raycam.z;

}



int SN_perm[512];
void noise_init()
{
	int i; for (i = 0; i < 256; i++) SN_perm[256 + i] = SN_perm[i] = rand() % 256;
}

float fade(float t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float lerp(float t, float a, float b)
{
	return a + t * (b - a);
}

float grad(int hash, float x, float y, float z)
{
	int h = hash & 15;
	float u = h < 8 ? x : y, v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float noise3(float x, float y, float z)
{
	int X = (int)floor(x) & 255,
		Y = (int)floor(y) & 255,
		Z = (int)floor(z) & 255;
	float u, v, w;
	int A, AA, AB, B, BA, BB;
	x -= floor(x); y -= floor(y); z -= floor(z);
	u = fade(x);  v = fade(y);  w = fade(z);
	A = SN_perm[X] + Y;     AA = SN_perm[A] + Z;  AB = SN_perm[A + 1] + Z;
	B = SN_perm[X + 1] + Y; BA = SN_perm[B] + Z;  BB = SN_perm[B + 1] + Z;
	return lerp(w, lerp(v, lerp(u, grad(SN_perm[AA], x, y, z),
		grad(SN_perm[BA], x - 1, y, z)),
		lerp(u, grad(SN_perm[AB], x, y - 1, z),
		grad(SN_perm[BB], x - 1, y - 1, z))),
		lerp(v, lerp(u, grad(SN_perm[AA + 1], x, y, z - 1),
		grad(SN_perm[BA + 1], x - 1, y, z - 1)),
		lerp(u, grad(SN_perm[AB + 1], x, y - 1, z - 1),
		grad(SN_perm[BB + 1], x - 1, y - 1, z - 1))));
}

float getplanetw(float xxx,float yyy,float zzz)
{
	int desem, i, r, o, j, q, w, a, b, c, d, tt1, tt2, tt3, tt4;
	float xx, yy, zz, x1, y1, z1, x2, y2, z2, x3, y3, z3, u, v;
	
	return noise3(xxx/30,yyy/30,zzz/30);
}


void add_mur(int x,int y);

void add_z(int x, int y, float pow,float add_l, int t)
{
	if (pole[x][y].z[t] + add_l < pow)
		pole[x][y].z[t] = pole[x][y].z[t] + add_l;
	else
		if (pole[x][y].z[t]< pow)
			pole[x][y].z[t] = pow;
}

void M_upd()
{
	int i, r, o, j, a, b, c, d, x, y,m[100];
	float u, v, xx[10], yy[10];

	//if (rand() % 4 == 0)
	add_mur(150,150);

	for (i = 0; i < 300; i++)
	for (r = 0; r < 300; r++)
	for (o = 0; o < 4; o++)
	{
		pole[i][r].z[o] *= 0.999;
	}

	for (i = 0; i < 300; i++)
	for (r = 0; r < 300; r++)
	for (o = 0; o < 4; o++)
		pole[i][r].zzz[o] = pole[i][r].z[o];

	for (i = 1; i < 300; i++)
	for (r = 1; r < 300; r++)
	for (o = 0; o < 4; o++)
	{
		v = pole[i][r].z[o] - pole[i - 1][r].z[o];
		pole[i][r].zzz[o] -= v*0.01;
		pole[i - 1][r].zzz[o] += v*0.01;
		v = pole[i][r].z[o] - pole[i][r-1].z[o];
		pole[i][r].zzz[o] -= v*0.01;
		pole[i][r-1].zzz[o] += v*0.01;
	}

	for (i = 0; i < 300; i++)
	for (r = 0; r < 300; r++)
	for (o = 0; o < 4; o++)
		pole[i][r].z[o] = pole[i][r].zzz[o];


	for (i = 0; i < 1000; i++)
	{
		if (mur[i].t == 1)
		{
			if (rand()%8==0)
			{
				//mur[i].zx = rand() % 3 - 1;
				//mur[i].zy = rand() % 3 - 1;
			}
			xx[9] = mur[i].zx;
			yy[9] = mur[i].zy;

			mur[i].dead_time--;

			if (mur[i].x>290)
				mur[i].t = 0;
			if (mur[i].y>290)
				mur[i].t = 0;
			if (mur[i].x<10)
				mur[i].t = 0;
			if (mur[i].y<10)
				mur[i].t = 0;

			if (mur[i].dead_time <= 0)
				mur[i].t = 0;

			mur[i].ray_p++;
			mur[i].x1[mur[i].ray_p] = mur[i].x;
			mur[i].y1[mur[i].ray_p] = mur[i].y;
			if (mur[i].ray_p>14900)
			{
				mur[i].t = 0;
			}

			if (strcmp(mur[i].mission, "base") == 0)
			{
				x = mur[i].x + mur[i].zx;
				y = mur[i].y + mur[i].zy;

				xx[0] = -pole[x][y - 1].z[1] + (rand() % 201 - 100) / 3000.0;
				xx[1] = -pole[x + 1][y].z[1] + (rand() % 201 - 100) / 3000.0;
				xx[2] = -pole[x][y + 1].z[1] + (rand() % 201 - 100) / 3000.0;
				xx[3] = -pole[x - 1][y].z[1] + (rand() % 201 - 100) / 3000.0;
				xx[4] = -3;

				m[0] = 1;
				m[1] = 1;
				m[2] = 1;
				m[3] = 1;

				if (mur[i].ray_p > 30)
				{
					x = mur[i].x1[mur[i].ray_p - 30];
					y = mur[i].y1[mur[i].ray_p - 30];
					u = 0.01+sqrt(float((x - mur[i].x)*(x - mur[i].x) + (y - mur[i].y)*(y - mur[i].y)));
					if (x < mur[i].x)
					if (rand() % 100 <  (mur[i].x - x) / u * 50)
						m[3] = 0;
					if (x > mur[i].x)
					if (rand() % 100 < (x - mur[i].x) / u * 50)
						m[1] = 0;
					if (y < mur[i].y)
					if (rand() % 100 < (mur[i].y - y) / u * 50)
						m[0] = 0;
					if (y > mur[i].y)
					if (rand() % 100 <  (y - mur[i].y) / u * 50)
						m[2] = 0;
				}

				o = 4;
				for (r = 0; r < 4; r++)
				if (m[r])
				if (xx[o] < xx[r])
					o = r;


				if (o == 0)
				{
					mur[i].y--;
				}
				if (o == 1)
				{
					mur[i].x++;
				}
				if (o == 2)
				{
					mur[i].y++;
				}
				if (o == 3)
				{
					mur[i].x--;
				}

				if (pole[mur[i].x][mur[i].y].food > 0)
				{
					pole[mur[i].x][mur[i].y].food--;
					mur[i].food++;
					mur[i].mark_level_f = 1;
					mur[i].mark_level_h = 0;
					strcpy(mur[i].mission, "home");
				}

			}
			else
			if (strcmp(mur[i].mission, "food") == 0)
			{
				x = mur[i].x + mur[i].zx;
				y = mur[i].y + mur[i].zy;

				xx[0] = pole[x][y - 1].z[0] * 0.92 - pole[x][y - 1].z[1] + (rand() % 201 - 100) / 3000.0 + pole[x][y - 1].food * 10;
				xx[1] = pole[x + 1][y].z[0] * 0.92 - pole[x + 1][y].z[1] + (rand() % 201 - 100) / 3000.0 + pole[x + 1][y].food * 10;
				xx[2] = pole[x][y + 1].z[0] * 0.92 - pole[x][y + 1].z[1] + (rand() % 201 - 100) / 3000.0 + pole[x][y + 1].food * 10;
				xx[3] = pole[x - 1][y].z[0] * 0.92 - pole[x - 1][y].z[1] + (rand() % 201 - 100) / 3000.0 + pole[x - 1][y].food * 10;
				xx[4] = -1;

				m[0] = 1;
				m[1] = 1;
				m[2] = 1;
				m[3] = 1;

				if (mur[i].ray_p > 30)
				{
					x = mur[i].x1[mur[i].ray_p - 30];
					y = mur[i].y1[mur[i].ray_p - 30];
					u = 0.01 + sqrt(float((x - mur[i].x)*(x - mur[i].x) + (y - mur[i].y)*(y - mur[i].y)));
					if (x < mur[i].x)
					if (rand() % 100 <  (mur[i].x - x) / u * 50)
						m[3] = 0;
					if (x > mur[i].x)
					if (rand() % 100 < (x - mur[i].x) / u * 50)
						m[1] = 0;
					if (y < mur[i].y)
					if (rand() % 100 < (mur[i].y - y) / u * 50)
						m[0] = 0;
					if (y > mur[i].y)
					if (rand() % 100 <  (y - mur[i].y) / u * 50)
						m[2] = 0;
				}

				o = 4;
				for (r = 0; r < 4; r++)
				if (m[r])
				if (xx[o] < xx[r])
					o = r;

				if (rand() % 5 == 0)
					o = rand() % 4;

				if (rand() % 10)
				{

					for (a = -3; a <= 3; a++)
					for (b = -3; b <= 3; b++)
					if (pole[mur[i].x + a][mur[i].y + b].food > 0)
					{
						if (a < 0)
							o = 3;
						if (a > 0)
							o = 1;
						if (b < 0)
							o = 0;
						if (b > 0)
							o = 2;

					}
				}

				if (o == 0)
				{
					mur[i].y--;
				}
				if (o == 1)
				{
					mur[i].x++;
				}
				if (o == 2)
				{
					mur[i].y++;
				}
				if (o == 3)
				{
					mur[i].x--;
				}

				if (pole[mur[i].x][mur[i].y].food > 0)
				{
					pole[mur[i].x][mur[i].y].food--;
					mur[i].food++;
					mur[i].mark_level_f = 1;
					mur[i].mark_level_h = 0;
					strcpy(mur[i].mission, "home");
				}
			}
			else
			if (strcmp(mur[i].mission, "home") == 0)
			{
				x = mur[i].x + mur[i].zx;
				y = mur[i].y + mur[i].zy;

				xx[0] = -pole[x][y - 1].z[0] * 0.92 + pole[x][y - 1].z[1] + (rand() % 201 - 100) / 3000.0;
				xx[1] = -pole[x + 1][y].z[0] * 0.92 + pole[x + 1][y].z[1] + (rand() % 201 - 100) / 3000.0;
				xx[2] = -pole[x][y + 1].z[0] * 0.92 + pole[x][y + 1].z[1] + (rand() % 201 - 100) / 3000.0;
				xx[3] = -pole[x - 1][y].z[0] * 0.92 + pole[x - 1][y].z[1] + (rand() % 201 - 100) / 3000.0;
				xx[4] = -1;

				o = 4;
				for (r = 0; r < 4; r++)
				if (xx[o] < xx[r])
					o = r;

				mur[i].x = mur[i].x1[mur[i].ray_p - 1];
				mur[i].y = mur[i].y1[mur[i].ray_p - 1];

				for (r = 0; mur[i].x != mur[i].x1[r]
					|| mur[i].y != mur[i].y1[r]; r++);

				mur[i].ray_p = r-1;

				if (mur[i].ray_p<4)
				{
					mur[i].t = 0;
				}

				if ((mur[i].x - 150)*(mur[i].x - 150) + (mur[i].y - 150)*(mur[i].y - 150) < 30)
				{
					mur[i].t = 0;
				}
			}


			mur[i].mark_level_f *= 0.999;
			mur[i].mark_level_h *= 0.999;

			if (strcmp(mur[i].mission, "home") == 0)
			if (mur[i].mark_level_f > 0.001)
				add_z(mur[i].x, mur[i].y, mur[i].mark_level_f, 0.15, 0);

			if (mur[i].mark_level_h > 0.001)
				add_z(mur[i].x, mur[i].y, mur[i].mark_level_h, 0.1, 1);

			mur[i].mark_level_f = pole[mur[i].x][mur[i].y].z[0];
			mur[i].mark_level_h = pole[mur[i].x][mur[i].y].z[1];

		}
	}
}


void M_init()
{
	int i, r, o, j, a, b, c, d;
	float u, v;

	M_speed = 1;
	for (i = 0; i < 1000; i++)
		mur[i].t = 0;

	for (i = 0; i < 300; i++)
	for (r = 0; r < 300; r++)
	{
		pole[i][r].block = 0;
		pole[i][r].food = 0;
		pole[i][r].z[0] = 0;
		pole[i][r].z[1] = 0;
		pole[i][r].z[2] = 0;
		pole[i][r].z[3] = 0;
	}

	add_mur(70, 150);
}

void M_put()
{
	byte *data=(byte*)malloc(3*512*512);
	int i, r,x,y;

	for (i = 0; i < 300; i++)
	for (r = 0; r < 300; r++)
	{
		data[(i + r * 512) * 3 + 0] = pole[i][r].z[0]*255;
		data[(i + r * 512) * 3+1] = 100;
		data[(i + r * 512) * 3 + 2] = pole[i][r].z[1]*255;

		if (pole[i][r].food>0)
		{
			data[(i + r * 512) * 3 + 0] = 0;
			data[(i + r * 512) * 3 + 1] = 255;
			data[(i + r * 512) * 3 + 2] = 255;
		}
		
	}
	for (i = 0; i < 1000; i++)
	if (mur[i].t==1)
	{
		x = mur[i].x;
		y = mur[i].y;
		data[(x + y* 512) * 3 + 0] = 0;
		data[(x + y* 512) * 3 + 1] = 0;
		data[(x + y* 512) * 3 + 2] = 0;
	}

	for (i = 130; i < 170; i++)
		for (r = 130; r < 170; r++)
		{

			if ((i - 150)*(i - 150) + (r - 150)*(r - 150)<50)
			{
				data[(i + r * 512) * 3 + 0] = 150;
				data[(i + r * 512) * 3 + 1] = 50;
				data[(i + r * 512) * 3 + 2] = 0;
			}

		}

	glDeleteTextures(1, &tex_mur);
	glGenTextures(1, &tex_mur);

	glBindTexture(GL_TEXTURE_2D, tex_mur);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, data);


	glColor4f(1, 1, 1, 1);
	glBindTexture(GL_TEXTURE_2D,tex_mur);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0);
	glVertex3f(-(razokx + 4 * 000) / (float)razoky * 10, -(razoky + 4 * 000) / (float)razoky * 10, 10);

	glTexCoord2f(0, 300 / 512.0); 
	glVertex3f(-(razokx + 4 * 000) / (float)razoky * 10, -(razoky - 4 * 300) / (float)razoky * 10, 10);

	glTexCoord2f(300 / 512.0, 300 / 512.0);
	glVertex3f(-(razokx - 4 * 300) / (float)razoky * 10, -(razoky - 4 * 300) / (float)razoky * 10, 10);

	glTexCoord2f(300 / 512.0, 0);
	glVertex3f(-(razokx - 4 * 300) / (float)razoky * 10, -(razoky + 4 * 000) / (float)razoky * 10, 10);

	glEnd();


	free(data);

}
void add_mur(int x,int y)
{
	int i, r, o, j;
	j = -1;
	for (i = 0; i < 1000 && j == -1; i++)
	if (mur[i].t == 0)
		j = i;

	if (j == -1)
		return;


	i = j;
	mur[i].dead_time = 5000;
	mur[i].t = 1;
	mur[i].x = x;
	mur[i].y = y;
	mur[i].zx = rand() % 3 - 1;
	mur[i].zy = rand() % 3 - 1;
	mur[i].ray_p = 0;
	if (rand() % 2)
	{
		strcpy(mur[i].mission, "food");
		mur[i].mark_level_f = 0;
		mur[i].mark_level_h = 0;
	}
	else
	{
		strcpy(mur[i].mission, "base");
		mur[i].mark_level_f = 0;
		mur[i].mark_level_h = 1;
	}

}



void Draw()
{

	int desem, i, r, o, j, q, w, a, b, c, d, tt1, tt2, tt3, tt4;
	float xx, yy, zz, x1, y1, z1, x2, y2, z2, x3, y3, z3, u, v;



	glDisable(GL_TEXTURE_2D);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glClearColor(0.0, 0.0, 0.0, 1);

	ti++;

	mxz = razokx / 2;
	myz = razoky / 2;
	if (((mx - mxz) != 0) || ((my - myz) != 0))
	{
		movve(&cam.c2, &cam.c1, &cam.c3, -(mx - mxz), (my - myz));
		//SetCursorPos(razokx / 2, razoky / 2);
	}


	glPushMatrix();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);

	if (1 == 2)
	{
		i = i;
	}

	cam.c3 = 0;
	cam.c2 = 0;
	cam.c1 = 180;
	//glTranslatef(0,0,-5 );  
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);

	glRotatef(cam.c3, 0, 0, 1);
	glRotatef(cam.c1, 1, 0, 0);
	glRotatef(cam.c2, 0, 1, 0);
	glTranslatef(-cam.x, -cam.y, -cam.z);



	M_put();

	glPopMatrix();

		
	glColor4f(1, 1, 1, 1);
	glRasterPos3f(-razokx/(float)razoky, 0.95, -1);
	glPrint("Q  add food");
	glRasterPos3f(-razokx / (float)razoky, 0.90, -1);
	glPrint("Z  + speed");
	glRasterPos3f(-razokx / (float)razoky, 0.85, -1);
	glPrint("X  - speed");
	glRasterPos3f(-razokx / (float)razoky, 0.80, -1);
	glPrint("speed: %i",M_speed);

	glColor4f(1, 1, 1, 1);
	for(i = 0; i < 40; i++)
	{
		glRasterPos3f(-razokx / (float)razoky, 0.95 - 0.05*i, -1);
		glPrint(out.s[i].data());
	}
	/*

	glDisable(GL_TEXTURE_2D);
	//END Graphics


	glPopMatrix();
	glDisable(GL_DEPTH_TEST);

	glColor4f(1, 0.7, 0.7, 1);
	glBegin(GL_LINES);
	glVertex3f(-0.1, 0, -1);
	glVertex3f(0.1, 0, -1);
	glVertex3f(0, -0.1, -1);
	glVertex3f(0, 0.1, -1);
	glEnd();

	glColor4f(1, 1, 1, 1);
	glRasterPos3f(0, 0, -1);
	//	glPrint("Milliseconds:%f",0.0001);


	*/

	mchz = mch;
	mchlz = mchl;
	mchrz = mchr;


}



void movve(float *cam_c, float *cam_c1, float *cam_c2, float x, float y)
{
	float u, v;

	Vec raycam;
	Vec raycamleft;
	Vec raycamup;

	raycam.x = 1;
	raycam.y = 0;
	raycam.z = 0;

	raycamleft.x = 0;
	raycamleft.y = 1;
	raycamleft.z = 0;

	raycamup.x = 0;
	raycamup.y = 0;
	raycamup.z = -1;

	roate(&raycam.x, &raycam.z, 0, 0, y);
	roate(&raycamleft.x, &raycamleft.z, 0, 0, y);
	roate(&raycamup.x, &raycamup.z, 0, 0, y);

	roate(&raycam.x, &raycam.y, 0, 0, -x);
	roate(&raycamleft.x, &raycamleft.y, 0, 0, -x);
	roate(&raycamup.x, &raycamup.y, 0, 0, -x);
	//sdelal samrrr@mail.ru
	roate(&raycam.y, &raycam.z, 0, 0, cam_c2[0]);
	roate(&raycamleft.y, &raycamleft.z, 0, 0, cam_c2[0]);
	roate(&raycamup.y, &raycamup.z, 0, 0, cam_c2[0]);

	roate(&raycam.x, &raycam.z, 0, 0, cam_c1[0]);
	roate(&raycamleft.x, &raycamleft.z, 0, 0, cam_c1[0]);
	roate(&raycamup.x, &raycamup.z, 0, 0, cam_c1[0]);

	roate(&raycam.x, &raycam.y, 0, 0, cam_c[0]);
	roate(&raycamleft.x, &raycamleft.y, 0, 0, cam_c[0]);
	roate(&raycamup.x, &raycamup.y, 0, 0, cam_c[0]);

	//	camv.x=raycam.y;
	//	camv.y=-raycam.z;
	//	camv.z=-raycam.x;

	/*
	//xyz
	//xzy
	//yxz
	//yzx
	//zxy
	//zyx
	*/

	u = ss(0, 0, raycam.y, raycam.x);
	u = -u;
	cam_c[0] = u;

	roate(&raycam.x, &raycam.y, 0, 0, -u);
	roate(&raycamleft.x, &raycamleft.y, 0, 0, -u);
	roate(&raycamup.x, &raycamup.y, 0, 0, -u);

	u = ss(0, 0, raycam.x, raycam.z);
	u = u + 90 + 180;
	cam_c1[0] = u;

	roate(&raycam.x, &raycam.z, 0, 0, -u);
	roate(&raycamleft.x, &raycamleft.z, 0, 0, -u);
	roate(&raycamup.x, &raycamup.z, 0, 0, -u);

	u = ss(0, 0, raycamup.y, raycamup.z);

	cam_c2[0] = u;

	//roate(&raycam    .y,&raycam    .z,0,0,-u);
	//roate(&raycamleft.y,&raycamleft.z,0,0,-u);
	//roate(&raycamup  .y,&raycamup  .z,0,0,-u);


}

void movveforw(float cam_c, float cam_c1, float cam_c2, float rass, float *cam_x, float *cam_y, float *cam_z)
{
	float u, v;

	Vec raycam;
	Vec raycamleft;
	Vec raycamup;

	raycam.x = 1;
	raycam.y = 0;
	raycam.z = 0;

	raycamleft.x = 0;
	raycamleft.y = 1;
	raycamleft.z = 0;

	raycamup.x = 0;
	raycamup.y = 0;
	raycamup.z = -1;

	roate(&raycam.y, &raycam.z, 0, 0, cam_c2);
	roate(&raycamleft.y, &raycamleft.z, 0, 0, cam_c2);
	roate(&raycamup.y, &raycamup.z, 0, 0, cam_c2);

	roate(&raycam.x, &raycam.z, 0, 0, cam_c1);
	roate(&raycamleft.x, &raycamleft.z, 0, 0, cam_c1);
	roate(&raycamup.x, &raycamup.z, 0, 0, cam_c1);

	roate(&raycam.x, &raycam.y, 0, 0, cam_c);
	roate(&raycamleft.x, &raycamleft.y, 0, 0, cam_c);
	roate(&raycamup.x, &raycamup.y, 0, 0, cam_c);

	cam_z[0] -= raycam.x*rass;
	cam_x[0] += raycam.y*rass;
	cam_y[0] -= raycam.z*rass;



}

