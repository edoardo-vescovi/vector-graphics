/*
  Name: GLbmp.cpp
  Copyright: basecode from nehe.gamedev.net
  Author: Edoardo Vescovi
  Date: 17/09/10
  Description: BMP image loader for OpenGL application
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GLbmp.hpp"

//two useful function to "load"

inline unsigned short getShort(FILE *file) {
  return (unsigned short)getc(file)+((unsigned short)getc(file)<<8);
}

inline unsigned int getInt(FILE *file) {
  return (unsigned int)getc(file)+((unsigned int)getc(file)<<8)+
         ((unsigned int)getc(file)<<16)+((unsigned int)getc(file)<<24);
}

//class member functions start here

Image::Image() {data=0;}  //let know it's empty
Image::~Image() {if(data) free(data);}

bool Image::load(char *Filename) {
  if(data) {
    free(data);
    data=0;
  }
  
  if(!Filename)
    return 0;
  
  strcpy(filename,Filename);
  FILE *file=fopen(filename,"rb");
  if(!file)
    return 0;

  ull size;  //size of the image (bytes), i.e number of pixels*bbp
  ull i;

  if(getc(file)!='B' || getc(file)!='M') {
    fclose(file);
    return 0;
  }

  fseek(file,16,SEEK_CUR);
  width=getInt(file);
  height=getInt(file);

  size=(ull)(width*height*3);

  fseek(file,2,SEEK_CUR);
  if(getShort(file)!=24) {
    fclose(file);
    return 0;
  }
  
  fseek(file,24,SEEK_CUR);
  data=(unsigned char *)malloc(size);
  if(!data) {
    fclose(file);
    return 0;	
  }
  
  if(fread(data,size,1,file)!=1) {
    free(data);
    data=0;
    fclose(file);
    return 0;
  }
  
  fclose(file);
  
  return 1;
}

void Image::release() {
  if(data) {
    free(data);
    data=0;
  }
}

void Image::getFilename(char *str) {
  if(str) strcpy(str,filename);
}

unsigned int Image::getWidth() {return width;}
unsigned int Image::getHeight() {return height;}
unsigned char *Image::getData() {return data;}
