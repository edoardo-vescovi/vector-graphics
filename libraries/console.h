/*
  Name: console.h
  Copyright: basecode from Andrea Scrollavezza
  Author: Edoardo Vescovi
  Date: 03/10/10
  Description: some console functions
*/

#ifndef _CONSOLE_H_
#define	_CONSOLE_H_

//return cursor position
unsigned short getx();
unsigned short gety();
//go to specific cursor position
void gotoxy(unsigned short x, unsigned short y);

//Remember console coordinates: (0,0) is top-left corner,
//x axis goes rightward and y axis downwards  

//set foreground/background color
void textcolor(unsigned char fore, unsigned char back);
void textcolor(char *fore, char *back);

/*0-7 colors are less bright then the next 8-15's:
0-8  grey (darker is black)
1-9  blue
2-10 green
3-11 light blue
4-12 red
5-13 pink
6-14 yellow
7-15 white
*/

//move cursor with "wasd" keys, "return key" to select final position
//hint: use before "clear" for a cool effect!
void pseudomouse();

//clear console if "clearcommand" is specified (set "*clearcommand=0" to disable)
inline void clear();

//print whatever ASCII character (0-255)
//hint: run "printASCII" to see the full list of characters you can display
inline void printASCII(unsigned char c);

#endif 
