/*
  Name: GLextension.cpp
  Copyright: basecode from nehe.gamedev.net
  Author: Edoardo Vescovi
  Date: 03/10/10
  Description: get info about your videocard
*/

#include <GL\gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GLextension.hpp"

void getRenderer(char *renderer) {strcat(renderer,(char *)glGetString(GL_RENDERER));}
void getVendor(char *vendor) {strcat(vendor,(char *)glGetString(GL_VENDOR));}
void getVersion(char *version) {strcat(version,(char *)glGetString(GL_VERSION));}

bool isVersionAtLeast(unsigned int maior_number, unsigned int minor_number) {
  char version[256];
  strcpy(version,(char *)glGetString(GL_VERSION));

  unsigned int GL_maior_number;
  unsigned int GL_minor_number;
  sscanf(version,"%u.%u",&GL_maior_number,&GL_minor_number);
  
  if(GL_maior_number>maior_number)
    return 1;
  else {
    if(GL_minor_number>=minor_number)
      return 1;
    else
      return 0;
  }
}

unsigned int extensionListLenght() {return strlen((char *)glGetString(GL_EXTENSIONS))+1;}

unsigned int extensionList(char *list) {
  unsigned int lenght=strlen((char *)glGetString(GL_EXTENSIONS))+1;
  strcpy(list,(char *)glGetString(GL_EXTENSIONS));

  unsigned int n=1;  //there are n-1 spaces
  unsigned int cnt;
  for(cnt=0;cnt<lenght;cnt++) {
    if(list[cnt]==' ')
      n++;
  }
  
  return n;
}

bool isExtensionSupported(char *extension) {
  unsigned int lenght=strlen((char *)glGetString(GL_EXTENSIONS))+1;
  char *list=(char *)malloc(lenght);
  strcpy(list,(char *)glGetString(GL_EXTENSIONS));

  return strstr(list,extension);
}

bool printInfo(char *filename) {
  char *info;                 //hold info
  char *token;                //hold a single extension
  
  FILE *file;
  if(filename)
    file=fopen(filename,"w"); 
  else
    file=fopen("your_videocard_info.txt","w");
  
  if(!file)
    return 0;
  
  info=(char *)malloc(256);
  
  info[0]=0;
  getRenderer(info);
  fprintf(file,"Renderer: %s\n",info);

  info[0]=0;
  getVendor(info);
  fprintf(file,"Vendor:   %s\n",info);  

  info[0]=0;
  getVersion(info);
  fprintf(file,"Version:  %s\n",info);
  
  info=(char *)realloc(info,strlen((char *)glGetString(GL_EXTENSIONS))+1);
  strcpy(info,(char *)glGetString(GL_EXTENSIONS));
  fprintf(file,"\nExtensions supported:");  

  unsigned int cnt=0;
  token=strtok(info," ");
  while(token!=0) {
    cnt++;
    fprintf(file,"\n%.3u %s",cnt,token);
    token=strtok(NULL," ");
  }
  
  free(info);
  fclose(file);
  return 1;
}
