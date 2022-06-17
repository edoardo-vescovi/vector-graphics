/*
 *		This Code Was Created By Jeff Molofee 2000
 *		A HUGE Thanks To Fredric Echols For Cleaning Up
 *		And Optimizing This Code, Making It More Flexible!
 *		If You've Found This Code Useful, Please Let Me Know.
 *		Visit My Site At nehe.gamedev.net
 */

#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#include <stdio.h>
#include <time.h>
#include <math.h>

HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

bool	keys[256];			// Array Used For The Keyboard Routine
bool	active=TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen=TRUE;	// Fullscreen Flag Set To Fullscreen Mode By Default

BOOL first_time=TRUE;

GLuint	texture[4];			// Storage For Four Textures

GLfloat xpos=0;             // Camera Position
GLfloat ypos=3;
GLfloat zpos=0;
GLfloat azimut=0;

GLfloat speed=0.18;         // Camera Speed
GLfloat aspeed=0.7;
//LE NORMALI???
//QLC DA GENERALIZ
//elimina disp lists, tutte!
GLuint loop;
GLint xloop,zloop;
GLfloat piover180=0.017453;

GLuint crate;
GLuint bunch;
GLuint sphere;

GLfloat cratepos[10][3];
GLfloat craterot[10];

GLfloat bunchpos[3][3];
GLfloat bunchrot[3];

GLfloat floatpos[10][3];
GLfloat floatrot[10][3];

GLfloat spherepos[4][3];
GLfloat sphereColor[4][3]={{0.8,0,0},{0,0.8,0},{0,0,0.8},{0.8,0.8,0}};

GLfloat	fogColor[4] = {0.5f,0.5f,0.5f,0.5f};     		// Fog Color
BOOL fog=FALSE;
BOOL fp=FALSE;

GLuint mode[3]={GL_FILL,GL_LINE,GL_POINT};
GLuint filter=0;
GLuint mp=FALSE;

GLUquadricObj *quadratic;	// Storage For Our Quadratic Objects ( NEW )

GLuint base;

/*GLfloat LightAmbient[]=		{ 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]=		{ 1.0f, 0.0f, 1.0f, 1.0f };
GLfloat LightPosition[]=	{ 0.0f, 1.0f, 0.0f, 1.0f };*/

GLfloat Time=0;

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

GLfloat my_rand(GLfloat min, GLfloat max) {
    return (GLfloat)rand()/RAND_MAX*(max-min)+min;
}

GLfloat sgn(GLfloat x) {
    return fabs(x)/x;
}

AUX_RGBImageRec *LoadBMP(char *Filename)				// Loads A Bitmap Image
{
	FILE *File=NULL;									// File Handle

	if (!Filename)										// Make Sure A Filename Was Given
	{
		return NULL;									// If Not Return NULL
	}

	File=fopen(Filename,"r");							// Check To See If The File Exists

	if (File)											// Does The File Exist?
	{
		fclose(File);									// Close The Handle
		return auxDIBImageLoad(Filename);				// Load The Bitmap And Return A Pointer
	}

	return NULL;										// If Load Failed Return NULL
}

