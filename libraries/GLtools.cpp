/*
  Name: GLtools.cpp
  Copyright: full-written by the author
  Author: Edoardo Vescovi
  Date: 01/11/10
  Description: useful OpenGL tools
*/

#include <GL\gl.h>
#include <stdio.h>
#include <stdlib.h>
#include "GLbmp.hpp"

#ifndef GL_BGR
#define GL_BGR 0x80E0
#endif

#ifndef FILENAME_MAX
#define	FILENAME_MAX 260
#endif

//two useful functions to "screenshot"

inline void putShort(unsigned short s, FILE *file) {
  fprintf(file,"%c%c",s,s>>8);
}

inline void putInt(unsigned int i, FILE *file) {
  fprintf(file,"%c%c%c%c",i,i>>8,i>>16,i>>24);
}

//header functions start here

bool loadTexture(char *filename, unsigned int *texture_id) {
  Image image;
  if(!(image.load(filename)))
    return 0;

  glGenTextures(1,texture_id);  
  glBindTexture(GL_TEXTURE_2D,*texture_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D,0,3,image.getWidth(),image.getHeight(),0,GL_BGR,GL_UNSIGNED_BYTE,image.getData());
  
  return 1;
}

bool screenshot(char *filename, unsigned int n,
                unsigned int width, unsigned int height) {
  if(!(width && height))
    return 0;
  
  //read buffer
  unsigned int bufferSize=width*height*3;
  unsigned char *buffer=(unsigned char *)malloc(bufferSize);
  if(!buffer)
    return 0;
  
  glReadBuffer(GL_FRONT);
  glReadPixels(0,0,width,height,GL_BGR,GL_UNSIGNED_BYTE,buffer);
  
  //create bitmap
  char Filename[FILENAME_MAX];
  if(filename)
    sprintf(Filename,"%s_%u.bmp",filename,n);
  else
    sprintf(Filename,"screenshot_%u.bmp",n);

  FILE *file=fopen(Filename,"wb");
  if(!file) {
    free(buffer);
    return 0;
  }
  
  unsigned int byteWidth=width*3;
  unsigned int bytePad;
  if(byteWidth%4)
    bytePad=4-byteWidth%4;    
  else
    bytePad=0;
  
  unsigned int imageSize=(byteWidth+bytePad)*height;
 
  fprintf(file,"BM");
  putInt(54+imageSize,file);
  putInt(0,file);
  putInt(54,file);
  
  putInt(40,file);
  putInt(width,file);
  putInt(height,file);
  putShort(1,file);
  putShort(24,file);
  putInt(0,file);

  putInt(0,file);
  putInt(0,file);
  putInt(0,file);
  putInt(0,file);
  putInt(0,file);
  
  char zeros[4]={0,0,0,0};
  unsigned int i;
  for(i=0;i<bufferSize;i+=byteWidth) {
    fwrite(buffer+i,byteWidth,1,file);
    fwrite(zeros,bytePad,1,file);
  }
 
  fclose(file);
  free(buffer);
  
  return 1;
}
