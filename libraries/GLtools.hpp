/*
  Name: GLtools.hpp
  Copyright: full-written by the author
  Author: Edoardo Vescovi
  Date: 01/11/10
  Description: useful OpenGL tools
*/

#ifndef _GL_TOOLS_HPP_
#define	_GL_TOOLS_HPP_

//remember to add GLbmp library to your project

//load truecolor bmp image "filename" and create a texture with id "texture"
bool loadTexture(char *filename, unsigned int *texture_id);

//take a screenshot of your OpenGL window and save in "filename_n.bmp"
//where "n" is a progressive number
//if 0 is passed, filename is "screenshot_n.bmp"
//call it after the drawing function
bool screenshot(char *filename, unsigned int n,
                unsigned int width, unsigned int height);

#endif
