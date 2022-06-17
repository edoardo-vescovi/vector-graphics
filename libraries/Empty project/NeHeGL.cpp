/*
  Name: NeHeGL.cpp
  Copyright: basecode from nehe.gamedev.net
  Author: Edoardo Vescovi
  Date: 29/09/10
  Description: OpenGL window functions
*/

#include <stdio.h>
#include <string.h>
#include "NeHeGL.hpp"

#ifndef CDS_FULLSCREEN
#define CDS_FULLSCREEN 4
#endif

#ifndef WM_TOGGLEFULLSCREEN
#define WM_TOGGLEFULLSCREEN (WM_USER+1)
#endif

//declare your extern variables here
Timer timer;

extern GL_Window *g_window;
extern Keys *g_keys;

static bool g_isProgramLooping;											//window creation loop
static bool g_createFullScreen;											//if TRUE, create fullscreen

void TerminateApplication(GL_Window *window) {							//terminate the application
  PostMessage(window->hWnd, WM_QUIT, 0, 0);
  g_isProgramLooping=FALSE;
}

void ToggleFullscreen(GL_Window *window) {								//toggle fullscreen/windowed
  PostMessage(window->hWnd, WM_TOGGLEFULLSCREEN, 0, 0);
}

void ReshapeGL(int width, int height) {									//reshape the window when it's moved or resized
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (float)(width)/(float)(height),	0.1, 100.0);		
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

