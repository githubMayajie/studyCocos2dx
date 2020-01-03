/*!
 *	@file			EAGLView.m
 *
 *	@title			EAGLView
 *
 *	@author			Created by Diney Bomfim on 15/2/11.
 *	
 *	@copyright		© 2011 DB-Interactive. All rights reserved.
 *
 *	@discussion		This project is an integrant part of the serie of tutorials "All about OpenGL ES 2.x"
 *					created on http://db-in.com/blog/
 *
 *					<ul>
 *						<li>1th - http://db-in.com/blog/2011/01/all-about-opengl-es-2-x-part-13/</li>
 *						<li>2nd - http://db-in.com/blog/2011/02/all-about-opengl-es-2-x-part-23/</li>
 *						<li>3rd - http://db-in.com/blog/2011/05/all-about-opengl-es-2-x-part-33/</li>
 *					</ul>
 *
 *					This tutorial contains a platform specific part for the iPhone iOS 3.2
 *					and may need modifications before you can run it.
 */

#import "EAGLView.h"
#import "CubeExample.mm"

#pragma mark -
#pragma mark Global Properties
//**********************************************************************************************************
//
//  Global Properties
//
//**********************************************************************************************************

// The number of Frames Per Second which this application will run.
#define FPS			60

// Holds a Global variable to access the UIView.
static EAGLView *eagl;

// Holds a Global variable to access the parsed texture.
static ParseImage *parseImage;

#pragma mark -
#pragma mark Implementation
//**********************************************************************************************************
//
//  Implementation
//
//**********************************************************************************************************

@implementation EAGLView

#pragma mark -
#pragma mark Initialization
//**********************************************************************************************************
//
//  Initialization
//
//**********************************************************************************************************

- (id)init
{
	if ((self = [super init]))
	{
		CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
		
		// Set the properties to EAGL.
		// If the color format here is set to kEAGLColorFormatRGB565, you'll not be able
		// to use texture with alpha in this EAGLLayer.
        eaglLayer.opaque = YES;
		eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
										[NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking,
										kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat,
										nil];
		
		// Set this EAGLView to be accessed later on.
		eagl = self;
		
		// Parses this image which is in main bundle.
		parseImage = [[ParseImage alloc] initWithImage:[UIImage imageNamed:@"cube_example.jpg"]];
    }
	
    return self;
}

#pragma mark -
#pragma mark Self Public Methods
//**********************************************************************************************************
//
//  Self Public Methods
//
//**********************************************************************************************************

- (void) renderFrame
{
	// Pre-Rendering step.
	preRender();
	
	// Drawing step.
	drawScene();
	
	// Render step.
	render();
}

#pragma mark -
#pragma mark Self Class Methods
//**********************************************************************************************************
//
//  Self Class Methods
//
//**********************************************************************************************************

+ (void) propertiesToCurrentColorbuffer
{
	// Sets the properties to the currently bound Color Renderbuffer based on the EAGLContext in use.
	[[EAGLContext currentContext] renderbufferStorage:GL_RENDERBUFFER
										 fromDrawable:(CAEAGLLayer *)eagl.layer];
}

+ (void) presentColorbuffer
{
	// Presents the currently bound Color Renderbuffer to to the EAGLContext in use.
	[[EAGLContext currentContext] presentRenderbuffer:GL_RENDERBUFFER];
}

+ (ParseImage *) image
{
	// Gives access to the parsed image.
	return parseImage;
}

#pragma mark -
#pragma mark Override Public Methods
//**********************************************************************************************************
//
//  Override Public Methods
//
//**********************************************************************************************************

- (void) applicationDidFinishLaunching:(UIApplication *)application
{
	// Starts a UIWindow with the size of the device's screen.
	CGRect rect = [[UIScreen mainScreen] bounds];	
	_window = [[UIWindow alloc] initWithFrame:rect];
	
	if(!(self = [super initWithFrame:rect])) 
	{
		[self release];
		return;
	}
	
	// Makes that UIWindow the key window and show it. Additionaly add this UIView to it.
	[_window makeKeyAndVisible];
	[_window addSubview:self];
	
	// Creates the EAGLContext and set it as the current one.
	_context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
	[EAGLContext setCurrentContext:_context];
	
	// Initializes the OpenGL in the CubeExample.mm
	initOpenGL(self.bounds.size.width, self.bounds.size.height);
	
	// Creates a loop routine to execute the OpenGL render "FPS" times per second.
	[NSTimer scheduledTimerWithTimeInterval:(1.0 / FPS) 
									 target:self
								   selector:@selector(renderFrame)
								   userInfo:nil
									repeats:YES];	
}

+ (Class) layerClass
{
	// This is mandatory to work with CAEAGLLayer in Cocoa Framework.
    return [CAEAGLLayer class];
}

- (void)dealloc
{
	clearOpenGL();
	[_context release];
	[_window release];
	[parseImage release];
	
	eagl = nil;
	parseImage = nil;
	
    [super dealloc];
}

@end
