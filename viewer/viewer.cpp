//
// This code was created by Jeff Molofee '99
// (ported to SDL by Sam Lantinga '2000)
//
// If you've found this code useful, please let me know.
//
// Visit me at www.demonews.com/hosted/nehe 
//
// Code modifed by Frederic Rodrigo 2005-2007 as viewer of object loaded by OpenCity
/*
    TODO:
    - add geometry commands line argument
    - improve help display: key
    - mine type for .ac
*/

// Useful enumerations
#include "opencity_direction.h"
#include "opencity_structure_type.h"

// OpenCity headers
#include "buildinglayer.h"
#include "conf.h"
#include "macros.h"
#include "modelloader.h"
#include "model.h"
#include "structure.h"
#include "texture.h"
#include "property.h"

// Standard headers
#include <string>

// Libraries headers
#include "pngfuncs.h"
#include "SimpleOpt.h"			// Simple command line argument parser


int W = 640;
int H = 480;

/* floats for x rotation, y rotation, z rotation */
float xrot=30, yrot=30, zrot=0, zoom=3;


/* A general OpenGL initialization function.  Sets all of the initial parameters. */
int initGL( const int width, const int height )	        // We call this right after our OpenGL window is created.
{
	glViewport(0, 0, width, height);
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

	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window

	glMatrixMode(GL_MODELVIEW);

//	glEnable( GL_LIGHTING );
//	glColor3f ( 1.0f, 0.0f, 0.0f ) ;
//	glColorMaterial ( GL_FRONT_AND_BACK, GL_EMISSION ) ;
	glEnable ( GL_COLOR_MATERIAL ) ;

	return 0;
}


/* The main drawing function. */
void DrawGLScene( float width, float length, float height, const Model *model )
{
	signed char tabY [] = { 0, 0, 0, 0};

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
	glLoadIdentity();				// Reset The View

	glTranslatef(0.0f,0.0f,-zoom);              // move 5 units into the screen.

	glRotatef(xrot,1.0f,0.0f,0.0f);		// Rotate On The X Axis
	glRotatef(yrot,0.0f,1.0f,0.0f);		// Rotate On The Y Axis
	glRotatef(zrot,0.0f,0.0f,1.0f);		// Rotate On The Z Axis

	model->DisplayList( -width/2, -length/2, tabY );

	// swap buffers to display, since we're double buffered.
	SDL_GL_SwapBuffers();
}


/* FIXME : Not sure about portability of this function */
void screenshot( const string name, int w, int h )
{
	glPushClientAttrib( GL_CLIENT_PIXEL_STORE_BIT ); // save context

	// make screenshot file name
	string file = name.substr( 0, name.length()-3 ) + "_screenshot.png";

	// Build the screen shot surface
	unsigned int* someBuffer = new unsigned int[ w * h * 4 ];

	Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000; gmask = 0x00ff0000; bmask = 0x0000ff00; amask = 0x000000ff;
#else
	rmask = 0x000000ff; gmask = 0x0000ff00; bmask = 0x00ff0000; amask = 0xff000000;
#endif

	SDL_Surface *shot = SDL_CreateRGBSurfaceFrom( someBuffer, w, h, 8*4, w*4, rmask, gmask, bmask, amask );
//	SDL_Surface *shot = SDL_CreateRGBSurfaceFrom( someBuffer, w, h, 8*4, w*4, 0, 0, 0, 0 );

	// set screenshot context
	glPixelStorei( GL_PACK_ROW_LENGTH, 0 );
	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	glReadPixels( 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, someBuffer );

	if( shot != NULL )
	{
		SDL_Surface *shotMirorred = Texture::HorizontalMirror( shot );
		if( shotMirorred != NULL )
		{
			if( png_save_surface( file.c_str(), shotMirorred ) == 0 )
			{
				OPENCITY_INFO( "Capture saved under \"" << file << "\"" );
			}
			else
			{
				OPENCITY_ERROR( "Failing save capture under \"" << file << "\"" );
			}
			SDL_FreeSurface( shotMirorred );
		}
		else
		{
			OPENCITY_ERROR( "Failing working whith capture for \"" << file << "\"" );
		}

		SDL_FreeSurface( shot );
	}
	else
	{
		OPENCITY_ERROR( "Failling take a shot." );
	}

	glPopClientAttrib(); // restor context
	delete [] someBuffer;
}


