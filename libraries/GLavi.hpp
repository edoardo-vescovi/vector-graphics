/*
  Name: GLavi.hpp
  Copyright: basecode from nehe.gamedev.net
  Author: Edoardo Vescovi
  Date: 17/09/10
  Description: AVI video loader for Windows OpenGL application
*/

#ifndef _GL_AVI_HPP_
#define	_GL_AVI_HPP_

#include <vfw.h>
//remember to link to libvfw_avi32.a and libvfw_ms32.a

#ifndef FILENAME_MAX
#define	FILENAME_MAX 260
#endif

class Video {
  private:
    //original video info
    char filename[FILENAME_MAX];
    unsigned int width;        //video size
    unsigned int height;
    
    unsigned int nframe;       //number of frames
    //warning: overflow if nframe >= 2^32
    double lenght;             //lenght (sec)
    double fps;                //FPS (frame/sec)

    //our video info
    unsigned int currentFrame; //number of the current frame (from 0 up to nframe-1)
    //hint: you can see if your video is finished (only if "loop" is FALSE)

    bool loop;                 //is it looping?

    PAVISTREAM pavi;	       //pointer to a buffer that receives the stream handle
    AVISTREAMINFO psi;	       //pointer to a structure containing stream info
    PGETFRAME pgf;	           //pointer to a GetFrame object

    HDC hdc;                   //creates a compatible DC
    HDRAWDIB hdd;		       //handle for our Dib
    HBITMAP hBitmap;		   //handle to a device dependant bitmap

    unsigned int texture_id;    //texture id
    unsigned char *res_data;   //pointer to our resized image
    
  public:
    Video();                   //create an empty video
    ~Video();                  //properly close the file and release resources

    //initialization
    bool init(char *Filename, bool Loop);
    //note: call it after "CreateWindowGL"
    //you can reload different videos
    
    void grabFrame();          //select our texture and grab the
                               //current frame from the stream
                               //it does NOT enable 2D texture mapping
    
    double ratio();            //return "height"/"width" ratio
    //hint: use suitable size when drawing

    void update(double sec);   //update "currentFrame"
    //note: you can handle time here
    
    bool setLoop(bool Loop);   //set "loop"
    void changeLoop();         //switch between looping/no-looping mode
    bool getLoop();            //return "loop"
    
    unsigned int getTexture(); //return "texture_id"
    //hint: call it in "glBindTexture" to draw multiple texture of the same video,
    //when you have selected another texture before drawing all of them
};

/*Simple usage:
Video video;
if(!video.init(...)) {
  //an error occurred
  ...
}

//in draw function:
  glEnable(GL_TEXTURE_2D);
  
  video.grabFrame();
  glBegin(GL_QUADS);
	glTexCoord2f(...); glVertex3f(...);
	glTexCoord2f(...); glVertex3f(...);
	glTexCoord2f(...); glVertex3f(...);
	glTexCoord2f(...); glVertex3f(...);
  glEnd();

  glDisable(GL_TEXTURE_2D);*/

#endif