bool ChangeScreenResolution(int width, int height, int bitsPerPixel) {	//change the screen resolution
  DEVMODE dmScreenSettings;
  ZeroMemory(&dmScreenSettings, sizeof(DEVMODE));
  dmScreenSettings.dmSize=sizeof(DEVMODE);
  dmScreenSettings.dmPelsWidth=width;
  dmScreenSettings.dmPelsHeight=height;
  dmScreenSettings.dmBitsPerPel=bitsPerPixel;
  dmScreenSettings.dmFields=DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
  
  if(ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
    return FALSE;
  
  return TRUE;
}

bool CreateWindowGL(GL_Window *window) {								//create our OpenGL window
  DWORD windowStyle=WS_OVERLAPPEDWINDOW;
  DWORD windowExtendedStyle=WS_EX_APPWINDOW;

  PIXELFORMATDESCRIPTOR pfd =
  {
 	sizeof(PIXELFORMATDESCRIPTOR),
	1,
	PFD_DRAW_TO_WINDOW |
	PFD_SUPPORT_OPENGL |
	PFD_DOUBLEBUFFER,
	PFD_TYPE_RGBA,
	window->init.bitsPerPixel,
	0, 0, 0, 0, 0, 0,
	0,
	0,
	0,
	0, 0, 0, 0,
	16,
	0,
	0,
	PFD_MAIN_PLANE,
	0,
	0, 0, 0
  };

  RECT windowRect={0, 0, window->init.width, window->init.height};
  unsigned int PixelFormat;
  if(window->init.isFullScreen==TRUE) {
    if(ChangeScreenResolution(window->init.width, window->init.height, window->init.bitsPerPixel)==FALSE) {
  	  MessageBox(HWND_DESKTOP, "Mode switch failed.\nRunning in windowed mode.", "Error", MB_OK | MB_ICONEXCLAMATION);
	  window->init.isFullScreen=FALSE;
	} else {
	  ShowCursor(FALSE);
	  windowStyle=WS_POPUP;
	  windowExtendedStyle|=WS_EX_TOPMOST;
	}
  } else {
	AdjustWindowRectEx(&windowRect, windowStyle, 0, windowExtendedStyle);
  }

  window->hWnd=CreateWindowEx(windowExtendedStyle,
  							  window->init.application->className,
							  window->init.title,
		                      windowStyle,
							  0, 0,
							  windowRect.right - windowRect.left,
							  windowRect.bottom - windowRect.top,
							  HWND_DESKTOP,
							  0,
							  window->init.application->hInstance,
							  window);

  if(window->hWnd==0)
 	return FALSE;

  window->hDC=GetDC(window->hWnd);
  if(window->hDC==0)	{
	DestroyWindow(window->hWnd);
	window->hWnd=0;
	return FALSE;
  }

  PixelFormat=ChoosePixelFormat(window->hDC, &pfd);
  if(PixelFormat==0) {
	ReleaseDC(window->hWnd, window->hDC);
	window->hDC=0;
	DestroyWindow(window->hWnd);
	window->hWnd=0;
	return FALSE;
  }

  if(SetPixelFormat(window->hDC, PixelFormat, &pfd)==FALSE) {
	ReleaseDC(window->hWnd, window->hDC);
	window->hDC=0;
	DestroyWindow(window->hWnd);
	window->hWnd=0;
	return FALSE;
  }

  window->hRC=wglCreateContext (window->hDC);
  if(window->hRC==0) {
	ReleaseDC(window->hWnd, window->hDC);
	window->hDC=0;
	DestroyWindow(window->hWnd);
	window->hWnd=0;
	return FALSE;
  }

  if(wglMakeCurrent(window->hDC, window->hRC)==FALSE) {
	wglDeleteContext(window->hRC);
	window->hRC=0;
	ReleaseDC(window->hWnd, window->hDC);
	window->hDC=0;
	DestroyWindow(window->hWnd);
	window->hWnd=0;
	return FALSE;
  }

  ShowWindow(window->hWnd, SW_NORMAL);
  window->isVisible=TRUE;

  ReshapeGL(window->init.width, window->init.height);

  ZeroMemory(window->keys, sizeof(Keys));

  return TRUE;
}

bool DestroyWindowGL(GL_Window *window)	{							    //destroy the OpenGL window and release resources
  if(window->hWnd!=0) {	
    if(window->hDC!=0) {
      wglMakeCurrent(window->hDC, 0);
	  if(window->hRC!=0) {
		wglDeleteContext(window->hRC);
		window->hRC=0;
      }
	  ReleaseDC(window->hWnd, window->hDC);
	  window->hDC=0;
    }
	DestroyWindow(window->hWnd);
	window->hWnd=0;
  }
  
  if(window->init.isFullScreen) {
	ChangeDisplaySettings(NULL,0);
	ShowCursor(TRUE);
  }	
  return TRUE;
}

//process window message callbacks
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  GL_Window *window=(GL_Window *)(GetWindowLong (hWnd, GWL_USERDATA));
  switch(uMsg) {

    case WM_SYSCOMMAND: {
	  switch(wParam) {
	    case SC_SCREENSAVE:
	    case SC_MONITORPOWER:
	  	  return 0;
	  }
	  break;
    }
	return 0;

   	case WM_CREATE: {
      CREATESTRUCT *creation=(CREATESTRUCT *)(lParam);
	  window=(GL_Window *)(creation->lpCreateParams);
	  SetWindowLong(hWnd, GWL_USERDATA, (LONG)(window));
    }
	return 0;

	case WM_CLOSE:
      TerminateApplication(window);
	return 0;

    case WM_SIZE:
	  switch(wParam) {
  	    case SIZE_MINIMIZED:
	  	  window->isVisible=FALSE;
		return 0;

		case SIZE_MAXIMIZED:
		  window->isVisible=TRUE;
		ReshapeGL(LOWORD (lParam), HIWORD (lParam));
		return 0;

		case SIZE_RESTORED:
		  window->isVisible=TRUE;
		ReshapeGL(LOWORD (lParam), HIWORD (lParam));
		return 0;
      }
    break;

    case WM_KEYDOWN:
	  if((wParam>=0) && (wParam<=255)) {
	    window->keys->keyDown[wParam]=TRUE;
	    return 0;
	  }
    break;

	case WM_KEYUP:
	  if((wParam>=0) && (wParam<=255)) {
	    window->keys->keyDown[wParam]=FALSE;
	    return 0;
      }
    break;

	case WM_TOGGLEFULLSCREEN:
	  g_createFullScreen=(g_createFullScreen==TRUE) ? FALSE : TRUE;
	  PostMessage(hWnd, WM_QUIT, 0, 0);
	break;
  
  }

  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool RegisterWindowClass(Application *application) {				    //register a window class for this application
  WNDCLASSEX windowClass;
  ZeroMemory(&windowClass, sizeof(WNDCLASSEX));
  windowClass.cbSize	    = sizeof(WNDCLASSEX);
  windowClass.style		    = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  windowClass.lpfnWndProc   = (WNDPROC)(WindowProc);
  windowClass.hInstance	    = application->hInstance;
  windowClass.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE);
  windowClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
  windowClass.lpszClassName = application->className;
  if(RegisterClassEx(&windowClass)==0) {
    MessageBox(HWND_DESKTOP, "RegisterClassEx failed!", "Error", MB_OK | MB_ICONEXCLAMATION);
    return FALSE;
  }
	return TRUE;
}