Conf *loadConf( const string &acFile )
{
	string confFile = acFile.substr( 0, acFile.length()-3 ) + ".conf";

	Conf* pConf = new Conf();

        int openStatus = pConf->Open( confFile );

        if( openStatus != OC_ERR_FREE )
	{
	    OPENCITY_DEBUG( "fail load confFile = " << confFile );
    	    delete pConf;
	    pConf = NULL;
	}
	else
	{
	    OPENCITY_DEBUG( "load confFile = " << confFile );
	}

    return pConf;
}

Property *loadProperty( const string &acFile )
{
	string propertyFile = acFile.substr( 0, acFile.length()-3 ) + ".xml";
	Property* pProperty = Property::LoadProperties( propertyFile );
	OPENCITY_DEBUG( "load propertyFile = " << propertyFile );
    return pProperty;
}

string parseArgLine( int argc, char **argv, bool *shot )
{
	enum {
		OPT_SCREENSHOT,
		OPT_HELP
	};

	CSimpleOpt::SOption g_rgOptions[] = {
		{ OPT_SCREENSHOT,	(char*)"--screenshot",		SO_NONE	},
		{ OPT_SCREENSHOT,	(char*)"-s",				SO_NONE	},
		{ OPT_HELP,			(char*)"--help",			SO_NONE	},
		{ OPT_HELP,			(char*)"-h",				SO_NONE	},
		SO_END_OF_OPTIONS // END
	};

	CSimpleOpt args(argc, argv, g_rgOptions, SO_O_EXACT | SO_O_NOSLASH | SO_O_SHORTARG | SO_O_CLUMP );

	while (args.Next()) {
		switch (args.LastError()) {
			case SO_OPT_INVALID:
				cout << "<OPTION> " << args.OptionText() << " unrecognized" << endl;
				break;
			case SO_OPT_MULTIPLE:
				cout << "<OPTION> " << args.OptionText() << " matched multiple options" << endl;
				break;
			case SO_ARG_INVALID:
				cout << "<OPTION> " << args.OptionText() << " does not accept argument" << endl;
				break;
			case SO_ARG_INVALID_TYPE:
				cout << "<OPTION> " << args.OptionText() << " has an invalid argument format" << endl;
				break;
			case SO_ARG_MISSING:
				cout << "<OPTION> " << args.OptionText() << " require an argument" << endl;
				break;
			case SO_ARG_INVALID_DATA:
            	cout << "<OPTION> " << args.OptionText() << " has an invalid argument data" << endl;
				break;
			case SO_SUCCESS:
				break;
		}

		switch (args.LastError()) {
			case SO_SUCCESS:
				cout << "<OPTION> " << args.OptionText() << " detected" << endl;
				break;
			default:
				cout << "try " << argv[0] << " --help for usage information" << endl;
				exit( -1 );
				break;
		}

		switch (args.OptionId()) {

			case OPT_SCREENSHOT:
				*shot = true;
				break;

			case OPT_HELP:
				cout << "Usage: " << argv[0]
					<< " [-s|--screenshot] model.ac" << endl << endl;
				exit( -1 );
				break;
		}
	}

	if( args.FileCount() >= 1 ) {
		return args.File(0);
	}
	else {
		return "";
	}
}


int initDisplay( const uint w, const uint h )
{
	/* Create a OpenGL screen */
	if ( SDL_SetVideoMode( w, h, 24, SDL_OPENGLBLIT | SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE ) == NULL )
	{
		fprintf( stderr, "Unable to create OpenGL screen: %s\n", SDL_GetError() );
		SDL_Quit();
		return -1;
	}

	return initGL( w, h );
}


