/*
  Name: GLbmp.hpp
  Copyright: basecode from nehe.gamedev.net
  Author: Edoardo Vescovi
  Date: 17/09/10
  Description: BMP image loader for OpenGL application
*/

#ifndef _GL_BMP_HPP_
#define	_GL_BMP_HPP_

#ifndef ull
typedef unsigned long long ull;
#endif

#ifndef FILENAME_MAX
#define	FILENAME_MAX 260
#endif

class Image {
  private:
    char filename[FILENAME_MAX];
    unsigned int width;
    unsigned int height;
    unsigned char *data;          //reversed order, BGR format

  public:
    Image();                      //create an empty image
    ~Image();                     //destroy image and release data memory
    bool load(char *Filename);    //read the file and fill the variables
                                  //overwrite a previous image if exists
                                  //if an error occurs, free memory
    void release();               //release data memory
    
    //return image info
    void getFilename(char *str);
    unsigned int getWidth();
    unsigned int getHeight();
    unsigned char *getData();     //hint: use this function instead of
                                  //"getFilename" to check if it's empty
};

/*Simple usage:
Image image;
if(!image.load(...)) {
  //an error occurred
  ...
}*/

#endif
