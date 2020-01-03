/*!
 *	@file			EAGLView.h
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

#import <QuartzCore/QuartzCore.h>
#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

#import "ParseImage.h"

@class EAGLContext;

/*!
 *	@class			EAGLView
 *	
 *	@abstract		Makes the bridge between OpenGL's core and Cocoa Framework.
 *
 *	@discussion		This class should not be taken as a good example of structure. It is just to give the
 *					arguments to CubeExample.mm, the principal file.
 */
@interface EAGLView : UIView <UIApplicationDelegate>
{
@private
	UIWindow		*_window;
	
	// EAGL context, the Apple's version of EGL API.
	EAGLContext		*_context;
}

/*!
 *	@method			propertiesToCurrentColorbuffer
 *
 *	@abstract		Sets the properties to a Color Renderbuffer.
 *
 *	@discussion		This method is required by EAGL, because is mandatory in EAGL to use a frame buffer
 *					and a color render buffer and also use the EAGL context to set the properties to
 *					the color render buffer.
 */
+ (void) propertiesToCurrentColorbuffer;

/*!
 *	@method			presentColorbuffer
 *
 *	@abstract		Presents a color render buffer to the device's screen.
 *
 *	@discussion		This method is required by EAGL, because EAGL doesn't use the pattern of swap the
 *					OpenGL's internal buffers, it makes the render to an off-screen color render buffer
 *					and then presents it on the device's screen.
 */
+ (void) presentColorbuffer;

/*!
 *	@method			image
 *
 *	@abstract		Returns an image properly parsed.
 *
 *	@discussion		OpenGL doesn't import image files directly, it must receive a binarized array of pixels.
 */
+ (ParseImage *) image;

@end
