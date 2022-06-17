/*
  Name: GLavi.cpp
  Copyright: basecode from nehe.gamedev.net
  Author: Edoardo Vescovi
  Date: 17/09/10
  Description: AVI video loader for Windows OpenGL application
*/

#include <GL\gl.h>
#include "GLavi.hpp"

#ifndef GL_BGR
#define GL_BGR 0x80E0
#endif

Video::Video() {}

Video::~Video() {
  DeleteObject(hBitmap);
  DrawDibClose(hdd);
  AVIStreamGetFrameClose(pgf);
  AVIStreamRelease(pavi);
  AVIFileExit();
}

bool Video::init(char *Filename, bool Loop) {
  if(!Filename)
    return 0;
  strcpy(filename,Filename);

  //open the AVIFile library
  AVIFileInit();
  //open the AVI stream
  if(AVIStreamOpenFromFile(&pavi,filename,streamtypeVIDEO,0,OF_READ,NULL))
	return 0;
  //read info about the stream and put into psi
  AVIStreamInfo(pavi,&psi,sizeof(psi));

  //fill the class variables
  width=psi.rcFrame.right-psi.rcFrame.left;
  height=psi.rcFrame.bottom-psi.rcFrame.top;
  
  nframe=AVIStreamLength(pavi);
  lenght=(double)AVIStreamSampleToTime(pavi,nframe)/1000.0;
  fps=(double)nframe/lenght;

  currentFrame=0;

  loop=Loop;

  //creates a compatible DC
  hdc=CreateCompatibleDC(0);
  //grab a DC for our dib
  hdd=DrawDibOpen();

  //header information for DrawDibDraw decoding
  BITMAPINFOHEADER  bmih;
  bmih.biSize=sizeof(BITMAPINFOHEADER);
  bmih.biPlanes=1;
  bmih.biBitCount=24;
  bmih.biWidth=width;
  bmih.biHeight=height;
  bmih.biCompression=BI_RGB;
  //creates a dib that we can directly write to
  hBitmap=CreateDIBSection(hdc,(BITMAPINFO*)(&bmih),DIB_RGB_COLORS,(void**)(&res_data),NULL,0);
  //select hBitmap into our DC hdc
  SelectObject(hdc, hBitmap);

  //prepare our program to decompress frames from the file
  //create the PGETFRAME using our request mode (which we need to read frames of data)
  pgf=AVIStreamGetFrameOpen(pavi,NULL);
  if(!pgf)
	return 0;

  //create the resized texture, ready to be drawn in OpenGL
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D,0,3,width,height,0,GL_BGR,GL_UNSIGNED_BYTE,res_data);

  return 1;
}

void Video::grabFrame() {
  //holds the bitmap header information
  LPBITMAPINFOHEADER lpbi;
  //grab the BitmapInfoHeader info for the current frame
  lpbi=(LPBITMAPINFOHEADER)AVIStreamGetFrame(pgf,currentFrame);
  //pointer to texture data
  unsigned char	*tex_data=(unsigned char *)lpbi+(lpbi->biSize)+(lpbi->biClrUsed)*sizeof(RGBQUAD);

  //convert the frame to the requested bitmap format
  DrawDibDraw(hdd,hdc,0,0,width,height,lpbi,tex_data,0,0,width,height,0);

  //update the texture
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexSubImage2D(GL_TEXTURE_2D,0,0,0,width,height,GL_BGR,GL_UNSIGNED_BYTE,res_data);
}

double Video::ratio() {return (double)(height)/(double)(width);}

void Video::update(double sec) {
  if(loop) {
    currentFrame=((unsigned int)(sec*fps))%(nframe);
  } else {
    if(sec<lenght)
      currentFrame=(unsigned int)(sec*fps);
    else
      currentFrame=nframe-1;
  }
}

bool Video::setLoop(bool Loop) {loop=Loop;}
void Video::changeLoop() {loop!=loop;}
bool Video::getLoop() {return loop;}

unsigned int Video::getTexture() {return texture_id;}