int LoadGLTextures()									// Load Bitmaps And Convert To Textures
{
	int Status=FALSE;									// Status Indicator

	AUX_RGBImageRec *TextureImage[3];					// Create Storage Space For Four Textures

	memset(TextureImage,0,sizeof(void *)*3);           	// Set The Pointer To NULL

	// Load The Bitmaps, Check For Errors, If Bitmaps Are Not Found Quit
	if ((TextureImage[0]=LoadBMP("Crate.bmp")) &&
        (TextureImage[1]=LoadBMP("Cube.bmp")) &&
        (TextureImage[2]=LoadBMP("Glass.bmp")) &&
        (TextureImage[3]=LoadBMP("Sand.bmp")))
	{
		Status=TRUE;									// Set The Status To TRUE

		glGenTextures(4, &texture[0]);					// Create The Textures

		// Typical Textures Generation Using Data From The Bitmaps
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[1]->sizeX, TextureImage[1]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[1]->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[2]->sizeX, TextureImage[2]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[2]->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		
		glBindTexture(GL_TEXTURE_2D, texture[3]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[3]->sizeX, TextureImage[3]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[3]->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	}

	return Status;										// Return The Status
}

GLvoid BuildListCrate()
{
	crate=glGenLists(1);									// Generate Lists
	glNewList(crate,GL_COMPILE);
        glBegin(GL_QUADS);
			// Top Face
			glNormal3f(0,1,0);
			glTexCoord2f(0,0); glVertex3f(-1,1,1);
			glTexCoord2f(1,0); glVertex3f(1,1,1);
   		    glTexCoord2f(1,1); glVertex3f(1,1,-1);
			glTexCoord2f(0,1); glVertex3f(-1,1,-1);			
 		    // Bottom Face
			glNormal3f(0,-1,0);
			glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
			glTexCoord2f(1,0); glVertex3f(1,-1,-1);
   		    glTexCoord2f(1,1); glVertex3f(1,-1,1);
			glTexCoord2f(0,1); glVertex3f(-1,-1,1);	
            // Front Face
			glNormal3f(0,0,1);
			glTexCoord2f(0,0); glVertex3f(-1,-1,1);
			glTexCoord2f(1,0); glVertex3f(1,-1,1);
   		    glTexCoord2f(1,1); glVertex3f(1,1,1);
			glTexCoord2f(0,1); glVertex3f(-1,1,1);	
			// Back Face
			glNormal3f(0,0,-1);
			glTexCoord2f(0,0); glVertex3f(1,-1,-1);
			glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
   		    glTexCoord2f(1,1); glVertex3f(-1,1,-1);
			glTexCoord2f(0,1); glVertex3f(1,1,-1);
			// Right face
			glNormal3f(1,0,0);
			glTexCoord2f(0,0); glVertex3f(1,-1,1);
			glTexCoord2f(1,0); glVertex3f(1,-1,-1);
   		    glTexCoord2f(1,1); glVertex3f(1,1,-1);
			glTexCoord2f(0,1); glVertex3f(1,1,1);	
			// Left Face
			glNormal3f(-1,0,0);
			glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
			glTexCoord2f(1,0); glVertex3f(-1,-1,1);
   		    glTexCoord2f(1,1); glVertex3f(-1,1,1);
			glTexCoord2f(0,1); glVertex3f(-1,1,-1);	
		glEnd();
	glEndList();
}

GLvoid BuildListBunch()
{
	bunch=glGenLists(1);									// Generate Lists
    glNewList(bunch,GL_COMPILE);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glTranslatef(1,0,1);
        glCallList(crate);

		glTranslatef(0,0,-2);
        glCallList(crate);
        glTranslatef(-2,0,0);

        glCallList(crate);      
        glTranslatef(2,2,0);

     	glBindTexture(GL_TEXTURE_2D, texture[2]);
	    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glColor4f(1,1,1,0.5);
        glCallList(crate);
        //glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glColor3f(1,1,1);
        glTranslatef(-1,-2,-1);
        
	glEndList();
}

GLvoid BuildListSphere()
{
	sphere=glGenLists(1);									// Generate Lists
    glNewList(sphere,GL_COMPILE);
        glBindTexture(GL_TEXTURE_2D, texture[1]);
	    quadratic=gluNewQuadric();							// Create A Pointer To The Quadric Object (Return 0 If No Memory) (NEW)
//	    gluQuadricNormals(quadratic, GLU_SMOOTH);			// Create Smooth Normals (NEW)
	    gluQuadricTexture(quadratic, GL_TRUE);				// Create Texture Coords (NEW)
		gluSphere(quadratic,1,32,32);				// Draw A Sphere With A Radius Of 1 And 16 Longitude And 16 Latitude Segments
	glEndList();
}

GLvoid BuildFont(GLvoid)								// Build Our Bitmap Font
{
	HFONT	font;						// Windows Font ID
	HFONT	oldfont;					// Used For Good House Keeping

	base = glGenLists(256);								// Storage For 96 Characters
	font = CreateFont(	-23,							// Height Of Font
						0,								// Width Of Font
						0,								// Angle Of Escapement
						0,								// Orientation Angle
						FW_BOLD,						// Font Weight
						FALSE,							// Italic
						FALSE,							// Underline
						FALSE,							// Strikeout
						ANSI_CHARSET,					// Character Set Identifier
						OUT_TT_PRECIS,					// Output Precision
						CLIP_DEFAULT_PRECIS,			// Clipping Precision
						ANTIALIASED_QUALITY,			// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
						"Arial");					    // Font Name
	oldfont = (HFONT)SelectObject(hDC, font);		// Selects The Font We Want
	wglUseFontBitmaps(hDC, 0, 255, base);			// Builds 96 Characters Starting At Character 32
	SelectObject(hDC, oldfont);				// Selects The Font We Want
	DeleteObject(font);					// Delete The Font
}

GLvoid KillLists()									    // Delete The Font List
{
    glDeleteLists(crate, 1);
    glDeleteLists(bunch, 1);
    glDeleteLists(sphere, 1);
	glDeleteLists(base, 256);
}

GLvoid glPrint(float x, float y, float z,const char *text)				// Custom GL "Print" Routine
{
	glTranslatef(x, y, z);				   			    // Position Text On The Screen

	if (text == NULL)									// If There's No Text
		return;											// Do Nothing

	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(base);								    // Sets The Base Character to 32
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits
	
    glTranslatef(-x, -y, -z);
}

GLvoid DisplayHelp() {
	
/*	glTranslatef(0,0,-zpos+1);
    glRasterPos2f(1, 1);
    glColor3f(1,1,0);
    if(VK_TAB) {
      glPrint(0,1,0,"Up/down to move");
      glPrint(0,1,0,"Left/right to turn");
      glPrint(0,1,0,"F to en/disable fog mode");
      glPrint(0,1,0,"M to switch polygon mode");
    } else {
      glPrint(0,1,0,"Hold TAB for help");
    }    
	glTranslatef(0,0,zpos-1);
    glColor3f(1,1,1);*/
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
  	if (!LoadGLTextures())								// Jump To Texture Loading Routine ( NEW )
	{
		return FALSE;									// If Texture Didn't Load Return FALSE
	}
	
    BuildListCrate();
	BuildListBunch();
	BuildListSphere();
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
    srand(time(NULL));
    
    if(first_time) {
      for(loop=0;loop<10;loop++) {
        cratepos[loop][0]=my_rand(-28.5,28.5);
        cratepos[loop][1]=1;
        cratepos[loop][2]=my_rand(-28.5,28.5);
        craterot[loop]=my_rand(0,360);
      }
      
      for(loop=0;loop<3;loop++) {
        bunchpos[loop][0]=my_rand(-26,26);
        bunchpos[loop][1]=1;
        bunchpos[loop][2]=my_rand(-26,26);
        bunchrot[loop]=my_rand(0,360);
      }
      
      for(loop=0;loop<10;loop++) {
        floatpos[loop][1]=my_rand(-0.8,-0.1);
        do{
          floatpos[loop][0]=my_rand(-55,55);
          floatpos[loop][2]=my_rand(-55,55);
        } while(fabs(floatpos[loop][0])<35 && fabs(floatpos[loop][2])<35);
        floatrot[loop][0]=my_rand(0,360);
        floatrot[loop][1]=my_rand(0,360);
        floatrot[loop][2]=my_rand(0,360);
      }   
      
      for(loop=0;loop<4;loop++) {
        spherepos[loop][0]=my_rand(-29,29);
        spherepos[loop][1]=1;
        spherepos[loop][2]=my_rand(-29,29);        
      }
      
      first_time=FALSE;
    }

	glFogi(GL_FOG_MODE, GL_EXP);	             		// Fog Mode
	glFogfv(GL_FOG_COLOR, fogColor);					// Set Fog Color
	glFogf(GL_FOG_DENSITY, 0.15f);						// How Dense Will The Fog Be
	glHint(GL_FOG_HINT, GL_DONT_CARE);					// Fog Hint Value
	glFogf(GL_FOG_START, 1.0f);							// Fog Start Depth
	glFogf(GL_FOG_END, 5.0f);							// Fog End Depth

/*	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);	// Position The Light
	glEnable(GL_LIGHT1);								// Enable Light One*/

	//BuildFont();										// Build The Font

    glPointSize(2);
    glLineWidth(1);

	return TRUE;										// Initialization Went OK
}

int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{    
    if(fog)
      glClearColor(fogColor[0],fogColor[1],fogColor[2],fogColor[3]);
    else
      glClearColor(0.67f, 0.8f, 0.94f, 0.5f);				// Black Background
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer

    glLoadIdentity();
	gluLookAt(xpos,ypos,zpos,xpos-sin(azimut*piover180),ypos,zpos-cos(azimut*piover180),0,1,0);

    glEnable(GL_TEXTURE_2D);

	// Floor
   	glBindTexture(GL_TEXTURE_2D, texture[1]);
    glBegin(GL_QUADS);
      glTexCoord2f(0,0);glVertex3f(-30,0,30);
      glTexCoord2f(15,0);glVertex3f(30,0,30);
	  glTexCoord2f(15,15);glVertex3f(30,0,-30);
	  glTexCoord2f(0,15);glVertex3f(-30,0,-30);
    glEnd();

	// Crates
  	glBindTexture(GL_TEXTURE_2D, texture[0]);
    for(loop=0;loop<10;loop++) {
      glPushMatrix();
      glTranslatef(cratepos[loop][0],cratepos[loop][1],cratepos[loop][2]);
      glRotatef(craterot[loop],0,1,0);
   	  glCallList(crate);
      glPopMatrix();
    }
   	
    // Floating Crates
  	glBindTexture(GL_TEXTURE_2D, texture[0]);
    for(loop=0;loop<10;loop++) {
      glPushMatrix();
      glTranslatef(floatpos[loop][0],floatpos[loop][1],floatpos[loop][2]);
      glRotatef(floatrot[loop][0],1,0,0);
      glRotatef(floatrot[loop][1],0,1,0);
      glRotatef(floatrot[loop][2],0,0,1);
      glCallList(crate);
      glPopMatrix();
    }
    
    // Sand
   	glBindTexture(GL_TEXTURE_2D, texture[3]);
    glBegin(GL_QUADS);
      glTexCoord2f(0,0);glVertex3f(-60,-0.1,60);
      glTexCoord2f(30,0);glVertex3f(60,-0.1,60);
	  glTexCoord2f(30,30);glVertex3f(60,-0.1,-60);
	  glTexCoord2f(0,30);glVertex3f(-60,-0.1,-60);
    glEnd();

    // Spheres
    for(loop=0;loop<4;loop++) {
      glPushMatrix();
      glTranslatef(spherepos[loop][0],spherepos[loop][1],spherepos[loop][2]);
      glColor3f(sphereColor[loop][0]+my_rand(-0.2,0.2),
                sphereColor[loop][1]+my_rand(-0.2,0.2),
                sphereColor[loop][2]+my_rand(-0.2,0.2));
      glCallList(sphere);
      glPopMatrix();
    }
    glColor3f(1,1,1);
    glDisable(GL_TEXTURE_2D);

    DisplayHelp();

    glEnable(GL_TEXTURE_2D);

	// Bunches Of Crates
    for(loop=0;loop<3;loop++) {
      glPushMatrix();
      glTranslatef(bunchpos[loop][0],bunchpos[loop][1],bunchpos[loop][2]);
      glRotatef(bunchrot[loop],0,1,0);
   	  glCallList(bunch);
      glLoadIdentity();      	
      glPopMatrix();
    }

    glDisable(GL_TEXTURE_2D);
    


	return TRUE;										// Everything Went OK
}

GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
	
	KillLists();
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/
 
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	
	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}

LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop

	// Ask The User Which Screen Mode They Prefer
	if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=FALSE;							// Windowed Mode
	}

	// Create Our OpenGL Window
	if (!CreateGLWindow("Pavimento",640,480,16,fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if (active)								// Program Active?
			{
				if (keys[VK_ESCAPE])				// Was ESC Pressed?
				{
					done=TRUE;						// ESC Signalled A Quit
				}
				else								// Not Time To Quit, Update Screen
				{
					DrawGLScene();					// Draw The Scene
					SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
				}
			}

			if (keys[VK_F1])						// Is F1 Being Pressed?
			{
				keys[VK_F1]=FALSE;					// If So Make Key FALSE
				KillGLWindow();						// Kill Our Current Window
				fullscreen=!fullscreen;				// Toggle Fullscreen / Windowed Mode
				// Recreate Our OpenGL Window
				if (!CreateGLWindow("Pavimento",640,480,16,fullscreen))
				{
					return 0;						// Quit If Window Was Not Created
				}
			}
			
			if(keys[VK_UP]) {
			  xpos-=speed*sin(azimut*piover180);
			  zpos-=speed*cos(azimut*piover180);
			  if(fabs(xpos)>30) {
			    xpos=30*sgn(xpos);
	    	    PlaySound("Hourglass.wav", NULL, SND_SYNC);
              }
	          if(fabs(zpos)>30) { 
	            zpos=30*sgn(zpos);
           	    PlaySound("Hourglass.wav", NULL, SND_SYNC);
              }
            } 
			if(keys[VK_DOWN]) {
			  xpos+=speed*sin(azimut*piover180);
			  zpos+=speed*cos(azimut*piover180);
			  if(fabs(xpos)>30) {
			    xpos=30*sgn(xpos);
	    	    PlaySound("Hourglass.wav", NULL, SND_SYNC);
              }
	          if(fabs(zpos)>30) { 
	            zpos=30*sgn(zpos);
           	    PlaySound("Hourglass.wav", NULL, SND_SYNC);
              }
            }
			if(keys[VK_LEFT]) {
			  azimut+=aspeed;
			  if(azimut>360)
			    azimut-=360;
            }
			if(keys[VK_RIGHT]) {
			  azimut-=aspeed;
	          if(azimut<0)
			    azimut+=360;
            }
			if (keys['F'] && !fp)
			{
				fp=TRUE;
				fog=!fog;
				if(fog==TRUE)
				  glEnable(GL_FOG);
		        else
		          glDisable(GL_FOG);
			}
			if (!keys['F'])
			{
				fp=FALSE;
			}
			if (keys['M'] && !mp)
			{
				mp=TRUE;
			    filter++;
				if(filter>2)
				  filter-=3;
		        glPolygonMode(GL_FRONT, mode[filter]);
  		        glPolygonMode(GL_BACK, mode[filter]);
			}
			if (!keys['M'])
				mp=FALSE;
        }
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}
