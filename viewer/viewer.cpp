//
// This code was created by Jeff Molofee '99
// (ported to SDL by Sam Lantinga '2000)
//
// If you've found this code useful, please let me know.
//
// Visit me at www.demonews.com/hosted/nehe 
//
// Code modifed by Frederic Rodrigo 2005 as viewer of object loaded by OpenCity
/*
    TODO:
    - done a better command ligne parser
    - save in other format than bmp
    - compute the initial zoom value (for big object)
*/

#include "modelloader.h"
#include "model.h"
#include "texture.h"
Model *model;

/* floats for x rotation, y rotation, z rotation */
float xrot=30, yrot=30, zrot=0, zoom=3;


/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void InitGL(int Width, int Height)	        // We call this right after our OpenGL window is created.
{
	glViewport(0, 0, Width, Height);
	glEnable(GL_TEXTURE_2D);				// Enable Texture Mapping
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
//	glBlendFunc( GL_ONE, GL_ZERO );
	glClearColor(0.5f, 0.5f, 0.8f, 1.0f);	// Clear The Background Color To Blue
	glClearDepth(1.0);						// Enables Clearing Of The Depth Buffer
	glDepthFunc(GL_LESS);					// The Type Of Depth Test To Do
	glEnable(GL_DEPTH_TEST);				// Enables Depth Testing
//	glDepthMask(GL_FALSE);					// Disable depth buffer writing
	glShadeModel(GL_SMOOTH);				// Enables Smooth Color Shading
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();				// Reset The Projection Matrix

	gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window

	glMatrixMode(GL_MODELVIEW);

//	glEnable( GL_LIGHTING );
//	glColor3f ( 1.0f, 0.0f, 0.0f ) ;
//	glColorMaterial ( GL_FRONT_AND_BACK, GL_EMISSION ) ;
	glEnable ( GL_COLOR_MATERIAL ) ;
}


/* The main drawing function. */
void DrawGLScene()
{
	OC_BYTE tabY [] = { 0, 0, 0, 0};

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
	glLoadIdentity();				// Reset The View
	
	glTranslatef(0.0f,0.0f,-zoom);              // move 5 units into the screen.
	
	glRotatef(xrot,1.0f,0.0f,0.0f);		// Rotate On The X Axis
	glRotatef(yrot,0.0f,1.0f,0.0f);		// Rotate On The Y Axis
	glRotatef(zrot,0.0f,0.0f,1.0f);		// Rotate On The Z Axis

	model->DisplayList(-.5,-.5,tabY);
	
	// swap buffers to display, since we're double buffered.
	SDL_GL_SwapBuffers();
}


void screenshoot( char *name )
{
	glPushClientAttrib( GL_CLIENT_PIXEL_STORE_BIT ); // save context

	// make screenshoot file name
	char* file = new char[ strlen(name)+4+1 ];
	sprintf( file, "%s.bmp", name );

	int w = 640;
	int h = 480;

	// Build the screen shot surface
	unsigned int* someBuffer = new unsigned int[ w * h * 3 ];
	SDL_Surface *shoot = SDL_CreateRGBSurfaceFrom( someBuffer, w, h, 3*8, 3*w, 0, 0, 0, 0 );

	// set screenshot context
	glPixelStorei( GL_PACK_ROW_LENGTH, 0 );
	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	glReadPixels( 0, 0, w, h, GL_BGR, GL_UNSIGNED_BYTE, someBuffer );

	if( shoot != NULL )
	{
		SDL_Surface *shootMirorred = Texture::HorizontalMirror( shoot );
		if( shootMirorred != NULL && SDL_SaveBMP( shootMirorred, file ) == 0 )
			printf( "Capture saved under %s\n", file );
		else
			printf( "Failing save capture under %s\n", file );
		if( shootMirorred != NULL )
			SDL_FreeSurface( shootMirorred );
	}
	else
		printf( "Failing take a shoot\n." );

	SDL_FreeSurface( shoot );

	glPopClientAttrib(); // restor context
	delete [] file;
	delete [] someBuffer;
}


int main(int argc, char **argv)
{
	if( argc == 1 ) {
		printf( "Usage: %s [s] model.ac\n", argv[0] );
		return 0;
	}
	
	char *modelFile = NULL;
	bool shoot = false;
	
	if( argc == 2 ) {
		modelFile = argv[1];
	}
	
	if( argc == 3 ) {
		shoot = true;
		modelFile = argv[2];
	}
	
	/* Initialize SDL for video output */
	if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0 ) {
		fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}
	
	/* Create a 640x480 OpenGL screen */
	if ( SDL_SetVideoMode(640, 480, 0, SDL_OPENGLBLIT | SDL_HWSURFACE | SDL_DOUBLEBUF) == NULL ) {
		fprintf(stderr, "Unable to create OpenGL screen: %s\n", SDL_GetError());
		SDL_Quit();
		exit(2);
	}
	
	/* Loop, drawing and checking events */
	InitGL(640, 480);

	model = ModelLoader::Load( modelFile );
	
	DrawGLScene();
	
	if( shoot )	{
		screenshoot( modelFile );
	}
	else {
		int done = 0;
		while ( ! done ) {
			bool redraw = false;
			
			SDL_Event event;
			while ( SDL_PollEvent(&event) ) {
				switch( event.type ) {
				case SDL_QUIT: done = 1; break;
				case SDL_KEYDOWN:
					switch( event.key.keysym.sym ) {
					case SDLK_ESCAPE: done = 1; break;
					case SDLK_SPACE: redraw = true; break;
					case SDLK_UP: xrot -= 15; redraw = true; break;
					case SDLK_DOWN: xrot += 15; redraw = true; break;
					case SDLK_LEFT: yrot -= 15; redraw = true; break;
					case SDLK_RIGHT: yrot += 15; redraw = true; break;
					case SDLK_PAGEUP: zoom -= 1; redraw = true; break;
					case SDLK_PAGEDOWN: zoom += 1; redraw = true; break;
					case SDLK_l: zoom=3; yrot=45; xrot=30; redraw = true; break;
					case SDLK_m: zoom=3; yrot=45+90; xrot=30; redraw = true; break;
					case SDLK_p: zoom=3; yrot=45+180; xrot=30; redraw = true; break;
					case SDLK_o: zoom=3; yrot=45+270; xrot=30; redraw = true; break;
					// Chose the wireframe polygon mode
					case SDLK_f: glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); redraw=true; break;
					// Chose the textured polygon mode
					case SDLK_t: glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ); redraw=true; break;
					case SDLK_s: screenshoot( modelFile ); break;
					default: ;
					};
					break;
					case SDL_VIDEOEXPOSE:
					redraw = true; break;
				default: ;
				}
			}
			
			if( redraw == true ) {
				DrawGLScene();
			}
			
			SDL_Delay( 50 );
		}
	
	} // shoot
	
	SDL_Quit();
	return 1;
}


string ocHomeDirPrefix( const string & s )
{
	return s;
}