//program entry
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  Application application;
  GL_Window	  window;
  Keys		  keys;
  bool		  isMessagePumpActive;
  MSG		  msg;

  application.className="OpenGL";
  application.hInstance=hInstance;

  ZeroMemory(&window, sizeof(GL_Window));
  window.keys	 		   = &keys;
  window.init.application  = &application;
  window.init.title		   = "progetto vuoto";
  window.init.width		   = 640;
  window.init.height	   = 480;
  window.init.bitsPerPixel = 16;
  window.init.isFullScreen = TRUE;

  ZeroMemory(&keys, sizeof(Keys));

  if(MessageBox(HWND_DESKTOP, "Would you like to run in fullscreen mode?", "Start fullscreen?", MB_YESNO | MB_ICONQUESTION)==IDNO)
	window.init.isFullScreen=FALSE;

  if(RegisterWindowClass(&application)==FALSE) {
    MessageBox(HWND_DESKTOP, "Error registering window class!", "Error", MB_OK | MB_ICONEXCLAMATION);
    return -1;
  }
  
  if(!Init()) {
    MessageBox(HWND_DESKTOP, "User initialization failed!", "Error", MB_OK | MB_ICONEXCLAMATION);  
    return -1;
  }
  
  g_isProgramLooping=TRUE;
  g_createFullScreen=window.init.isFullScreen;

  char str1[37];  //useful to the title
  char str2[11];

  while(g_isProgramLooping) {

	window.init.isFullScreen=g_createFullScreen;

    if(CreateWindowGL(&window)) {

      if(!InitGL(&window, &keys)) {
        MessageBox(HWND_DESKTOP, "OpenGL initialization failed!", "Error", MB_OK | MB_ICONEXCLAMATION);
        TerminateApplication(&window);
      }	else {
		isMessagePumpActive=TRUE;
		while(isMessagePumpActive) {
		  if(PeekMessage(&msg, window.hWnd, 0, 0, PM_REMOVE)!=0) {
			if(msg.message!=WM_QUIT) {
			  DispatchMessage(&msg);
			} else {
			  isMessagePumpActive=FALSE;
			}
		  } else {
			if(!window.isVisible) {
		      WaitMessage();
			} else {
              timer.grabStart();

			  Draw();
  			  SwapBuffers(window.hDC);

              Update(timer.getTime(),0.001);

              strcpy(str1,"Drawing time: ");
              timer.getTimeStrClock(str1);
              sprintf(str2,", FPS: %u",timer.getFPS());
              strcat(str1,str2);
              SetWindowText(g_window->hWnd,str1);
              
     		  timer.wasteUntil(0.001);
  			  timer.updateElapsed();
			}
		  }
		}  //while(isMessagePumpActive)
	  }  //if(!InitGL(&window, &keys))

	  Deinit();
  	  DestroyWindowGL(&window);

    } else {
	  
      MessageBox(HWND_DESKTOP, "Error creating OpenGL window", "Error", MB_OK | MB_ICONEXCLAMATION);
	  g_isProgramLooping=FALSE;
	  
    }

  }  //while(g_isProgramLooping)

  UnregisterClass(application.className, application.hInstance);
  return 0;
}
