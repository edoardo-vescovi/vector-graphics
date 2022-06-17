/*
  Name: NeHeGL.hpp
  Copyright: basecode from nehe.gamedev.net
  Author: Edoardo Vescovi
  Date: 29/09/10
  Description: window functions
*/

#include <windows.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include "..\GLtiming.hpp"
#include "..\GLavi.hpp"
#include "..\GLtools.hpp"

typedef struct {									//structure for keyboard stuff
  bool keyDown[256];
} Keys;

typedef struct {									//contains information vital to applications
  HINSTANCE hInstance;
  const char* className;
} Application;

typedef struct {									//window creation info
  Application* application;
  char*		   title;
  int		   width;
  int		   height;
  int		   bitsPerPixel;
  bool		   isFullScreen;
} GL_WindowInit;

typedef struct {									//contains information vital to a window
  Keys*			keys;
  HWND			hWnd;
  HDC			hDC;
  HGLRC			hRC;
  GL_WindowInit	init;
  bool			isVisible;
} GL_Window;

void TerminateApplication(GL_Window* window);
void ToggleFullscreen(GL_Window* window);

//declare your extern variables here
extern Timer timer;
extern Video video;
//these are the functions you must provide
bool InitGL(GL_Window* window, Keys* keys);	        //performs initialization every window creation
bool Init();                                        //performs initialization once
void Deinit();							            //performs deinitialization in the end
void Update(double sec, double delta);				//perform motion updates  /////////////Is The Number Of Milliseconds Passed Since The Last Call
void Draw();									    //perform scene drawing
