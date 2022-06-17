/*
  Name: console.c
  Copyright: basecode from Andrea Scrollavezza
  Author: Edoardo Vescovi
  Date: 03/10/10
  Description: some console functions
*/

#include <conio.h>
#include <string>
#include <windows.h>
#include "console.h"

//things you must declare in the program
unsigned short cur_x=0,cur_y=0;

unsigned short min_x=0;
unsigned short max_x=69;
unsigned short min_y=0;
unsigned short max_y=49;

//hold info about your system "clear" command
char clearcommand[4]="CLS";

//function definitions start here
unsigned short getx() {
  CONSOLE_SCREEN_BUFFER_INFO info;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&info);
  return info.dwCursorPosition.X;
}

unsigned short gety() {
  CONSOLE_SCREEN_BUFFER_INFO info;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&info);
  return info.dwCursorPosition.Y;
}

void gotoxy(unsigned short x, unsigned short y) {
  COORD pos;
  pos.X = x;
  pos.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);     
}

void textcolor(unsigned char fore, unsigned char back) {
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),(back<<4)+fore);
}

void textcolor(char *fore, char *back) {
  unsigned short color;

  if(!strcmp(back,"black"))       color=0;
  if(!strcmp(back,"dblue"))       color=1;
  if(!strcmp(back,"dgreen"))      color=2;
  if(!strcmp(back,"dlight blue")) color=3;
  if(!strcmp(back,"dred"))        color=4;
  if(!strcmp(back,"dpink"))       color=5;
  if(!strcmp(back,"dyellow"))     color=6;
  if(!strcmp(back,"dwhite"))      color=7;

  if(!strcmp(back,"grey"))        color=8;
  if(!strcmp(back,"blue"))        color=9;
  if(!strcmp(back,"green"))       color=10;
  if(!strcmp(back,"light blue"))  color=11;
  if(!strcmp(back,"red"))         color=12;
  if(!strcmp(back,"pink"))        color=13;
  if(!strcmp(back,"yellow"))      color=14;
  if(!strcmp(back,"white"))       color=15;

  color<<=4;

  if(!strcmp(fore,"black"))       color+=0;
  if(!strcmp(fore,"dblue"))       color+=1;
  if(!strcmp(fore,"dgreen"))      color+=2;
  if(!strcmp(fore,"dlight blue")) color+=3;
  if(!strcmp(fore,"dred"))        color+=4;
  if(!strcmp(fore,"dpink"))       color+=5;
  if(!strcmp(fore,"dyellow"))     color+=6;
  if(!strcmp(fore,"dwhite"))      color+=7;

  if(!strcmp(fore,"grey"))        color+=8;
  if(!strcmp(fore,"blue"))        color+=9;
  if(!strcmp(fore,"green"))       color+=10;
  if(!strcmp(fore,"light blue"))  color+=11;
  if(!strcmp(fore,"red"))         color+=12;
  if(!strcmp(fore,"pink"))        color+=13;
  if(!strcmp(fore,"yellow"))      color+=14;
  if(!strcmp(fore,"white"))       color+=15;

  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color);
}

void pseudomouse() {
  char c;
  do {
    gotoxy(cur_x,cur_y);                
    c=getch();
    switch(c) {
      case 'a':
        if(cur_x>min_x)
          cur_x--;
        break;
      case 'd':
        if(cur_x<max_x)
          cur_x++;
        break;
      case 'w':
        if(cur_y>min_y)
          cur_y--;
        break;
      case 's':
        if(cur_y<max_y)
          cur_y++;
        break;
    }
  } while(c!=13);
}

inline void clear() {
  if(!clearcommand) system(clearcommand);
}

inline void printASCII(unsigned char c) {printf("%c",c);}