Model *loadModel( const string &modelFile, float *width, float *length, float *height )
{
	OPENCITY_INFO( "Open model \"" << modelFile << "\"" );
	Model *model = ModelLoader::Load( modelFile );

	if( model != NULL )
	{
		*width = 1;
		*length = 1;
		*height = 1;

		Property *pProperty = loadProperty( modelFile );
		if( pProperty != NULL )
		{
			*width = pProperty->uiWidth;
			*length = pProperty->uiLength;
			*height = pProperty->fHeight;
			OPENCITY_DEBUG( "Model : " << *width << "x" << *length << "x" << *height );
			delete pProperty;
		}

		// heuristic
	    	zoom = ( (*width) + (*length) )/2*1.5 + .5;
	    	OPENCITY_DEBUG( "Zoom : " << zoom );
		if( zoom < (*height) * 3 )
		{
		        zoom = (*height) * 3;
		}

		return model;
	}
	else
	{
		return NULL;
	}
}


int main( int argc, char **argv )
{
	bool shot = false;
	string modelFile = parseArgLine( argc, argv, &shot );

	if( modelFile == "" )
	{
		exit( 0 );
	}

	/* Initialize SDL for video output */
	if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0 )
	{
		fprintf( stderr, "Unable to initialize SDL: %s\n", SDL_GetError() );
		return -1;
	}

	int isInitDisplay = initDisplay( W, H );
	if( isInitDisplay < 0 )
	{
		exit( isInitDisplay );
	}

	float width, length, height;
	Model *model = loadModel( modelFile, &width, &length, &height );
	if( model == NULL )
	{
		OPENCITY_ERROR( "Can't load model from file : \"" << modelFile << "\"" );
		exit( -3 );
	}

	DrawGLScene( width, length, height, model );
	
	if( shot )
	{
		screenshot( modelFile, W, H );
	}
	else
	{
		int done = 0;
		while ( ! done )
		{
			bool redraw = false;

			SDL_Event event;
			while ( SDL_PollEvent(&event) )
			{
				switch( event.type )
				{
					case SDL_QUIT:
						done = 1;
						break;
					case SDL_KEYDOWN:
						redraw = true;
						switch( event.key.keysym.sym )
						{
							case SDLK_ESCAPE:
								done = 1;
								redraw = false;
								break;
							case SDLK_SPACE:
								break;
							case SDLK_UP:
								xrot -= 15;
								break;
							case SDLK_DOWN:
								xrot += 15;
								break;
							case SDLK_PAGEUP:
							case SDLK_LEFT:
								yrot -= 15;
								break;
							case SDLK_PAGEDOWN:
							case SDLK_RIGHT:
								yrot += 15;
								break;
							case SDLK_INSERT:
								zoom -= 1;
								break;
							case SDLK_DELETE:
								zoom += 1;
								break;
							case SDLK_l:
								zoom=3;
								yrot=45;
								xrot=30;
								break;
							case SDLK_m:
								zoom=3;
								yrot=45+90;
								xrot=30;
								break;
							case SDLK_p:
								zoom=3;
								yrot=45+180;
								xrot=30;
								break;
							case SDLK_o:
								zoom=3;
								yrot=45+270;
								xrot=30;
								break;
							// Chose the wireframe polygon mode
							case SDLK_f:
								glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
								break;
							// Chose the textured polygon mode
							case SDLK_t:
								glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
								break;
							case SDLK_s:
								screenshot( modelFile, W, H );
								break;
							default:
								redraw=false;
	    					};
						break;
		    			case SDL_VIDEOEXPOSE:
						redraw = true;
						break;
					case SDL_VIDEORESIZE:
						W = event.resize.w;
						H = event.resize.h;
					    	initDisplay( W, H );
					default: ;
				}
			}

			if( redraw == true )
			{
				DrawGLScene( width, length, height, model );
			}

			SDL_Delay( 50 );
		}	
	} // shot

	delete model;

	SDL_Quit();
	return 1;
}


string ocDataDirPrefix( const string& s )
{
	return s;
}


string ocConfigDirPrefix( const string& s )
{
	return s;
}
