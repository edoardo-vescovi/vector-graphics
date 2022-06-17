/*
  Name: GLextension.hpp
  Copyright: basecode from nehe.gamedev.net
  Author: Edoardo Vescovi
  Date: 03/10/10
  Description: get info about your videocard
*/

#ifndef _GL_EXTENSION_HPP_
#define	_GL_EXTENSION_HPP_

//call them after "CreateWindowGL"

//grab the renderer, vendor name and version number and append to the string
void getRenderer(char *renderer);
void getVendor(char *vendor);
void getVersion(char *version);
//is your GL version "maior_number.minor_number" or greater?
bool isVersionAtLeast(unsigned int maior_number, unsigned int minor_number);

//use it to allocate enough memory before "extensionList" call (NULL included)
unsigned int extensionListLenght();
//return number of extensions supported and store in "list" space-separated
unsigned int extensionList(char *list);
//check if an extension is supported

bool isExtensionSupported(char *extension);

//print videocard info in a file called "filename"
//if 0 is passed, filename is "your_videocard_info.txt"
bool printInfo(char *filename);

#endif
