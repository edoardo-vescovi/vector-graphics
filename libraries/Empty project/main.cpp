/*
  Name: main.cpp
  Copyright: basecode from nehe.gamedev.net
  Author: Edoardo Vescovi
  Date: 29/09/10
  Description: main OpenGL source
*/

#include "NeHeGL.hpp"

GL_Window *g_window;
Keys *g_keys;

bool Init(){
  timer.enable();

  return TRUE;
}

bool InitGL(GL_Window *window, Keys *keys) {
  g_window=window;
  g_keys=keys;
  
  glClearColor(0.0, 0.0, 0.0, 0.5);
  glClearDepth(1.0);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  return TRUE;
}

void Deinit() {}

void Update(double sec, double delta) {
  if(g_keys->keyDown[VK_ESCAPE])
	TerminateApplication(g_window);

  if(g_keys->keyDown[VK_F1])
	ToggleFullscreen(g_window);
}

void Draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
}
