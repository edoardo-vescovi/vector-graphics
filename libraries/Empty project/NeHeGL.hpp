/*
  Name: NeHeGL.hpp
  Copyright: basecode from nehe.gamedev.net
  Author: Edoardo Vescovi
  Date: 29/09/10
  Description: OpenGL window functions
*/

#include <GL\gl.h>
#include <GL\glu.h>
#include <windows.h>
#include "..\GLtiming.hpp"

typedef struct {									//structure for keyboard stuff
  bool keyDown[256];
} Keys;

typedef struct {									//contains information vital to applications
  HINSTANCE hInstance;
  const char *className;
} Application;

typedef struct {									//window creation info
  Application *application;
  char        *title;
  int		  width;
  int		  height;
  int		  bitsPerPixel;
  bool		  isFullScreen;
} GL_WindowInit;

typedef struct {									//contains information vital to a window
  Keys          *keys;
  HWND			hWnd;
  HDC			hDC;
  HGLRC			hRC;
  GL_WindowInit	init;
  bool			isVisible;
} GL_Window;

void TerminateApplication(GL_Window *window);
void ToggleFullscreen(GL_Window *window);

//put your extern variables here
extern Timer timer;
//these are the functions you must provide
bool Init();                                        //perform initialization once
                                                    //and BEFORE "CreateWindowGL"
bool InitGL(GL_Window *window, Keys *keys);	        //perform initialization every window
                                                    //creation, so AFTER "CreateWindowGL"
void Deinit();							            //perform deinitialization in the end
void Update(double sec, double delta);				//perform motion updates
void Draw();									    //perform scene drawing
