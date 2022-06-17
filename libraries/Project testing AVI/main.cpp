/*
  Name: main.cpp
  Copyright: basecode from nehe.gamedev.net
  Author: Edoardo Vescovi
  Date: 29/09/10
  Description: main OpenGL source
*/

#include "NeHeGL.hpp"
#include <stdio.h>
#include "..\GLbmp.hpp"

#ifndef GL_BGR
#define GL_BGR 0x80E0
#endif

GL_Window* g_window;
Keys* g_keys;

GLuint	texture[1];
Image image;
unsigned int scrcnt=0;
bool sp=0;

bool InitGL(GL_Window* window, Keys* keys) {
  g_window=window;
  g_keys=keys;
  
  glClearColor(0.0, 0.0, 0.0, 0.5);
  glClearDepth(1.0);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  if (!loadTexture("nuovo.bmp",&texture[0]))								// Jump To Texture Loading Routine ( NEW )
	return FALSE;									// If Texture Didn't Load Return FALSE

  if(!video.init("face.avi",1))
    return FALSE;

  return TRUE;
}

bool Init(){
  timer.enable();

  return TRUE;
}

void Deinit() {}

void Update(double sec, double delta) {
  if(g_keys->keyDown[VK_ESCAPE])
	TerminateApplication(g_window);

  if(g_keys->keyDown[VK_F1])
	ToggleFullscreen(g_window);
  
  if(g_keys->keyDown['S'] && !sp) {
    screenshot(0,scrcnt,g_window->init.width,g_window->init.height);
    sp=1;
    scrcnt++;
  }
  if(!g_keys->keyDown['S'])
    sp=0;
  
  video.update(sec);
}

void Draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
 /*
  glColor3ub(255,0,0);
  glBegin(GL_QUADS);
    glVertex3i(-20,-20,-1);glVertex3i(20,-20,-1);glVertex3i(20,20,-1);glVertex3i(-20,20,-1);
  glEnd();
 
 */
  gluLookAt(2,0,0,0,0,-7,0,1,0);
  glEnable(GL_TEXTURE_2D);

  video.grabFrame();

  double w=1;
  double h=w*video.ratio();

  glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-w, -h, -5);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(w, -h, -5);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(w, h, -5);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-w, h, -5);
  glEnd();


    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin(GL_QUADS);
  		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  -10.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  -10.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  -10.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  -10.0f);
    glEnd();

  	glDisable(GL_TEXTURE_2D);

}
